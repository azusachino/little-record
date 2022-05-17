# Istio

- 连接：对网格内部的服务之间的调用所产生的流量进行智能管理，并以此为基础，为微服务的部署、测试和升级等操作提供有力保障。
- 安全：为网格内部的服务之间的调用提供认证、加密和鉴权支持，在不侵入代码的情况下，加固现有服务，提高其安全性。
- 策略：在控制面定制策略，并在服务中实施。
- 观察：对服务之间的调用进行跟踪和测量，获取服务的状态信息。

---

- 服务发现 - Netflix Eureka | Consul
- 自动负载平衡 — Netflix Zuul
- 路由，熔断，重试，故障转移，故障注入 — Netflix Ribbon, Hytrix
- 用于访问控制，限流，A / B 测试，流量拆分和配额的策略实施 — Zuul
- metrics，日志和链路跟踪 - ELK | Stack driver
- 安全的服务间通信 - TLS

## How it works

- Data Plane
- Control Plane

Service mesh uses a proxy to intercept all your network traffic, allowing a broad set of application-aware features based on configuration you set.

- Traffic Management
- Observability
- Security capabilities
