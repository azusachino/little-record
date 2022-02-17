# The client

## Partition Assign Strategy

`partition.assignment.strategy`

- Range
- RoundRobin
- Sticky
- CooperativeSticky

## Consumer Coordinator & Group Coordinator

1. Find coordinator
2. Join group
3. Sync group
4. heartbeat

## Transaction

1. at most once
2. at least once
3. exactly once

**Idempotence**:

```java
props.put(ProducerConfig.ENABLE_IDEMPOTENCE_CONFIG, "true");
// set config correct otherwise throws ConfigException
// set retrys > 0
// set ack "-1" or "all"
```

**Transaction**:

为了实现事务，应用程序必须提供唯一的 transactionalId

```java
props.put(ProducerConfig.TRANSACTION_ID_CONFIG, "transactionId");

producer.initTransaction();
producer.beginTransaction();

try {
    tx operations...
    producer.commitTransaction();
} catch (ProducerFencedException e) {
    producer.abortTransaction();
}
producer.close();
```
