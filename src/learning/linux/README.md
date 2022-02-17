# Linux Learning

## IO 多路复用

Go 配合协程在网络 IO 上实现了异步流程的代码同步化。核心就是用 epoll 池来管理网络 fd。

## epoll 池原理

- `epoll_create` => `int epoll_create(int size)`
- `epoll_ctl` => `int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)`
- `epoll_wait` => ``

```c++
// create the epoll fd pool
epollfd = epoll_create(1024);
if (epollfd == -1) {
    perror("epoll_create");
    exit(EXIT_FAILURE);
}
if (epollctl(epollfd, EPOLL_CTL_ADD, 11, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
}
```

```c++
struct file_operations {
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    __poll_t (*poll) (struct file *, struct poll_table_struct *);
    int (*open) (struct inode *, struct file *);
    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
    // ....
};
```

- 内部管理 fd 使用了高效的红黑树结构管理，做到了增删改之后性能的优化和平衡；
- epoll 池添加 fd 的时候，调用 file_operations->poll ，把这个 fd 就绪之后的回调路径安排好。通过事件通知的形式，做到最高效的运行；
- epoll 池核心的两个数据结构：红黑树和就绪列表。红黑树是为了应对用户的增删改需求，就绪列表是 fd 事件就绪之后放置的特殊地点，epoll 池只需要遍历这个就绪链表，就能给用户返回所有已经就绪的 fd 数组；

---

- IO 多路复用的原始实现很简单，就是一个 1 对多的服务模式，一个 loop 对应处理多个 fd ；
- IO 多路复用想要做到真正的高效，必须要内核机制提供。因为 IO 的处理和完成是在内核，如果内核不帮忙，用户态的程序根本无法精确的抓到处理时机；
- fd 记得要设置成非阻塞的；
- epoll 池通过高效的内部管理结构，并且结合操作系统提供的 poll 事件注册机制，实现了高效的 fd 事件管理，为高并发的 IO 处理提供了前提条件；
- epoll 全名 eventpoll，在 Linux 内核下以一个文件系统模块的形式实现，所以有人常说 epoll 其实本身就是文件系统也是对的；
- socketfd，eventfd，timerfd 这三种”文件“fd 实现了 poll 接口，所以网络 fd，事件 fd，定时器 fd 都可以使用 epoll_ctl 注册到池子里。我们最常见的就是网络 fd 的多路复用；
- ext2，ext4，xfs 这种真正意义的文件系统反倒没有提供 poll 接口实现，所以不能用 epoll 池来管理其句柄。

## Linux Memory

```sh
cat /proc/meminfo

MemTotal:         688576 kB     总内存
MemFree:          153736 kB     空闲内存
MemAvailable:     339884 kB     可用内存
Buffers:              16 kB     给文件的缓冲大小
Cached:           267672 kB     高速缓冲存储器
SwapCached:           36 kB     被高速缓冲存储用的交换空间的大小
Active:           222900 kB     活跃使用中的高速缓冲存储器页面文件大小
Inactive:         123700 kB     不经常使用中的告诉缓冲存储器文件大小
Active(anon):      31800 kB     活跃的匿名内存（进程中堆上分配的内存,是用malloc分配的内存）
Inactive(anon):    57272 kB     不活跃的匿名内存
Active(file):     191100 kB     活跃的file内存，//file内存：磁盘高速缓存的内存空间和“文件映射(将物理磁盘上的文件内容与用户进程的逻辑地址直接关联)”的内存空间，其中的内容与物理磁盘上的文件相对应
Inactive(file):    66428 kB　　　　不活跃的file内存
Unevictable:           0 kB　　　　不能被释放的内存页
Mlocked:               0 kB　　　　mlock()系统调用锁定的内存大小
SwapTotal:       2097148 kB　　　　交换空间总大小
SwapFree:        2096884 kB　　　　空闲交换空间
Dirty:                 0 kB　　　　等待被写回到磁盘的大小
Writeback:             0 kB　　　　正在被写回的大小
AnonPages:         78876 kB       未映射页的大小
Mapped:            28556 kB　　　　设备和文件映射大小
Shmem:             10160 kB　　　　已经被分配的共享内存大小
Slab:             102916 kB　　　　内核数据结构缓存大小
SReclaimable:      49616 kB　　　 可收回slab的大小
SUnreclaim:        53300 kB　　　 不可回收的slab的大小
KernelStack:        4416 kB      kernel消耗的内存
PageTables:         6028 kB      管理内存分页的索引表的大小
NFS_Unstable:          0 kB      不稳定页表的大小
Bounce:                0 kB      在低端内存中分配一个临时buffer作为跳转，把位于高端内存的缓存数据复制到此处消耗的内存
WritebackTmp:          0 kB      USE用于临时写回缓冲区的内存
CommitLimit:     2441436 kB      系统实际可分配内存总量
Committed_AS:     308028 kB      当前已分配的内存总量
VmallocTotal:   34359738367 kB   虚拟内存大小
VmallocUsed:      179588 kB　　　 已经被使用的虚拟内存大小
VmallocChunk:   34359310332 kB   malloc 可分配的最大的逻辑连续的内存大小
HardwareCorrupted:     0 kB      删除掉的内存页的总大小(当系统检测到内存的硬件故障时)
AnonHugePages:      6144 kB      匿名 HugePages 数量
CmaTotal:              0 kB　　　 总的连续可用内存
CmaFree:               0 kB      空闲的连续内存
HugePages_Total:       0　　　　  预留HugePages的总个数
HugePages_Free:        0　　　　　池中尚未分配的 HugePages 数量
HugePages_Rsvd:        0      　 表示池中已经被应用程序分配但尚未使用的 HugePages 数量
HugePages_Surp:        0　　　　  这个值得意思是当开始配置了20个大页，现在修改配置为16，那么这个参数就会显示为4，一般不修改配置，这个值都是0
Hugepagesize:       2048 kB     每个大页的大小
DirectMap4k:      108416 kB　　　映射TLB为4kB的内存数量
DirectMap2M:      940032 kB　　　映射TLB为2M的内存数量
DirectMap1G:           0 kB     映射TLB为1G的内存数量
```

## Inter Process Communication

### Pipe

管道一般用于父子进程之间相互通信

1. 父进程使用 pipe 系统调用创建一个管道
2. 父进程使用 fork 系统调用创建一个子进程
3. 因为子进程会继承父进程打开的文件句柄，所以父子进程可以通过新创建的管道进行通信
