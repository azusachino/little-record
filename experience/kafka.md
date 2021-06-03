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
