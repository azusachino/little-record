# The Producer

## Producer Client

1. configure client parameters and create relative client instances
2. prepare the message
3. send the message
4. close client instance

```java
public class KafkaProducer {
    public static final String BROKER_LIST = "localhost:9092";
    public static final String TOPIC = "TOPIC_DEMO";

    public static Properties initConfig() {
        Properties props = new Properties();
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, BROKER_LIST);
        props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        // 若使用JsonSerializer，会多裹一层双引号
        props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, StringSerializer.class);
        // 为提高性能，不接收partition leader的ack
        props.put(ProducerConfig.ACKS_CONFIG, "0");
        props.put(ProducerConfig.RETRIES_CONFIG, 3);
        props.put(ProducerConfig.BATCH_SIZE_CONFIG, 16384);
        props.put(ProducerConfig.LINGER_MS_CONFIG, 1);
        props.put(ProducerConfig.BUFFER_MEMORY_CONFIG, 33554432);
        props.put(ProducerConfig.REQUEST_TIMEOUT_MS_CONFIG, 3000);
        return props;
    }

    public static void main(String[] args) {
        Properties props = initConfig();
        KafkaProducer<String, String> producer = new KafkaProducer<>(props);
        ProducerRecord<String, String> record = new ProducerRecord<>(topic, "Hello Kafka");
        try {
            producer.send(record);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

## Main Process

![.](https://res.weread.qq.com/wrepub/epub_25462424_80)

## Important parameters

**acks**:

- `acks=1`, default. After producer send message, only need the Partiton leader writes successfully, the producer will ack
- `acks=0`, no response need after send message. (Mostly used for high performance)
- `acks=-1` or `acks=all`. Waiting for all ISR write successfully

**max.request.size**: the max size of request body, default 1048576B aka 1MB.

**retries&retry.backoff.ms**: retry times, default 0. backoff time gap, default 100.

**compression.type**: default none, can be configured as gzip, snappy, lz4.

**connections.max.idle.ms**: wait such time before close idle connection, default 54000(ms) aka 9min.

**linger.ms**: configure producerBatch waiting time before other producerRecord join, default 0. The producer will send out message when producerBatch is full or linger.ms is timeout.

**receive.buffer.bytes**: configure Socket Receive Buffer size, default 32768B aka 32KB. -1 for OS DEFAULT_VALUE.

**send.buffer.bytes**: configure Socket Send Buffer size, default 131072B aka 128KB. -1 for OS DEFAULT_VALUE.

**request.timeout.ms**: the max time gap producer waiting for response, default 30000(ms).
