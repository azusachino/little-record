# Docker Best Practices

## Container?

use `docker exec -it <containerName> bash` instead of sshd

当前 Docker 对运行在它内部应用的日志管理较薄弱，每个运行在容器内应用的日志输出统一保存在宿主机的/var/log 目录下，文件夹以容器 ID 命名。当前 Docker 仅将应用的 stdout 和 stderr 两个日志输出通过通道重定向到/var/log 下。Docker 以 JSON 消息记录每一行日志，这将导致文件增长过快，从而超过主机磁盘限额。此外，日志没有自动切分功能，docker logs 命令返回的日志记录也过于冗长。

## Docker network

## Dockerfile

Dockerfile 描述了组装镜像的步骤，其中每条指令都是单独执行的。除了 FROM 指令，其他每一条指令都会在上一条指令所生成镜像的基础上执行，执行完后会生成一个新的镜像层，新的镜像层覆盖在原来的镜像之上从而形成了新的镜像。Dockerfile 所生成的最终镜像就是在基础镜像上面叠加一层层的镜像层组建的。

- FORM
- MAINTAINER
- RUN
- CMD
- EXPOSE
- ENV
- ADD
- COPY
- ENTRYPOINT
- VOLUME
- USER
- WORKDIR
- ONBUILD

---

- 使用标签
- 谨慎选择基础镜像
- 充分利用缓存
- 正确使用 ADD 和 COPY
- 不要在 Dockerfile 中做端口映射
- 使用 Dockerfile 共享 Docker 镜像

## Docker Container Monitor

**宿主机维度**:

- CPU
- MEM
- images
- container status

**image**:

- basic information(ID, name, version, size)
- image to container
- build history

**container**:

- basic information(same as using inspect) (ID, name, image, exec cmd, port)
- running status (running, pause, terminated, exit abnormally)
- resource usage

1. `docker ps -a`
2. `docker images`
3. `docker stats` => `echo -e "GET /containers/<containerName>/stats HTTP/1.0\r\n" | nc -U /var/run/docker.sock`
4. `docker inspect -f {{.NetworkSettings.IPAddress}} 9bec` (by using -f to conform a format)
5. `docker top`
6. `docker port <containerName>`

- cAdvisor
- datadog
- prometheus

## High Available Config Center

### ETCD

- 简单：基于 HTTP+JSON 的 API，用 curl 命令就可以轻松使用。
- 安全：可选 SSL 客户认证机制。
- 快速：每个实例每秒支持一千次写操作。
- 可信：使用 Raft 算法充分实现了分布式。

**服务发现**:

- 一个强一致性、高可用的服务存储目录。
- 一种注册服务和监控服务健康状态的机制。
- 一种查找和连接服务的机制。

**消息发布与订阅**:

构建一个配置共享中心，数据提供者在这个配置中心发布消息，而消息使用者则订阅他们关心的主题，一旦相关主题有消息发布，就会实时通知订阅者。通过这种方式可以实现分布式系统配置的集中式管理与实时动态更新。

**负载均衡**:

**分布式通知与协调**:

etcd 中的 Watcher 机制，通过注册与异步通知机制，实现分布式环境下不同系统之间的通知与协调，从而对数据变更进行实时处理。

**分布式锁与竞选**:

etcd 使用 Raft 算法保持了数据的强一致性，某次操作存储到集群中的值必然是全局一致的，因此很容易实现分布式锁。锁服务有两种使用方式，一是保持独占，二是控制时序。

**分布式队列**:

创建一个先进先出的队列，保证顺序。

**集群监控**:

#### 架构

![.](https://res.weread.qq.com/wrepub/epub_26211797_118)

- HTTP Server：用于处理用户发送的 API 请求以及其他 etcd 节点的同步与心跳信息请求。
- Store：用于处理 etcd 支持的各类功能的事务，包括数据索引、节点状态变更、监控与反馈、事件处理与执行等。它是 etcd 对用户提供的大多数 API 功能的具体实现。
- Raft:Raft 强一致性算法的具体实现，是 etcd 的核心。
- WAL：即 Write Ahead Log（预写式日志），它是 etcd 的数据存储方式。除了在内存中存有所有数据的状态以及节点的索引以外，etcd 还通过 WAL 进行持久化存储。WAL 中，所有的数据在提交前都会事先记录日志。Snapshot 是为了防止数据过多而进行的状态快照；Entry 则表示存储的具体日志内容。

**术语分析**:

- Raft:etcd 所采用的保证分布式系统强一致性的算法。
- Node：一个 Raft 状态机实例。
- Member：一个 etcd 实例，管理着一个 Node，可以为客户端请求提供服务。
- Cluster：由多个 Member 构成的可以协同工作的 etcd 集群。
- Peer：对同一个 etcd 集群中另外一个 Member 的称呼。
- Client：向 etcd 集群发送 HTTP 请求的客户端。
- WAL：预写式日志，是 etcd 用于持久化存储的日志格式。
- Snapshot:etcd 防止 WAL 文件过多而设置的快照，存储 etcd 数据状态。
- Proxy:etcd 的一种模式，为 etcd 集群提供反向代理服务。
- Leader:Raft 算法中通过竞选而产生的处理所有数据提交的节点。
- Follower：竞选失败的节点作为 Raft 中的从属节点，为算法提供强一致性保证。
- Candidate:Follower 超过一定时间接收不到 Leader 的心跳时，转变为 Candidate 开始 Leader 竞选。
- Term：某个节点成为 Leader 到下一次竞选开始的时间周期，称为一个 Term。
- Index：数据项编号。Raft 中通过 Term 和 Index 来定位数据。
