# 学习ELK

## Elastic Search

### 主要功能

- 分布式搜索引擎
- 大数据近实时分析引擎

### 文档 - 可搜索数据的最小单位

#### 元数据

- _index - 索引
- _type 类型
- _id 唯一id
- _source 原始json数据
- _version 版本信息
- _score 相关性打分

#### 索引 index是文档的容器, 是一类文档的集合

### 分布式特性

- 存储的水平扩容
- 提高系统的可用性, 部分节点停止服务, 整个集群服务不受影响

### 节点 - 一个ES实例(java进程)

#### Master Node

#### DataNode & Coordinating Node

- 保存数据的节点
- 负责接受Client请求, 分发到合适的节点, 最终把结果汇总

### 分片 (Primary Shard & Replica Shard)

- 主分片, 用以解决数据水平扩展的问题
- 副本, 用以解决数据高可用的问题

### 文档的CRUD

#### CREATE
