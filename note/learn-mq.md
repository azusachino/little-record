# Learn RocketMQ

## RocketMQ

1. 支持集群模型, 负载均衡, 水平扩展能力
2. 亿级别的消息堆积能力
3. 采用零拷贝的原理, 顺序写盘, 随机读
4. 丰富的API
5. 底层通信框架采用NettyNIO框架
6. 采用NameServer代替Zookeeper
7. 强调集群无单点, 可扩展, 任意一点高可用, 水平可扩展
8. 消息失败重试机制, 消息可查询
9. 开源社区活跃

## RockerMQ - 概念模型

- Producer: 消息生产者 - 业务系统
- Consumer: 消息消费者 - 后台系统 异步消费
- Push Consumer: 向Consumer对象注册监听
- Pull Consumer: 主动请求Broker拉取消息
- Producer Group: 生产者集合, 发送一类消息
- Consumer Group: 消费者集合, 接受一类消息
- Broker: MQ消息服务(中转角色, 用于消息存储与生产消费转发)

## RockerMQ - 环境搭建

## RockerMQ - 生产者使用

1. 创建生产者对象 DefaultMQProducer
2. 设定NamesrvAddr
3. 启动生产者服务
4. 创建消息并发送

## RocketMQ - 消费者使用

1. 创建消费者对象 DefaultMQPushConsumer
2. 设置NamesrvAddr及其消费位置 ConsumeFromWhere
3. 订阅主题 Subscribe
4. 注册监听并消费 registerMessageListener

## RocketMQ - 四种集群环境构建详解

- 单点模式
- 主从模式
- 双主模式
- 双主双从模式, 多主多从模式
