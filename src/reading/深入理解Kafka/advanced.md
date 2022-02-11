# Advanced Kafka

## TTL

```java
// create a ttl message
ProducerRecord<String, String> record = new ProducerRecord<>(topic, 0, System.currentTimeMillis(), null, "msg_ttl", new RecordHeaders().add(new RecordHeader("ttl", ByteUtils.longToBytes(20))));

producer.send(record).get();
```

## Delayed Queue

队列是存储消息的载体，延时队列存储的对象是延时消息。所谓的“延时消息”是指消息被发送以后，并不想让消费者立刻获取，而是等待特定的时间后，消费者才能获取这个消息进行消费，延时队列一般也被称为“延迟队列”。注意延时与 TTL 的区别，延时的消息达到目标延时时间后才能被消费，而 TTL 的消息达到目标超时时间后会被丢弃。

## 死信队列和重试队列

由于某些原因消息无法被正确地投递，为了确保消息不会被无故地丢弃，一般将其置于一个特殊角色的队列，这个队列一般称为死信队列。后续分析程序可以通过消费这个死信队列中的内容来分析当时遇到的异常情况，进而可以改善和优化系统。

重试队列其实可以看作一种回退队列，具体指消费端消费消息失败时，为了防止消息无故丢失而重新将消息回滚到 broker 中。

## Message track

对消息轨迹而言，最常见的实现方式是封装客户端，在保证正常生产消费的同时添加相应的轨迹信息埋点逻辑。

可以参考 Kafka 中的做法，它将消费位移信息保存在主题`__consumer_offset` 中。对应地，我们同样可以将轨迹信息保存到 Kafka 的某个主题中，比如图 11-12 中的主题 trace_topic。

## Message audit

Chaperone, Confluent Control Center, Kafka Monitor

## Message proxy

Kafka REST Proxy

## 选型

- 优先队列
- 延时队列
- 死信队列
- 消费模式: push / pull
- 广播消费
- 回溯消费
- 消费堆积+持久化
- 消费轨迹
- 消费审计
- 消息过滤
- 多租户
- 多协议支持
- 跨语言支持
- 流量控制
- 消息顺序性
- 安全机制
- 消息幂等性
- 事务性消息

---

- 性能
- 可靠性、可用性
- 运维管理
- 社区力度、生态发展
