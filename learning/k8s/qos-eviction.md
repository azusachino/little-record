# K8S 资源管理

## limit & request

在 k8s 中，CPU 和内存资源主要通过 limit 和 request 进行限制；在调度时，kube-scheduler 只会按照 request 的值进行计算，真正限制资源的是 limit。

```yml
apiVersion: v1
kind: Pod
metadata:
  name: cpu-demo
  namespace: cpu-example
spec:
  containers:
    - name: cpu-demo-ctr
      image: vish/stress
      resources:
        limits:
          cpu: "1"
        requests:
          cpu: "0.5"
      args:
        - -cpus
        - "2"
```

在创建好 pod 之后，使用`kubectl top`查看资源使用情况，发现 cpu 的使用不会超过 1。在容器没有指定 request 的时候，request 的值和 limit 默认相等。

## Qos 模型与 Eviction

k8s 中有三种 QOS：

1. `Guaranteed`：Pod 中所有 Container 的所有 Resource 的 limit 和 request 都相等且不为 0；
2. `Burstable`：pod 不满足 Guaranteed 条件，但是其中至少有一个 container 设置了 requests 或 limits ；
3. `BestEffort`：pod 的 requests 与 limits 均没有设置；

当宿主机资源紧张的时候，kubelet 对 Pod 进行 Eviction（即资源回收）时会按照 Qos 的顺序进行回收，回收顺序是：BestEffort>Burstable>Guaranteed

Eviction 有 Soft 和 Hard 两种模式。Soft Eviction 允许你为 Eviction 过程设置 `eviction-max-pod-grace-period`，然后等待一个用户配置的 grace period 之后，再执行 Eviction，而 Hard 则立即执行。
