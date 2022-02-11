# ElasticSearch

## 性能调优

### 硬件方面

1. 内存： 64G 最理想，16-32 也可以，8G 内存可能适得其反
2. CPU：需要 2 到 8 核
3. 硬盘：SSD 最好，避免 NAS
4. 网络：避免跨多个数据中心的集群

### 索引优化

1. 批量提交
2. 增加 refresh 时间间隔 `index.refresh_interval`
3. 调整索引缓存配置 `indices.memory.index_buffer_size: 10%`, `indices.memory.min_index_buffer_size: 48mb`
4. 调整 translog 配置 `index.translog.sync_interval: 5s`, `index.translog.flush_threshold_size: 512mb`
5. 注意 `_id` 字段的使用
6. 注意 `_all` 和 `_source` 字段的使用
7. 合理配置`index`的属性
8. 减少副本数量

### 操作系统

1. 较大的文件描述符，例如 64000
2. 设置 MMAP `sysctl -w vm.max_map_count=655300` 或者设置 `/etc/sysctl.conf`

### JVM

1. 不要调整 JVM 参数
2. 不要修改垃圾收集器
3. 给 Lucene 留下一半的内存空间
4. 不要超过 32G
5. 避免使用过大内存
6. 最好禁用系统 swapping

### ES 参数

1. 禁止调整线程池
2. 设置集群和节点名称
3. 设置路径
4. 设置 minimum_master_nodes

```sh
ES_JAVA_OPTS 配置ES JVM heap内存限制
cluster.name   集群名称
node.name 节点名称
node.master 节点角色配置，true表示可以成为主节点，false不能成为主节点
node.data   节点角色配置，true表示可以成为数据节点，false不能成为数据节点
node.ingest 节点角色配置，true表示可以成为协调节点，false不能成为协调节点（简单解释，请以官方为准）
node.attr.rack  节点服务器所在的机柜信息，可能在数据分布中起到指导作用
discovery.zen.ping.unicast.hosts 配置自动发现IP列表
discovery.zen.minimum_master_nodes 防止脑裂，这个参数控制的是，一个节点需要看到的具有master节点资格的最小数量，然后才能在集群中做操作。官方的推荐值是(N/2)+1，其中N是具有master资格的节点的数量（我们的情况是3，因此这个参数设置为2，但对于只有2个节点的情况，设置为2就有些问题了，一个节点DOWN掉后，你肯定连不上2台服务器了，这点需要注意）。
gateway.recover_after_nodes 控制集群在达到多少个节点之后才会开始数据恢复,通过这个设置可以避免集群自动相互发现的初期,shard分片不全的问题,假如es集群内一共有5个节点,就可以设置为5,那么这个集群必须有5个节点启动后才会开始数据分片,如果设置为3,就有可能另外两个节点没存储数据分片
network.host 绑定服务的IP地址
transport.tcp.port 内部通信端口
http.port 对外服务端口
path.data 数据存放目录
bootstrap.memory_lock 锁住内存，确保ES不使用swap
bootstrap.system_call_filter        系统调用过滤器，建议禁用该项检查，因为很多检查项需要Linux 3.5以上的内核，否则会报错。
```

## 部署 ES

1.内核参数优化

```sh
vim /etc/sysctl.conf

vm.max_map_count = 655300
vm.swappiness = 1
```

2.执行 docker

```sh
#!/bin/bash
# 删除已退出的同名容器
docker ps -a | grep es_master |egrep "Exited|Created" | awk '{print $1}'|xargs -i% docker rm -f % 2>/dev/null
# 启动
docker run --name es_master \
        -d --net=host \
        --restart=always \
        --privileged=true \
        --ulimit nofile=655350 \
        --ulimit memlock=-1 \
        --memory=12G \
        --memory-swap=-1 \
        --cpuset-cpus='31-34' \
        --volume /data:/data \
        --volume /etc/localtime:/etc/localtime \
        -e TERM=dumb \
        -e ES_JAVA_OPTS="-Xms8g -Xmx8g" \
        -e cluster.name="iyunwei" \
        -e node.name="MASTER-100" \
        -e node.master=true \
        -e node.data=false \
        -e node.ingest=false \
        -e node.attr.rack="0402-K03" \
        -e discovery.zen.ping.unicast.hosts="192.168.2.100:9301,192.168.2.101:9301,192.168.2.102:9301,192.168.2.100:9300,192.168.2.102:9300,192.168.2.103:9300,192.168.3.100:9300,192.168.3.101:9300,192.168.3.102:9300,192.168.3.103:9300" \
        -e discovery.zen.minimum_master_nodes=2 \
        -e gateway.recover_after_nodes=5 \
        -e network.host=0.0.0.0 \
        -e transport.tcp.port=9301 \
        -e http.port=9201 \
        -e path.data="/data/iyunwei/master" \
        -e path.logs=/data/elastic/logs \
        -e bootstrap.memory_lock=true \
        -e bootstrap.system_call_filter=false \
        -e indices.fielddata.cache.size="25%" \
```

## es sort

with special field name for `_score` to sort by score, and `_doc` to sort by index order.

## ik

基于 ik 分词器，增加 filter

```json
{
  "settings": {
    "index.number_of_shards": 3,
    "index.max_result_window": 20000000,
    "number_of_replicas": 1,
    "index.refresh_interval": "60s",
    "index.highlight.max_analyzed_offset": "10000",
    "analysis": {
      "analyzer": {
        "my_analyzer": {
          "tokenizer": "ik_max_word",
          "char_filter": ["camel_case_filter", "special_character_filter"]
        }
      },
      "char_filter": {
        "camel_case_filter": {
          "type": "pattern_replace",
          "pattern": "(?<=\\p{Lower})(?=\\p{Upper})",
          "replacement": " "
        },
        "special_character_filter": {
          "type": "pattern_replace",
          "pattern": "(?:\\p{Punct})",
          "replacement": " "
        }
      }
    }
  },
  "mappings": {
    "record": {
      "_all": { "enabled": false },
      "dynamic": true,
      "date_detection": true,
      "properties": {
        "id": {
          "type": "keyword",
          "index": true
        },
        "record": {
          "type": "text",
          "index": "true",
          "analyzer": "my_analyzer",
          "search_analyzer": "ik_smart",
          "fields": {
            "exact": {
              "type": "keyword"
            }
          },
          "fielddata": true
        }
      }
    }
  }
}
```
