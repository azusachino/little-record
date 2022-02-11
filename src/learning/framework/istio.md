# Istio

Service Mesh 是一种控制处理服务间通信的基础设施层，可以在云原生场景下帮助应用程序在复杂的服务拓扑间可靠的传递请求。

![Service Mesh](https://img.luozhiyun.com/20201025200941.png)

Istio 就是一个 Service Mesh 实现的形态，用于服务治理的开放平台，并且 Istio 是与 K8s 紧密结合的适用于云原生场景的平台。

## `Istio` Architecture

![https://img.luozhiyun.com/20201025200944.svg](https://img.luozhiyun.com/20201025200944.svg)

Istio 分别由数据平面（Data plane）和控制平面（Control plane）组成。

数据平面由网格内的 Proxy 代理和应用组成，这些代理以 sidecar 的形式和应用服务一起部署。每一个 sidecar 会接管进入和离开服务的流量，并配合控制平面完成流量控制等方面的功能。

控制平面用于控制和管理数据平面中的 sidecar 代理，完成配置的分发、服务发现、和授权鉴权等功能，可以统一的对数据平面进行管理。

在上面的组件中，Proxy 代理默认使用 Envoy 作为 sidecar 代理，Envoy 是由 Lyft 内部于 2016 年开发的，其性能和资源占用都有着很好的表现，能够满足服务网格中对透明代理的轻量高性能的要求。

Pilot 组件主要功能是将路由规则等配置信息转换为 sidecar 可以识别的信息，并下发给数据平面，完成流量控制相关的功能。

Citadel 是专门负责安全的组件，内置有身份和证书管理功能，可以实现较为强大的授权和认证等操作。

Galley 主要负责配置的验证、提取和处理等功能。
