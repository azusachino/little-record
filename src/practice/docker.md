# Docker Related

## `network`

- bridge 为每一个容器分配、设置 IP 等，并将容器连接到一个 docker0 虚拟网桥，默认为该模式。
- host 容器将不会虚拟出自己的网卡，配置自己的 IP 等，而是使用宿主机的 IP 和端口。
- none 容器有独立的 Network namespace，但并没有对其进行任何网络设置，如分配 veth pair 和网桥连接，IP 等。
- container 新创建的容器不会创建自己的网卡和配置自己的 IP，而是和一个指定的容器共享 IP、端口范围等。

### `bridge`

![..](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/4f5206a75a884cc2968ceb1f6c14acb6~tplv-k3u1fbpfcp-zoom-1.image)

Bridge 桥接模式的实现步骤主要如下：

Docker Daemon 利用 veth pair 技术，在宿主机上创建一对对等虚拟网络接口设备，假设为 veth0 和 veth1。而
veth pair 技术的特性可以保证无论哪一个 veth 接收到网络报文，都会将报文传输给另一方。
Docker Daemon 将 veth0 附加到 Docker Daemon 创建的 docker0 网桥上。保证宿主机的网络报文可以发往 veth0；
Docker Daemon 将 veth1 添加到 Docker Container 所属的 namespace 下，并被改名为 eth0。如此一来，宿主机的网络报文若发往 veth0，则立即会被 Container 的 eth0 接收，实现宿主机到 Docker Container 网络的联通性；同时，也保证 Docker Container 单独使用 eth0，实现容器网络环境的隔离性。

### `host`

- host 网络模式需要在创建容器时通过参数 --net host 或者 --network host 指定；
- 采用 host 网络模式的 Docker Container，可以直接使用宿主机的 IP 地址与外界进行通信，若宿主机的 eth0 是一个公有 IP，那么容器也拥有这个公有 IP。同时容器内服务的端口也可以使用宿主机的端口，无需额外进行 NAT 转换；
- host 网络模式可以让容器共享宿主机网络栈，这样的好处是外部主机与容器直接通信，但是容器的网络缺少隔离性。

![.](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/32ab62e6be9d4b4dbe9280ca3b9206f9~tplv-k3u1fbpfcp-zoom-1.image)

### `none`

- none 网络模式是指禁用网络功能，只有 lo 接口 local 的简写，代表 127.0.0.1，即 localhost 本地环回接口。在创建容器时通过参数 --net none 或者 --network none 指定；
- none 网络模式即不为 Docker Container 创建任何的网络环境，容器内部就只能使用 loopback 网络设备，不会再有其他的网络资源。可以说 none 模式为 Docke Container 做了极少的网络设定，但是俗话说得好“少即是多”，在没有网络配置的情况下，作为 Docker 开发者，才能在这基础做其他无限多可能的网络定制开发。这也恰巧体现了 Docker 设计理念的开放。

### `container`

- Container 网络模式是 Docker 中一种较为特别的网络的模式。在创建容器时通过参数 --net container:已运行的容器名称|ID 或者 --network container:已运行的容器名称|ID 指定；
- 处于这个模式下的 Docker 容器会共享一个网络栈，这样两个容器之间可以使用 localhost 高效快速通信。

![.](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/905bc296603243ad8ee09e13b651e5ba~tplv-k3u1fbpfcp-zoom-1.image)
