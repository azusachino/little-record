# Redis

> 使用Redis有哪些好处

- 速度快，因为数据存在内存中，类似于HashMap，HashMap的优势就是查找和操作的时间复杂度都是O(1)
- 支持丰富数据类型，支持string，list，set，sorted set，hash
- 支持事务，操作都是原子性，所谓的原子性就是对数据的更改要么全部执行，要么全部不执行
- 丰富的特性：可用于缓存，消息，按key设置过期时间，过期后将会自动删除

> redis相比memcached有哪些优势？

- memcached所有的值均是简单的字符串，redis作为其替代者，支持更为丰富的数据类型
- redis的速度比memcached快很多
- redis可以持久化其数据

> redis常见性能问题和解决方案?

(1) Master最好不要做任何持久化工作，如RDB内存快照和AOF日志文件

(2) 如果数据比较重要，某个Slave开启AOF备份数据，策略设置为每秒同步一次

(3) 为了主从复制的速度和连接的稳定性，Master和Slave最好在同一个局域网内

(4) 尽量避免在压力很大的主库上增加从库

(5) 主从复制不要用图状结构，用单向链表结构更为稳定，即：Master <- Slave1 <- Slave2 <- Slave3...

Master写内存快照，save命令调度rdbSave函数，会阻塞主线程的工作，当快照比较大时对性能影响是非常大的，会间断性暂停服务，所以Master最好不要写内存快照。

Master AOF持久化，如果不重写AOF文件，这个持久化方式对性能的影响是最小的，但是AOF文件会不断增大，AOF文件过大会影响Master重启的恢复速度。Master最好不要做任何持久化工作，包括内存快照和AOF日志文件，特别是不要启用内存快照做持久化,如果数据比较关键，某个Slave开启AOF备份数据，策略为每秒同步一次。

Master调用BGREWRITEAOF重写AOF文件，AOF在重写的时候会占大量的CPU和内存资源，导致服务load过高，出现短暂服务暂停现象。

Redis主从复制的性能问题，为了主从复制的速度和连接的稳定性，Slave和Master最好在同一个局域网内

> MySQL里有2000w数据，redis中只存20w的数据，如何保证redis中的数据都是热点数据

redis 内存数据集大小上升到一定大小的时候，就会施行数据淘汰策略。redis 提供 6种数据淘汰策略：

voltile-lru：从已设置过期时间的数据集（server.db[i].expires）中挑选最近最少使用的数据淘汰

volatile-ttl：从已设置过期时间的数据集（server.db[i].expires）中挑选将要过期的数据淘汰

volatile-random：从已设置过期时间的数据集（server.db[i].expires）中任意选择数据淘汰

allkeys-lru：从数据集（server.db[i].dict）中挑选最近最少使用的数据淘汰

allkeys-random：从数据集（server.db[i].dict）中任意选择数据淘汰

no-enviction（驱逐）：禁止驱逐数据

> redis 最适合的场景

Redis最适合所有数据in-memory的场景，虽然Redis也提供持久化功能，但实际更多的是一个disk-backed的功能，跟传统意义上的持久化有比较大的差别，那么可能大家就会有疑问，似乎Redis更像一个加强版的Memcached，那么何时使用Memcached,何时使用Redis呢?

如果简单地比较Redis与Memcached的区别，大多数都会得到以下观点：

- Redis不仅仅支持简单的k/v类型的数据，同时还提供list，set，zset，hash等数据结构的存储。
- Redis支持数据的备份，即master-slave模式的数据备份。
- Redis支持数据的持久化，可以将内存中的数据保持在磁盘中，重启的时候可以再次加载进行使用

> Memcache与Redis的区别都有哪些？

1.存储方式

Memecache把数据全部存在内存之中，断电后会挂掉，数据不能超过内存大小。

Redis有部份存在硬盘上，这样能保证数据的持久性。

2.数据支持类型

Memcache对数据类型支持相对简单。

Redis有复杂的数据类型。

3.使用底层模型不同

它们之间底层实现方式 以及与客户端之间通信的应用协议不一样。

Redis直接自己构建了VM 机制 ，因为一般的系统调用系统函数的话，会浪费一定的时间去移动和请求。

4.value大小

redis最大可以达到1GB，而memcache只有1MB

> Redis的同步机制了解么？

主从同步。第一次同步时，主节点做一次bgsave，并同时将后续修改操作记录到内存buffer，待完成后将rdb文件全量同步到复制节点，复制节点接受完成后将rdb镜像加载到内存。加载完成后，再通知主节点将期间修改的操作记录同步到复制节点进行重放就完成了同步过程。

- slave发送sync命令到master
- Master启动一个后台进程, 将redis中的数据快照保存到文件中
- master将保存数据期间接收到的写命令缓存起来
- master完成写文件操作后, 将该文件发送给slave
- 使用新的aof文件替换掉旧的aof文件
- master将这期间收集的增量写命令发送给slave

---
增量同步过程

1. master接收到用户的操作指令, 判断是否需要传播到slave
2. 将操作记录追加到aof文件
3. 将操作传播到其他slave: 1. 对齐主从库 2. 网相应缓存写入指令
4. 将缓存中的数据发送给slave

> 是否使用过Redis集群，集群的原理是什么？

Redis Sentinel着眼于高可用，在master宕机时会自动将slave提升为master，继续提供服务。

Redis Cluster着眼于扩展性，在单个redis内存不足时，使用Cluster进行分片存储。

> 如何在亿级数据中找到有一定规则的key?

1. keys [pattern], 当前操作会产生阻塞, 但可以同时返回所有key
2. scan cursor [Match pattern] [Count count] [Type type]

- 基于游标的迭代器, 需要基于上一次的游标延续之前的迭代过程
- 以0作为游标开始一次新的迭代, 知道命令返回游标0完成一次遍历
- 不保证每次执行都返回某个给定数量的元素, 支持模糊查询
- 一次返回的数量不可控, 只能是大概率符合count参数

```sh
keys a*
scan 0 match user* count 10 type zset
```

> Why Redis so fast?

- 完全基于内存, 绝大部分请求是纯粹的内存操作, 执行效率高
- 数据结构简单, 对数据操作也简单
- 采用单线程, 单线程也能处理高并发请求, 多核也可以启动多实例
- 使用多路I/O复用模型, NIO

> 多路复用模型

Redis采用的I/O多路复用函数: epoll/kqueue/evport/select

- 因地制宜(不同的平台, 不同的函数)
- 优先选择时间复杂度为O(1)的I/O多路复用函数作为底层实现
- 以时间复杂度为O(1)的select作为保底
- 基于React设计模式监听I/O事件

> redis 数据类型

- string
- hash
- list
- set
- sorted set

> 底层数据类型基础

- SDS(Simple dunamic string)
- linkedlist
- dictionary
- skiplist
- integer
- ziplist
- object

## java面试题总览

> 常见的缓存策略有哪些? 如何做到缓存与DB里的数据一致性, 你们项目中用到了什么缓存系统, 如何设计的

TODO
> 如何防止缓存击穿和雪崩

TODO
> 缓存数据过期后的更新如何设计

TODO
> redis的list结构相关的操作

TODO
> Redis的数据结构都有哪些?

TODO
> Redis的使用要注意什么, 讲讲持久化方式, 内存设置, 集群的应用和优劣势, 淘汰策略等

TODO
> Redis2和Redis3的区别, Redis3的内部通讯机制

TODO
> 当前redis有哪些玩法, 各自优缺点, 场景

TODO
> Memcache的原理, 哪些数据适合放在缓存里

TODO
> Redis和Memcache内存管理的区别

TODO
> Redis的并发竞争问题如何解决, 了解Redis事务的CAS操作吗?

TODO
> Redis的选举算法和流程是怎样的?

TODO
> Redis持久化的机制, AOF和RDB的区别

TODO
> Redis的集群是怎么同步数据的

TODO
> 知道哪些Redis的优化操作

TODO
> Redis的主从复制机制原理

TODO
> Redis的线程模型是什么

TODO
> 请思考一个方案, 设计一个可以控制缓存总体大小的自动适应的本地缓存

TODO
> 如何看待缓存的使用(本地缓存, 集中式缓存), 简述本地缓存和集中式缓存的优缺点, 本地缓存在并发使用时的注意事项

TODO
