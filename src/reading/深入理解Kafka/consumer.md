# The Consumer

## Consumer & Consumer Group

Consumer is responsible for topics, and pulls messages. Each Consumer belongs to a consumer Group, Messages will only be consumed by a consumer in the consumer group subscribed to this topic.

## Consumer Client

1. configure consumer client parameters & create consumer client instance.
2. subscribe to topic.
3. pull messages and consume.
4. submit consume offset.
5. close consumer client instance.

```java
public class KafkaConsumer {
    public static final String BROKER_LIST = "localhost:9092";
    public static final String TOPIC = "TOPIC_DEMO";
    public static final String GROUP_ID = "group.demo";
    public static final AtomicBoolean isRunning = new AtomicBoolean(true);

    public Properties initConfig() {
        Properties props = new Properties();
        props.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        props.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class);
        props.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, BROKER_LIST);
        props.put(ConsumerConfig.GROUP_ID_CONFIG, GROUP_ID);
        props.put(ConsumerConfig.CLIENT_ID_CONFIG, "NO.1");

        return props;
    }

    public static void main(String[] args) {
        Properties props = initConfig();
        KafkaConsumer<String, String> consumer = new KafkaConsumer<>(props);
        consumer.subscribe(Collections.singletonList(topic));
        try {
            while(isRunning.get()) {
                ConsumerRecords<String, String> records = consumer.poll(Duration.ofMillis(1000));
                for(ConsumerRecord<String, String> record: records) {
                    System.out.println("topic = " + record.topic() + ", partition = " + record.partition());
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            consumer.close();
        }
    }
}
```

## Client Parameters

**fetch.min.bytes**: configure min data size when pulling message, default 1B.

**fetch.max.bytes**: configure max data size when pulling message, default 52428800B aka 50MB.

**fetch.max.wait.ms**: the max waiting time when no message was qualified by `fetch.min.bytes`

**max.partition.fetch.bytes**: configure max data size when a single consumer pulling message from single partition, default 1048675B aka 1MB.

**max.poll.records**: max records in one poll, default 500.

**connections.max.idle**: waiting such time before close idle connection, default 540000(MS) aka 9min.

**exclude.internal.topics**: exclude internal topics `__consumer_offsets` & `__transaction_state`, default true. if true, only `subscribe(Collection)` is allowed; if false, `subcribe(Pattern)` is also allowed.

**receive.buffer.bytes**: configure Socket Receive Buffer size, default 32768B aka 32KB. -1 for OS DEFAULT_VALUE.

**send.buffer.bytes**: configure Socket Send Buffer size, default 131072B aka 128KB. -1 for OS DEFAULT_VALUE.

**request.timeout.ms**: the max time gap consumer waiting for response, default 30000(ms).

**metadta.max.age.ms**: metadata timeout, default 300000(ms) aka 5min.

**reconnect.backoff.ms**: configure waiting time before reconnect, default 50(ms).

**retry.backoff.ms**: configure retry waiting time, default 100(ms).

**isolation.level**: `read_uncommitted`, `read_committed`
