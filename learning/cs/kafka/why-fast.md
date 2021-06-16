# 为什么 Kafka 那么快

## 写入数据

Kafka 会把收到的消息都写入到硬盘中，它绝对不会丢失数据。为了优化写入速度 Kafka 采用了两个技术， 顺序写入和 MMFile。

### 顺序写入

Linux对磁盘的读写优化包括：read-ahead, write-behind, 磁盘缓存

磁盘操作相比于内存操作的好处：

1. 磁盘顺序读写速度超过内存随机读写
2. JVM的GC效率低，内存占用大
3. 系统冷启动后，磁盘缓存依然可用

每一个Partition都是一个文件，收到消息后Kafka会把数据插入到文件末尾，这样就导致了Kafka是没有办法删除数据的，它会把所有数据都保存下来。 参考官方文档：Instead, you define for how long Kafka should retain your events through a per-topic configuration setting, after which old events will be discarded.
