# Prometheus

Prometheus 既是一个时序数据库，又是一个监控系统，更是一套完备的监控生态解决方案。

From metrics to insight. Power your metrics and alerting with a leading open-source monitoring solution.

## Prometheus 的特点

- 通过 PromQL 实现多维度数据模型的灵活查询。
- 定义了开放指标数据的标准，自定义探针（如 Exporter 等），编写简单方便。
- PushGateway 组件让这款监控系统可以接收监控数据。
- 提供了 VM 和容器化的版本。
- Go 语言编写，拥抱云原生。
- 采用拉模式为主、推模式为辅的方式采集数据。
- 二进制文件直接启动，也支持容器化部署镜像。
- 支持多种语言的客户端，如 Java、JMX、Python、Go、Ruby、.NET、Node.js 等语言。
- 支持本地和第三方远程存储，单机性能强劲，可以处理上千 target 及每秒百万级时间序列。
- 高效的存储。平均一个采样数据占 3.5B 左右，共 320 万个时间序列，每 30 秒采样一次，如此持续运行 60 天，占用磁盘空间大约为 228GB（有一定富余量，部分要占磁盘空间的项目未在这里列出）。
- 可扩展。可以在每个数据中心或由每个团队运行独立 Prometheus Server。也可以使用联邦集群让多个 Prometheus 实例产生一个逻辑集群，当单实例 Prometheus Server 处理的任务量过大时，通过使用功能分区（sharding）+联邦集群（federation）对其进行扩展。
- 出色的可视化功能。Prometheus 拥有多种可视化的模式，比如内置表达式浏览器、Grafana 集成和控制台模板语言。它还提供了 HTTP 查询接口，方便结合其他 GUI 组件或者脚本展示数据。
- 精确告警。Prometheus 基于灵活的 PromQL 语句可以进行告警设置、预测等，另外它还提供了分组、抑制、静默等功能防止告警风暴。
- 支持静态文件配置和动态发现等自动发现机制，目前已经支持了 Kubernetes、etcd、Consul 等多种服务发现机制，这样可以大大减少容器发布过程中手动配置的工作量。
- 开放性。Prometheus 的 client library 的输出格式不仅支持 Prometheus 的格式化数据，还可以在不使用 Prometheus 的情况下输出支持其他监控系统（比如 Graphite）的格式化数据。

## Prometheus 的局限性

- Prometheus 主要针对性能和可用性监控，不适用于针对日志（Log）、事件（Event）、调用链（Tracing）等的监控。
- Prometheus 关注的是近期发生的事情，而不是跟踪数周或数月的数据。因为大多数监控查询及告警都针对的是最近（通常不到一天）的数据。Prometheus 认为最有用的数据是最近的数据，监控数据默认保留 15 天。
- 本地存储有限，存储大量的历史数据需要对接第三方远程存储。
- 采用联邦集群的方式，并没有提供统一的全局视图。
- Prometheus 的监控数据并没有对单位进行定义。
- Prometheus 对数据的统计无法做到 100%准确，如订单、支付、计量计费等精确数据监控场景。
- Prometheus 默认是拉模型，建议合理规划网络，尽量不要转发。

## Prometheus 架构

![.](https://prometheus.io/assets/architecture.png)

### Job/Exporter

Job/Exporter 属于 Prometheus target，是 Prometheus 监控的对象。

### Pushgateway

Prometheus 是拉模式为主的监控系统，它的推模式就是通过 Pushgateway 组件实现的。

使用 Pushgateway 时需要记住的另一个问题是，Pushgateway 不会自动删除推送给它的任何指标数据。因此，必须使用 Pushgateway 的 API 从推送网关中删除过期的指标。

### Service Discovery

作为下一代监控系统的首选解决方案，Prometheus 通过服务发现机制对云以及容器环境下的监控场景提供了完善的支持。

### Prometheus Server

Prometheus 服务器是 Prometheus 最核心的模块。它主要包含抓取、存储和查询这 3 个功能。

- 抓取：通过服务发现组件，周期性地从上面介绍的 Job、Exporter、Pushgateway 这 3 个组件中通过 HTTP 轮询的形式拉取监控指标数据。
- 存储：抓取到的监控数据通过一定的规则清理和数据整理（抓取前使用服务发现提供的 relabel_configs 方法，抓取后使用作业内的 metrics_relabel_configs 方法），会把得到的结果存储到新的时间序列中进行持久化。
- 查询：Prometheus 持久化数据以后，客户端就可以通过 PromQL 语句对数据进行查询。

### DashBoard

在 Prometheus 架构图中提到，Web UI、Grafana、API client 可以统一理解为 Prometheus 的 Dashboard。

### AlertManager

Alertmanager 是独立于 Prometheus 的一个告警组件，需要单独安装部署。Prometheus 可以将多个 Alertmanager 配置为一个集群，通过服务发现动态发现告警集群中节点的上下线从而避免单点问题，Alertmanager 也支持集群内多个实例之间的通信
