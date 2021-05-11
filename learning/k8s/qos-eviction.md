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

在创建好 pod 之后，使用`kubectl top`查看资源使用情况，发现 cpu 的使用不会超过 1。

## Qos 模型与 Eviction
