# Log Storage

## Directory Layout

![.](https://res.weread.qq.com/wrepub/epub_25462424_354)

![.](https://res.weread.qq.com/wrepub/epub_25462424_358)

## Evolution of Log Format

v0 -> v1 -> v2

## Log Index

offset, timestamp

serval cases when log segment:

1. current segment file is larger than `log.segment.bytes` (default 107341824B aka 1GB)
2. the max timestamp in current segment minus system timestamp is greater than `log.roll.ms`(default 168 aka 7days)
3. offset index file or timestamp index file size reaches `log.index.size.max.bytes` (default 10485760 aka 10MB)
4. Append message's offset minus current segment offset is greater than Integer.MAX_VALUE

**offset**:

1. relativeOffset: message's offset refers to baseOffset, 4 Byte
2. position: physical location in log segment file, 4 Byte

---

00000000000.index

```s
0000 0006 0000 009c => relativeOffset=6, position=156
```

**timestamp**:

1. timestamp: the biggest timestamp in current segment, 8 Byte
2. relativeOffset, 4 Byte

---

000000000.timeindex

```s
0000 0163 639e 5a35 0000 0006 => 1526384712245, 6
```

## Log Cleanup

`log.cleanup.policy`, default _delete_. To enable compact, set `log.cleanup.policy` to _compact_ and set `log.cleaner.enable` to _true_.

1. Log Retention: According to reserve strategy, directly delete unqualified segment.
2. Log Compaction: Compact message by key, reserve the latest version.

**Retention**:

`log.retention.check.interval.ms` default 300000 aka 5min. 3 strategy: time based, log size based, log start offset based.

- time based

![.](https://res.weread.qq.com/wrepub/epub_25462424_405)

Use `log.retention.hours`, `log.retention.minutes`, `log.retention.ms` to configure retentionMs. _default_: `log.retention.hours`=168, aka 7days.

- log size based

![.](https://res.weread.qq.com/wrepub/epub_25462424_406)

`log.retention.bytes`, default is 1073741824, aka 1GB.

## Disk Storage

**Page Cache**: 把磁盘中的数据缓存到内存中，把对磁盘的访问变为对内存的访问

**IO**: NOOP, CFQ, DEADLINE, ANTICIPATORY, default CFQ

![.](https://res.weread.qq.com/wrepub/epub_25462424_417)

**ZeroCopy**:零拷贝是指将数据直接从磁盘文件复制到网卡设备中，而不需要经由应用程序之手
