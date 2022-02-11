# PromQL

```sh
# 获取当前主机可用的内存空间大小，单位为MB。
node_memory_free_bytes_total  / (1024 * 1024)

# 基于2小时的样本数据，预测未来24小时内磁盘是否会满。
IF predict_linear(node_filesystem_free[2h], 24*3600) < 0
```

```sh
# 1.查询HTTP请求总数
http_requests_total

# 2.查询返回所有时间序列、指标http_requests_total，以及给定 job和handler的标签
http_requests_total{job="apiserver", handler="/api/comments"}

# 3.条件查询：查询状态码为200的请求总数
http_requests_total{code="200"}

# 4.区间查询：查询5分钟内的请求总量
http_request_total{}[5m]

# 5. 系统函数使用
# 查询系统所有HTTP请求的总量
sum(http_request_total)

# 6. 使用正则表达式，选择名称与特定模式匹配的作业（如以server结尾的作业）的时间序列
http_requests_total{job=~".*server"}

# 7. 过滤除了4XX之外所有HTTP状态码的数据
http_requests_total{status!~"4.."}

# 8. 子查询，以1次/分钟的速率采集最近30分钟内的指标数据，然后返回这30分钟内距离当前时间
# 最近的5分钟内的采集结果
rate(http_requests_total[5m])[30m:1m]

# 9. 函数rate，以1次/秒的速率采集最近5分钟内的数据并将结果以时间序列的形式返回
rate(http_requests_total[5m])
```

## 四种数据类型

- Instant vector 一组时间序列，每个时间序列包含单个样本，它们共享相同的时间戳。
- Range vector 一组时间序列，每个时间序列包含一段时间范围内的样本数据。
- Scalar 一个浮点型的数据值，没有时序。
- String 一个简单的字符串值。

### 时间序列

时间序列数据库主要按照一定的时间间隔产生一个个数据点，而这些数据点按照时间戳和值的生成顺序存放，即向量。

## 四种选择器

**匹配器**:

- = `http_requests_total{job="HelloWorld",status="200",method="POST",handler="/api/comments"}`
- != `http_requests_total{job!="HelloWorld"}`
- =~ `http_requests_total{job=~"Hello.*"}`
- !~ `node_filesystem_size_bytes{job="node",mountpoint=~"/prometheus/.*", mountpoint!~ "/prometheus/user/.*"}`

**瞬时向量选择器**:

瞬时向量选择器用于返回在指定时间戳之前查询到的最新样本的瞬时向量，也就是包含 0 个或者多个时间序列的列表。

**区间向量选择器**:

区间向量选择器返回一组时间序列，每个时间序列包含一段时间范围内的样本数据。

```sh
# 最近5分钟内的所有HTTP请求的样本数据，其中[5m]将瞬时向量选择器转变为区间向量选择器
http_request_total{}[5m]
rate(node_cpu[2m])
1 - avg without(cpu) (rate(node_cpu{mode="idle"}[2m]))
```

**偏移量修改器**:

偏移量修改器可以让瞬时向量选择器和区间向量选择器发生偏移，它允许获取查询计算时间并在每个选择器的基础上将其向前推移。

## 四大指标类型

**计数器 Counter**:

计数器类型代表一种样本数据单调递增的指标，在没有发生重置（如服务器重启、应用重启）的情况下只增不减，其样本值应该是不断增大的。

**仪表盘 Gauge**:

仪表盘类型代表一种样本数据可以任意变化的指标，即可增可减。它可以理解为状态的快照，Gauge 通常用于表示温度或者内存使用率这种指标数据，也可以表示能随时增加或减少的“总数”，例如当前并发请求的数量 node_memory_MemFree（主机当前空闲的内容大小）、node_memory_MemAvailable（可用内存大小）等。在使用 Gauge 时，用户往往希望使用它们求和、取平均值、最小值、最大值等。

**直方图**:

量化指标的平均值

**摘要**:

摘要用于表示一段时间内的数据采样的结果（通常是请求持续时间或响应大小等），但它直接存储了分位数（通过客户端计算，然后展示出来），而非通过区间来计算（Histogram 的分位数需要通过 histogram_quantile（φfloat，b instant-vector）函数计算得到）。

## 聚合操作

- sum
- min
- max
- avg
- stddev
- stdvar
- count
- count_values
- bottomk
- topk
- quantile

## 运算符

- `+, -, x, /, %, ^`, `scalar/scalar, vector/scalar, vector/vector`
- `and, or, unless`
- `==, !=, >, <, >=, <=`

## 向量匹配

- 一对一
- 一对多
- 多对多

## 内置函数

![.](https://res.weread.qq.com/wrepub/epub_34630839_103)

![.](https://res.weread.qq.com/wrepub/epub_34630839_104)
