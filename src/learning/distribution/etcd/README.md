# ETCD

## Lease

- 租约是一个有时间期限的承诺；
- Lease 是一个独立的结构对象，它具有一段指定的生命期限；
- key 绑定到 Lease 上就能实现 ttl 自动删除的功能；
- key 和 Lease 是多对一的关系，这种设计能够提升 etcd 的内部性能，并且具有相当的灵活性；
- Lease 的创建（ Grant ）和销毁（ Revoke ）都要持久化（ 到 boltdb ），这之前要走 raft 状态机（ 涉及到 wal 的持久化 ）；
- Lease 结构体数据存储在 boltdb 中一个叫做 “lease” 的 bucket 中；
- Lease 的 CheckPoint 会走 raft 状态机，但是不会持久化到后端 boltdb ；
- Lease 机制要慎用（ 明确自己场景 ），因为它并没有严格保证 ttl 的时间准确的增减；

## MVCC

- 数据多版本，不覆盖更新，并发的时候各搞各的，做到 MVCC。在具有 MVCC 的系统中要小心，所有的黑历史都记着呢；
- 版本用 revision 表示，其内有两个：主版本号和次版本号，主版本在每次写事务单调递增，次版本号在事务内多次更新时候递增；
- 在 MVCC 的设计中，删除被显式搞成两个步骤，用户的删其实是写入一个删除标记，真正的空间释放是异步的；
- compact 使用的参数是主版本号，释放这个版本以前所有的被标记删除的数据；
- etcd 用的 B 树是全内存的，解析了所有的用户 key ，用来管理用户 key 到 revision 的映射关系；
- boltdb 是一个纯粹的 kv 存储引擎，内部无覆盖写，B+ 树索引 kv 的映射，适合读多写少的场景（写事务串行，读事务并发）；
- etcd 在 boltdb 的基础上又给每一个 key 实现了多版本数据，能够很方便的实现事务并发的控制；
- etcd 的 MVCC 其实并没有给性能带来多大提升，很容易理解，因为底层用的是 boltdb ，天然就是读并发、写串行的引擎，上层无论怎么做，都逃不掉这一点，但在 etcd 中 MVCC 机制对于 watch 机制却很关键；

```go
type revision struct {
    main int64      // 主版本号
    sub int64       // 次版本号
}

type keyIndex struct {
    key         []byte       // 用户 key
    modified    revision     // 最新的版本号；
    generations []generation // 一个key有可能是有多次创建，删除的，每一次的轮回都是一个 generation
}
```
