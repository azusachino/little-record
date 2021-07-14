# 守护进程 DaemonSet

## Daemon Pod 的三个主要特征

- 这个 Pod 运行在 Kubernetes 集群里的每一个节点（Node）上；
- 每个节点上只有一个这样的 Pod 实例；
- 当有新的节点加入 Kubernetes 集群后，该 Pod 会自动地在新节点上被创建出来；而当旧节点被删除后，它上面的 Pod 也相应地会被回收掉。

Daemon Pod 可以运用在网络插件的 Agent 组件上、日志组件、监控组件等。

## 创建

```yml
apiVersion: apps/v1
kind: DaemonSet
metadata:
  name: fluentd-elasticsearch
  namespace: kube-system # k8s默认的命名空间
  labels:
    k8s-app: fluentd-logging
spec:
  selector:
    matchLabels:
      name: fluentd-elasticsearch
  template:
    metadata:
      labels:
        name: fluentd-elasticsearch
    spec:
      tolerations:
        # 在默认情况下，Kubernetes 集群不允许用户在 Master 节点部署 Pod。因为，Master 节点默认携带了一个叫作node-role.kubernetes.io/master的“污点”taint。所以，为了能在 Master 节点上部署 DaemonSet 的 Pod，我就必须让这个 Pod“容忍”这个“污点”。
        - key: node-role.kubernetes.io/master
          effect: NoSchedule
      containers:
        - name: fluentd-elasticsearch
          image: mirrorgooglecontainers/fluentd-elasticsearch:v2.4.0
          resources:
            limits:
              memory: 200Mi
            requests:
              cpu: 100m
              memory: 200Mi
          volumeMounts:
            - name: varlog
              mountPath: /var/log
            - name: varlibdockercontainers
              mountPath: /var/lib/docker/containers
              readOnly: true
      terminationGracePeriodSeconds: 30
      volumes:
        - name: varlog
          hostPath:
            path: /var/log
        - name: varlibdockercontainers
          hostPath:
            path: /var/lib/docker/containers
```

```sh
# 查看Pod运行情况
kubectl get pod -n kube-system -l name=fluentd-elasticsearch
# 查看DaemonSet对象信息
kubectl get ds -n kube-system fluentd-elasticsearch
```

## 仅在某些节点上运行 Pod

- nodeAffinity(亲和性)
- nodeTaint(污点)

### affinity

```yml
apiVersion: v1
kind: Pod
metadata:
  name: with-node-affinity
spec:
  affinity:
    nodeAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
        nodeSelectorTerms:
          - matchExpressions:
              - key: metadata.name
                operator: In
                values:
                  - node
```

上面的这个 pod，我们指定了 nodeAffinity，matchExpressions 的含义是这个 pod 只能运行在 metadata.name 是 node 的节点上，operator=In 表示部分匹配的意思，除此之外 operator 还可以指定：In，NotIn，Exists，DoesNotExist，Gt，Lt 等。

requiredDuringSchedulingIgnoredDuringExecution 表明将 pod 调度到一个节点必须要满足的规则。除了这个规则还有 preferredDuringSchedulingIgnoredDuringExecution 将 pod 调度到一个节点可能不会满足规则

### Taints & Tolerations

在 k8s 集群中，我们可以给 Node 打上污点，这样可以让 pod 避开那些不合适的 node。在 node 上设置一个或多个 Taint 后，除非 pod 明确声明能够容忍这些污点，否则无法在这些 node 上运行。

```sh
# 给node增加污点
kubectl taint nodes node key=value:NoSchedule
# 移除污点
kubectl taint nodes node key:NoSchedule-
```

通过`kubectl taint nodes node1 key=value:NoSchedule`给 node1 打上了污点，将阻止 pod 调度到 node1 上；通过`kubectl taint nodes node1 key:NoSchedule-`移出污点。

如果想让 pod 运行在有污点的 node 节点上，需要在 pod 上声明 Toleration，表明可以容忍具有该 Taint 的 Node。

```yml
apiVersion: v1
kind: Pod
metadata:
  name: pod-taints
spec:
  tolerations:
    - key: "key"
      operator: "Equal"
      value: "value"
      effect: "NoSchedule"
  containers:
    - name: pod-taints
      image: busybox:latest
```
