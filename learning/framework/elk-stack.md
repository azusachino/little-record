# 学习 ELK

## Elastic Search

### 主要功能

- 分布式搜索引擎
- 大数据近实时分析引擎

### 文档 - 可搜索数据的最小单位

#### 元数据

- \_index - 索引
- \_type 类型
- \_id 唯一 id
- \_source 原始 json 数据
- \_version 版本信息
- \_score 相关性打分

#### 索引 index 是文档的容器, 是一类文档的集合

### 分布式特性

- 存储的水平扩容
- 提高系统的可用性, 部分节点停止服务, 整个集群服务不受影响

### 节点 - 一个 ES 实例(java 进程)

#### Master Node

#### DataNode & Coordinating Node

- 保存数据的节点
- 负责接受 Client 请求, 分发到合适的节点, 最终把结果汇总

### 分片 (Primary Shard & Replica Shard)

- 主分片, 用以解决数据水平扩展的问题
- 副本, 用以解决数据高可用的问题

### 文档的 CRUD

#### CREATE

index 就像 sql 中的库，type 就像 sql 中的表，document 就像 sql 中的记录。

### 查看单个索引

语法： `GET /索引名`

效果：返回指定索引的信息

例子： `GET /product` 返回结果解析：

![https://progor.oss-cn-shenzhen.aliyuncs.com/img/20190818021353.png](https://progor.oss-cn-shenzhen.aliyuncs.com/img/20190818021353.png)

- aliases：是索引的别名，由于我们没有定义索引别名所以没有数据
- mappings：是索引中存储的数据的结构信息，由于上图的索引 product 中并没有存储 document，而且我们也没有指定，所以也为空。mappings 定义了文档的字段的数据类型和搜索策略等信息。
- settings：索引的配置信息

  - creation_date 是创建日期（毫秒级别的时间戳）
  - number_of_shards 是主分片数量
  - number_of_replicas 是副本分片的数量
  - uuid 是索引的 id
  - version 是索引的版本信息
  - provided_name 是索引名一个包含了 mappings 的结果图：

    ![https://progor.oss-cn-shenzhen.aliyuncs.com/img/20190707201333.png](https://progor.oss-cn-shenzhen.aliyuncs.com/img/20190707201333.png)

### **查看所有索引**

命令： `GET /_cat/indices?v`

效果：查看所有索引，显示索引的健康状态等信息。

【如果没有 v 选项，那么就不会有第一行关于该列意义的头部】

返回结果解析：

![https://progor.oss-cn-shenzhen.aliyuncs.com/img/20190818020758.png](https://progor.oss-cn-shenzhen.aliyuncs.com/img/20190818020758.png)

- health: 索引的健康状态（受分片的运行状态影响）【集群管理的知识点】
- status: 索引是开启的还是关闭的
- index: index 的名称
- uuid：索引的 UUID
- pri: primary shared 的数量
- rep: replicated shared 的数量
- docs.count: 文档 document 的数量
- docs.deleted: 被删除的文档 document 的数量
- store.size：总数据的大小
- pri.store.size：主分片上的数据的大小（这里因为只运行了一个服务节点，所以没有可运行的副本分片，所以总数据大小等于主分片的数据大小）

### **修改副本分片数量**

```plain
PUT /product/_settings
{
  "index":{
    "number_of_replicas":2
  }
}
```

关闭索引是为了**避免修改索引的配置时他人进行文档读写**。关闭索引后，就只能获取索引的配置信息，而不能读写索引中的 document。有时候也用于关闭一些无用的索引来减少资源消耗。语法：

- 关闭索引：`POST /索引名/_close`
- 打开索引：`POST /索引名/_open`

### \_score
