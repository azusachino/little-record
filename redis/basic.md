# Learn Redis

## 数据结构

- String
- Hash
- List
- Set
- Zset

### 单值缓存

- Set key value
- Get key

### 对象缓存

- Set user:1 value(json格式数据)
- MSET user:1:name az user:1:balance 1888 -> MGET user:1:name user:1:balance

### 分布式锁

SETNX (set if not exist)

- SETNX product:10001 true  // 返回1代表获取锁成功
- SETNX product:10001 false  // 返回0代表获取锁失败
- DEL product:10001  // 执行完业务释放锁
- SET product:10001 true ex 10 nx // 防止程序意外中止导致死锁

### 计数器

- INCR article:readCount:1001 (将key中存储的值加一)

### Hash常用操作

- HMSET HMGET

### SET

- 抽奖 SADD SMEMBERS SRANDOMEMBER / SPOP
- 点赞, 收藏, 标签
