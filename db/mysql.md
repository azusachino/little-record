# 关于MYSQL的面试题

## mysql乐观锁和悲观锁的概念? 原理机制

## MySQL的原子性是怎么保证的

- undo log
- binlog (数据库的变更, 搜索引擎的数据也需要变更)
- redo log (innoDB)

### binlog

binlog记录了数据库表结构和表数据变更，比如update/delete/insert/truncate/create。它不会记录select（因为这没有对表没有进行变更）  
binlog我们可以简单理解为：存储着每条变更的SQL语句（当然从下面的图看来看，不止SQL，还有trcID「事务Id」等等）  
主要作用: 复制和恢复数据  

### redo log

redo log的存在为了：当我们修改的时候，写完内存了，但数据还没真正写到磁盘的时候。此时我们的数据库挂了，我们可以根据redo log来对数据进行恢复。因为redo log是顺序IO，所以写入的速度很快，并且redo log记载的是物理变化（xxxx页做了xxx修改），文件的体积很小，恢复速度很快。![redo-log](../resources/mysql/redo-log.jpg)
