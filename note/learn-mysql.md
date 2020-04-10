# Mysql

## mysql查询优化原理分析

操作：查询条件放到子查询中，子查询只查主键ID，然后使用子查询中确定的主键关联查询其他的属性字段；  
原理：减少回表操作；  

```sql
select * from test a inner join (select id from test where val=4 limit 300000,5) b on a.id=b.id;
```

## 乐观锁&悲观锁

![mysql-lock](../resources/mysql/mysql-lock.jpg)
select...for update是MySQL提供的实现悲观锁的方式.在MySQL中用悲观锁务必须确定走了索引，而不是全表扫描，否则将会将整个数据表锁住.  
利用数据版本号(version)机制(或者是时间戳)是乐观锁最常用的一种实现方式

## 数据库中间层

## MyCat

## 关系型数据库

### 如何设计一个关系型数据库

- 存储 (文件系统)
- 程序实例
  1. 存储管理
  2. 缓存机制
  3. SQL解析
  4. 日志管理
  5. 权限划分
  6. 异常机制(容灾)
  7. 索引管理 -- 快速访问 (二叉查找树)
  8. 锁管理

### BTree索引

1. 根节点至少包括两个孩子
2. 树中每个节点最多含有m个孩子(m>=2)
3. 除根节点和叶节点外, 其他每个至少有ceil(m/2)个孩子
4. 所有叶子节点都位于同一层

### B+Tree索引

1. 非叶子节点的子树指针与关键字个数相同
2. 非叶子节点的子树指针p[i],指向关键字值(k[i],k[i+1])的子数
3. 非叶子节点仅用来索引, 数据都保存在叶子节点中
4. 所有叶子节点均有一个链指针指向下一个叶子节点

- Hash索引
- BitMap索引

### 密集索引和稀疏索引的区别

- 密集索引文件中的每个搜索码值都对应一个索引值
- 稀疏索引文件只为索引码的某些值建立索引项

> 如何定位并优化慢查询sql?

- 根据慢日志定位慢查询sql (`show variables like '%query%'`)
  1. `long_query_time = 1.000s`
  2. `show_query_log = ON`
- 使用explain等工具分析sql (`explain xxx`)
  1. type (index, all)
  2. extra (using filesort, using temporary)
- 修改sql或者尽量让sql找索引

> 联合索引的最左匹配原则的成因?

啊
> 索引是建立得越多越好吗?

啊
