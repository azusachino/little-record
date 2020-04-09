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
