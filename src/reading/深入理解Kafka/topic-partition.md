# TOPIC PARTITION

## Topic Management

if broker config `auto.create.topics.enable` was true, a topic(`num.partitions[1]`, `default.replication.factor[1]`) will be created when producer send message.

- `kafka-topics`.sh
- TopicCommand

```java
public static void createTopic() {
    String[] options = new String[] {
        "--zookeeper", "localhost:2181/kafka",
        "--create",
        "--replication-factor", "1",
        "--partitions", "1",
        "--topic", "DEMO_TOPIC"
    }
    kafka.admin.TopicCommand.main(options);
}
```

## KafkaAdminClient

- `CreateTopicsResult#createTopics(Collection<NewTopic>)`
- `DeleteTopicsResult#deleteTopics(Collection<String>)`
- `ListTopicsResult#listTopics()`
- `DescribeTopicsResult#describeTopics(Collection<String>)`
- `DescribeConfigsResult#describeConfigs(Collection<ConfigResource>)`
- `AlterConfigsResult#alterConfigs(Map<ConfigResource, Config>)`
- `CreatePartitionsResult#createPartitions(Map<String, NewPartitions>)`

Create Topic Demo:

```java
String broker = "localhost:9092";
String topic = "DEMO_TOPIC";

Properties props = new Properties();
props.put(AdminClientConfig.BOOTSTRAP_SERVERS_CONFIG, broker);
props.put(AdminClientConfig.REQUEST_TIMEOUT_MS_CONFIG, 30000);

AdminClient ac = AdminClient.create(props);
NewTopic nt = new NewTopic(topic, 4, (short) 1);
CreateTopicsResult result = ac.createTopics(Collections.singletonList(nt));

try {
    // block and wait for response
    result.all().get();
} catch (Exception e) {
    e.printStackTrace();
} finally {
    ac.close();
}
```
