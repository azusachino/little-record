# REDIS

## 简介

- 高性能Key-Value服务器
- 多种数据结构
- 丰富的功能
- 高可用分布式支持

## 第一章 Redis初识

### 什么是Redis

- 开源软件
- 基于键值的存储服务系统
- 多种数据结构

---
RedisObject:

- type: string, hash, list, set, sorted set
- encoding: raw, int, ht, zipmap, linkedlist, ziplist, intset

### 特性回顾

- 速度快 (10w OPS)
- 持久化 (断电不丢失数据) (AOF, RDB)
- 多种数据结构 (String, HashTable, LinkedList, Set, ZSet, BitMaps, HyperLogLog, GEO)
- 支持多种编程语言 (Java, Php, Python, Ruby, Lua)
- 功能丰富 (发布订阅, Lua脚本, 事务, pipeline)
- 简单 (23000 lines of code, 不依赖外部库, 单线程模型)
- 主从复制
- 高可用, 分布式 (Redis-Sentinel, Redis-Cluster)

### 单机安装

```bash
wget http://download.redis.io/releases/redis-5.0.x.tar.gz
tar -xzf redis-5.0.x.tar.gz
ln -s redis-5.0.x redis
cd redis
make && make install
```

- redis-server -> start
- redis-cli -> client
- redis-benchmark
- redis-check-aof -> repair aof
- redis-check-dump -> repair RDB
- redis-sentinel

---

start the server

1. redis-server
2. command line
3. config file

---

validate the start

1. `ps -ef | grep redis`
2. `netstat -antpl | grep redis`
3. `redis-cli -h ip -p port ping`

---
return value

- return status
- return error
- return integer
- return string
- return multiple strings

### 典型使用场景

- 缓存系统
- 计数器
- 消息订阅系统
- 排行榜
- 社交网络
- 实时系统

### 常用配置

- daemonize -- 守护进程
- port -- 端口
- logfile -- 日志文件
- dir -- 工作目录

## Redis API

### common command

- keys (blocking) O(n)
- dbsize O(1)
- exists key O(1)
- del key [key ...] O(1)
- expire key seconds (ttl, persist) O(1)
- type key O(1)

---

> why single thread so fast?

1. Pure Memory
2. Non-Blocking IO
3. 避免线程切换和竞态消耗

---

1. 一次只运行一条命令
2. 拒绝长(慢)命令: keys, flushall, flushdb, show lua script, mutil/exec, operate big value(collection)
3. 其实不是单线程 (fsync file descriptor)

### String

#### String结构和命令

key -.- value

---

- 缓存
- 计数器
- 分布式锁

---

- mget, mset O(n)
- get (mget, getset)
- set (setnx, set key value xx, mset)
- del
- incr
- decr
- incrby
- decrby
- append key value
- strlen key
- incrbyfloat
- getrange, setrange

### Hash

key -.- field -.- value

---

- hget O(1)
- hset O(1)
- hdel O(1)
- hgetall O(n)
- hexists
- hlen
- hmget, hmset O(n)
- hvals, hkeys O(n)

### List

key -.- elements

- 有序
- 可重复
- 左右都可操作

---

- lpop, lpush
- rpop, rpush
- linsert key before|after value newValue O(n)
- lrem key count value
- ltrim key start end O(n)
- lrange key start end O(n)
- lindex key index O(n)
- llen key O(1)
- lset key index newValue O(n)
- blpop, brpop key timeout (Blocking)

### Set

key -.- values

- 无序
- 无重复
- 集合间操作

---

- sadd,srem key element O(1)
- scard, sismember, srandmember, smembers
- sdiff, sunion, sinter + store destKey

### Sorted Set

key -.- score value

- 有序
- 无重复

---

- zadd key score element O(logN)
- zrem key element O(1)
- zscore key element
- zincrby key incrScore element O(1)
- zcard key
- zrange key start end [WITHSCORES] O((LogN)+m)
- zrangebyscore key start end [WITHSCORES] O((LogN)+m)
- zcount key minScore maxScore
- zremrangebyrank key start end
- zremrangebyscore key min max

---

- 排行榜

## Redis 客户端

### Java: Jedis

### Python: redis-py

### Go: Redigo

## 第四章 Redis瑞士军刀

### 慢查询

- 生命周期
  1. 发送命令
  2. 排队
  3. 执行命令
  4. 返回结果
- 两个配置
  - slowlog-max-len (128)
    1. 先进先出队列
    2. 固定长度
    3. 保存至内存中
  - slowlog-log-slower-than (10000)
    1. 慢查询阈值(MS)
  - 修改配置文件重启
- 三个命令
  1. slowlog get [n]
  2. slowlog len
  3. slowlog reset
- 运维经验
  1. slowlog-max-len不要设置过大, 默认10ms, 通常设置1ms
  2. slowlog-log-slower-than不要设置过小, 通常设置1000左右
  3. 理解命令生命周期
  4. 定期持久化慢查询

### pipeline

建议

1. 注意每次pipeline携带数据量
2. pipeline每次只能作用在一个Redis节点上
3. M*操作与pipeline区别

### 发布订阅

#### 角色

- 发布者(publisher)
- 订阅者(subscriber)
- 频道(channel)

#### 模型

redis-cli -> redis-server

#### 发布订阅 API

- publish channel message
- subscribe [channel]
- unsubscribe [channel]

### BitMap

#### 位图

#### BitMap API

- setbit key offset value
- getbit key offset
- bitcount key [start end]
- bitop op destkey key [key...] (op, and, or, not, xor)
- bitpos key targetBit [start][end]

### HyperLogLog

基于HyperLogLog算法: 极小空间完成独立数量统计, 本质上还是字符串

#### HyperLogLog API

- pfadd key element [element...]
- pfcount key [key...]
- pfmerge destkey sourcekey [sourcekey...]

### GEO

GEO(地理信息定位): 存储经纬度, 计算两地距离, 范围计算等

应用场景: 摇一摇, 周围餐馆

#### GEO API

- geo key longitude latitude member
- getpos key member [member...]
- georadius key longitude latitude radius m\km\ft\mi

## Redis 持久化

### 持久化的作用

- 什么是持久化
  - redis将所有数据保持在内存中, 对数据的更新异步保存到磁盘中
- 持久化方式
  - 快照 SNAPSHOT (Redis#RDB, Mysql#Dump)
  - 日志 (Mysql#Binlog, Redis#AOF, Hbase#HLog)

### RDB

- 什么是RDB
  - RedisDataBase(二进制文件)
- 触发机制
  - save (同步)
  - bgsave (异步)
    - 文件策略: 替换旧的文件
    - 复杂度: O(n)
  - 自动
    - 900s -> 1 changes
    - 300s -> 10 changes
    - 60s -> 10000 changes
- 试验

1. RDB是Redis内存到硬盘的快照, 用于持久化
2. save通常会阻塞Redis
3. bgsave不会阻塞Redis, 但会fork新进程
4. save自动配置满足任一就会被执行
5. 有些触发机制不容忽视

### AOF

#### RDB现存问题

1. 耗时, 耗性能
2. 不可控, 丢失数据

#### AOF定义

将每条执行命令记录到log中

#### AOF三种策略

- always (记录每条命令)
- everysec (每秒记录一次)
- no (OS决定记录频率)

#### AOF重写

```lua
set hello world
set hello java
set hello redis
---
set hello redis
```

1. 减少硬盘占用量
2. 加速恢复速度

---

1. bgrewriteaof
2. aof重写配置

### RDB <> AOF

1. 启动优先级
2. 体积
3. 恢复速度
4. 数据安全性
5. 轻重

#### RDB最佳策略

1. "关闭"
2. 集中管理
3. 主从, 从开?

#### AOF最佳策略

1. "开": 缓存和存储
2. AOF重写集中管理
3. everysec

#### 最佳策略

1. 小分片
2. 缓存或者存储
3. 监控(硬盘, 内存, 负载, 网络)
4. 足够的内存

### 开发运维常见问题

#### fork操作

1. 同步操作
2. 与内存大小密切相关
3. info: latest_fork_usec

---

1. 优先使用物理机或者高效支持fork操作的虚拟化技术
2. 控制Redis实例最大可用内存: maxmemory
3. 合理控制Linux内存分配策略: vm.overcommit_memory=1
4. 降低fork频率: 例如放宽AOF重写自动触发时机, 不必要的全量复制

#### 子进程开销和优化

- CPU:
  - 开销: RDB和AOF文件生成, 属于CPU密集型
  - 优化: 不做CPU绑定, 不和CPU密集型部署
- 内存:
  - 开销: fork内存开销, copy-on-write
  - 优化: echo never > /sys/kernel/mm/transparent_hugepage/enabled
- 硬盘:
  - 开销: AOF和RDB文件写入, 可以结合iostae,iotop分析
  - 优化: 不要和高硬盘负载服务部署在一起: 存储服务,消息队列等
  - no-appendfsync-on-rewrite=yes
  - 根据写入量决定磁盘类型: ssd
  - 单机多实例持久化文件目录可以考虑分盘

#### AOF追加阻塞

### 主从复制

> 单机有什么问题?

- 机器故障
- 容量瓶颈
- QPS瓶颈

#### 主从复制概念

- 数据副本
- 拓展读性能

1. 一个master可以有多个slave
2. 一个slave只能有一个master
3. 数据流向是单向的, master到slave

#### 复制的配置

- `slaveof` + ip port
- 配置 => 'slaveof ip port // slave-read-only yes'

取消复制: `slaveof no one`

查看主从关系,偏移量(offset) `info replication`  
查看run_id以及其他信息 `info server`

#### 全量复制

1. psync ? -1
2. +FULLRESYNC {runId}{offset}
3. save masterInfo
4. bgsave
5. send RDB
6. send buffer
7. flush old data

- 开销
  1. bgsave时间
  2. RDB文件网络传输时间
  3. 从节点清空数据时间
  4. 从节点加载RDB的时间
  5. 可能的AOF重写时间

#### 部分复制

1. Connection lost
2. master -> write -> repl_back_buffer
3. connecting to master
4. psync {offset} {runId}
5. continue
6. send partial data

#### 故障处理

- 自动故障转移
  - slave宕机
  - master宕机

#### 开发运维中的问题

- 读写分离
  1. 复制数据延迟
  2. 读到过期数据
  3. 从节点故障
- 主从配置不一致
  1. maxmemory不一致: 丢失数据
  2. 数据结构优化参数(hash-max-ziplist-entries): 内存不一致
- 规避全量复制
  1. 第一次全量复制无法避免
  2. 节点运行ID不匹配
  3. 复制积压缓冲区不足
- 避免复制风暴
  - 机器宕机后, 大量全量复制

## Redis Sentinel

### 主从复制高可用

- 手动故障转移
- 写能力和存储能力有限

### Sentinel架构

#### 故障转移

1. 多个sentinel发现并确定master有问题
2. 选举出一个sentinel作为领导
3. 选出一个slave作为master
4. 通知其他slave成为新的master的slave
5. 通知客户端主从变化
6. 等到老的master复活成为新master的slave

#### 安装与配置

1. 配置开启主从节点
2. 配置开启sentinel监控注解点(特殊的redis节点)

```lua
port ${port}
dir "opt/soft/redis/data"
logfile "${port}.log"
sentinel monitor mymaster 127.0.0.1 7000 2
sentinel down-after-milliseconds mymaster 30000
sentinel parallel-syncs mymaster 1
sentinel failover-timeout mymaster 180000
```

#### Jedis

1. Sentinel地址集合
2. masterName
3. 不是代理模式

```java
JedisSentinelPool sentinelPool = new JedisSentinelPool(masterName, sentinelSet, poolConfig, timeout);
try (Jedis jedis = redisSentinelPool.getResource()) {
  // jedis command
  do something
} catch(Exception e) {
  log.error(e);
}
```

#### 三个定时任务

- 每十秒每个sentinel对master和slave执行info
  - 发现slave节点
  - 确认主从关系
- 每2秒每个sentinel通过master节点的channel交换信息(pub/sub)
  - 通过_sentinel_:hello频道交互
  - 交互对节点的“看法”和自身信息
- 每3秒每个sentinel对其他sentine和redis节点执行ping

主观下线和客观下线

`sentinel monitor <masterName> <ip> <port> <quorum>`

`sentinel down-after-milliseconds <masterName> <timeout>`

#### 领导者选举

原因: 只有一个sentinel节点完成故障转移

#### 故障转移 (sentinel领导者节点完成)

1. 从slave节点选出一个“合适的”节点作为新的master
2. 对上面的slave节点执行 *slaveof no one*命令让其成为master节点
3. 向剩余slave节点发送命令, 让它们成为新master节点的slave节点, 复制规则和parallel-syncs参数有关
4. 更新原来master节点配置为slave, 并保持着对其“关注”, 当恢复后命令它去复制新的master节点

#### 选择“合适的”slave节点

1. 选择slave-priority(slave节点优先级)最高的slave节点, 如果存在则返回, 不存在则继续
2. 选择复制偏移量最大的节点(复制的最完整(最近)), 如果存在则返回, 不存在则继续
3. 选择runId最小的slave节点(最早启动的)

#### 从节点的作用

1. 副本: 高可用的基础
2. 拓展: 读能力
