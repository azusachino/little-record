# 为什么 Kafka 那么快

## 写入数据

Kafka 会把收到的消息都写入到硬盘中，它绝对不会丢失数据。为了优化写入速度 Kafka 采用了两个技术， 顺序写入和 MMFile。

### 顺序写入

Linux 对磁盘的读写优化包括：read-ahead, write-behind, 磁盘缓存

磁盘操作相比于内存操作的好处：

1. 磁盘顺序读写速度超过内存随机读写
2. JVM 的 GC 效率低，内存占用大
3. 系统冷启动后，磁盘缓存依然可用

每一个 Partition 都是一个文件，收到消息后 Kafka 会把数据插入到文件末尾，这样就导致了 Kafka 是没有办法删除数据的，它会把所有数据都保存下来。 参考官方文档：Instead, you define for how long Kafka should retain your events through a per-topic configuration setting, after which old events will be discarded.

两种删除数据的策略：

1. 基于时间
2. 基于 partition 大小

### Memory Mapped Files

其工作原理是直接利用操作系统的 Page 来实现文件到物理内存的直接映射。完成映射之后，对物理内存的操作就会被同步到硬盘上(操作系统在适当的时候)。

- I/O Scheduler 会将连续的小块写组装成大块的物理写从而提高性能
- I/O Scheduler 会尝试将一些写操作重新按顺序排好，从而减少磁盘头的移动时间
- 充分利用所有空闲内存（非 JVM 内存）。如果使用应用层 Cache（即 JVM 堆内存），会增加 GC 负担
- 读操作可直接在 Page Cache 内进行。如果消费和生产速度相当，甚至不需要通过物理磁盘（直接通过 Page Cache）交换数据
- 如果进程重启，JVM 内的 Cache 会失效，但 Page Cache 仍然可用

有一个很明显的缺陷——不可靠，写到 mmap 中的数据并没有被真正地写到硬盘，操作系统在程序主动调用 flush 的时候才把数据真正的写到硬盘。

Kafka 提供了一个参数 producer.type 来控制是不是主动 flush，如果 Kafka 写入到 mmap 之后就立即 flush 然后再返回 Producer 叫 同步 (sync)；写入 mmap 之后立即返回 Producer 不调用 flush 叫异步 (async)。

## 读取数据

### 基于 sendfile 实现 ZeroCopy

传统模式下，对文件进行传输时，具体流程细节如下：

1. 基于 sendfile 实现 ZeroCopu 调用 read 函数，文件数据被拷贝到内核缓冲区
2. read 函数返回，文件数据从内核缓冲区拷贝到用户缓冲区
3. 调用 write 函数，将文件数据从用户缓冲区拷贝到内核与 socket 相关缓冲区
4. 数据从 socket 缓冲区拷贝到相关协议引擎

在内核版本 2.1 中，引入了 sendfile 系统调用，以简化网络上和两个本地文件之间的数据传输。sendfile 的引入不仅减少了数据复制，也减少了上下文切换。

```c
sendfile(socket, file, len);
```

1. sendfile 系统调用，文件数据被拷贝到内核缓冲区
2. 从内核缓冲区拷贝至内核中 socket 相关缓冲区
3. 从 socket 相关缓冲区拷贝到协议引擎

### 批量压缩

在很多情况下，系统的瓶颈不是 CPU 或磁盘，而是网络 IO

1. 如果每个消息都压缩，压缩率很低，所以 Kafka 使用了批量压缩
2. Kafka 允许使用递归的消息集合，批量的消息可以通过压缩的形式传输并且在日志中也可以保持压缩格式，直到被消费者解压缩
3. Kafka 支持多种压缩协议，包括 Gzip 和 Snappy 压缩协议
