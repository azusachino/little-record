# Linux 网络虚拟化

在 Linux 的世界里，文件系统挂载点、主机名、POSIX 进程间通信消息队列、进程 PID 数字空间、IP 地址、user ID 数字空间等全局系统资源被 namespace 分割，装到一个个抽象的独立空间里。

- Mount namespace
- UTS namespace
- IPC namespace
- PID namespace
- network namespace
- user namespace

无法跨 namespace 访问资源，同时 namespace 让其内部进程以为它是系统里唯一的进程，它独享系统的所有资源。  
默认情况下，Linux 进程处在和宿主机相同的 namespace，即初始的跟 namespace 中，默认享有全局系统资源

## Network Namespace

隔离 Linux 系统的设备，以及 IP 地址、端口、路由表、防火墙规则等网络资源。因此，每个 network namespace 中都有自己的网络设备(IP 地址、路由表、端口范围、/proc/net 目录等)

```sh
# create network namespace
ip netns add iris_network # create mount point in /var/run/netns
ip netns exec iris_network ip link list
# output
1: lo: <LOOPBACK> mtu 65536 qdisc noop state DOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
ip netns list
ip netns delete iris_network

# configure network namespace
ip netns exec iris_network ip link set dev lo up # set up
ip netns exec iris_network ping 127.0.0.1 # ping OK

# set up virtual eth
ip link add veth0 type veth peer name iris_veth
ip link set iris_veth netns iris_network

# start up veth
ip netns exec iris_network ifconfig iris_veth 10.1.1.1/24 up
ifconfig veth0 10.1.1.2/24 up

# ping test
ping 10.1.1.1
ip netns exec iris_network ping 10.1.1.2

# check iptables & route in iris_network
ip netns exec iris_network route
ip netns exec iris_network iptables -L
```

## veth pair

veth 是虚拟以太网卡（Virtual Ethernet）的缩写。veth 设备总是成对的，因此我们称之为 veth pair。veth pair 一端发送的数据会在另外一端接收，非常像 Linux 的双向管道。根据这一特性，veth pair 常被用于跨 network namespace 之间的通信，即分别将 veth pair 的两端放在不同的 namespace 里

```c
static netdev_tx_t veth_xmit(struct sk_buff *skb, struct net_device *dev) {
    struct veth_priv *priv = netdev_priv(dev);
    struct net_device *rcv;

    int length = skb->len;
    rcu_read_lock();
    rcv = rcu_dereference(priv->peer);

    if (unlikely(!rcv)) {
        kfree_skb(skb);
        goto drop;
    }
    if (likely(dev_forward_skb(rcv, skb) == NET_RX_SUCCESS)) {
        struct pcpu_vstats *stats = this_cpu_ptr(dev->vstats);
        u64_stats_update_begin(&stats->syncp);
        stats->bytes += length;
        stats->packets++;
        u64_stats_update_end(&stats->syncp);
    } else {
drop:
    atomic64_inc(&priv->dropped);
    }
    rcu_read_unlock();
    return NETDEV_TX_OK;
}
```

## Linux Bridge

Linux bridge 与 Linux 上其他网络设备的区别在于，普通的网络设备只有两端，从一端进来的数据会从另一端出去。Linux bridge 则有多个端口，数据可以从任何端口进来，进来之后从哪个口出去取决于目的 MAC 地址，原理和物理交换机差不多。
