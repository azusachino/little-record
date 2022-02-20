# Monitor

## Monitor data source

Kafka 自身提供的监控指标（包括 broker 和主题的指标，而集群层面的指标可以通过各个 broker 的指标值累加来获得）都可以通过 JMX（Java Management Extension，Java 管理扩展）来获取，在使用 JMX 之前需要确保 Kafka 开启了 JMX 的功能（默认关闭）。
