# Kafka

## 查询消费情况

```sh
# 查看group列表
docker exec -it kafka /bin/bash
cd /opt/bitnami/kafka/bin
./kafka-consumer-groups.sh --bootstrap-server kafka:9092 --list

# 查看group消费情况

docker exec -it kafka bash
cd /opt/bitnami/kafka/bin/
./kafka-consumer-groups.sh --bootstrap-server kafka:9092 --group logstash --describe
```

## KafkaConfig

```java
public class KafkaConfig implements InitializingBean {

    private static final String NO_ACK = "0";

    @Resource
    private KafkaProperties kafkaProperties;

    @Resource
    private CustomProperties customProperties;

    /**
     * Message Producer
     *
     * @return KafkaTemplate<String, String>
     */
    @Bean
    public KafkaTemplate<String, String> kafkaTemplate() {
        Map<String, Object> props = new HashMap<>(16);

        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, this.kafkaProperties.getBootstrapServers());
        // 若使用JsonSerializer，会多裹一层双引号
        props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        // 为提高性能，不接收partition leader的ack
        props.put(ProducerConfig.ACKS_CONFIG, NO_ACK);
        props.put(ProducerConfig.RETRIES_CONFIG, 3);
        //  16kb
        props.put(ProducerConfig.BATCH_SIZE_CONFIG, 16384);
        // 生产者用来缓存等待发送到服务器的消息的内存总字节数 32MB
        props.put(ProducerConfig.BUFFER_MEMORY_CONFIG, 33554432);

        // properties
        props.put(ProducerConfig.LINGER_MS_CONFIG, 1);
        props.put(ProducerConfig.REQUEST_TIMEOUT_MS_CONFIG, 3000);
        props.put(ProducerConfig.MAX_BLOCK_MS_CONFIG, 300);
        props.put(ProducerConfig.MAX_IN_FLIGHT_REQUESTS_PER_CONNECTION, 20);

        ProducerFactory<String, String> producerFactory = new DefaultKafkaProducerFactory<>(props);
        return new KafkaTemplate<>(producerFactory);
    }

    /**
     * 自动生成主题
     */
    @Override
    public void afterPropertiesSet() {
        // only run when topics configured
        List<CustomProperties.TopicReference> topicReferences = this.customProperties.getTopics();
        if (CollUtil.isNotEmpty(topicReferences)) {
            Properties properties = new Properties();
            properties.put(AdminClientConfig.BOOTSTRAP_SERVERS_CONFIG, this.kafkaProperties.getBootstrapServers());
            try (AdminClient adminClient = AdminClient.create(properties)) {
                List<NewTopic> topicList = topicReferences
                        .stream()
                        .map(tr -> new NewTopic(tr.getName(), tr.getPartition(), tr.getReplicationFactor()))
                        .collect(Collectors.toList());
                // send the request & wait for all results
                adminClient.createTopics(topicList).all().get();
            } catch (Exception ignored) {
            }
        }
    }
}
```
