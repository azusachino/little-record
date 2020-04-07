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
