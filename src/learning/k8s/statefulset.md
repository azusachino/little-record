# StatefulSet 控制器

## StatefulSet 概念

Deployment 对应无状态的应用，StatefulSet 用来对有状态应用提供支持的控制器。

StatefulSet 创建的 pod 具有唯一的标识和创建和删除顺序的保障：

1. 提供稳定的网络标识。一个 StatefulSet 创建的每个 pod 都有一个从零开始的顺序索引。这样可以方便通过主机名来定位 pod，例如我们可以创建一个 headless Service，通过 Service 记录每个 pod 的独立 DNS 记录来定位到不同的 pod，由于 pod 主机名固定，所以 DNS 记录也不会变。
2. 提供稳定的专属存储。一个 StatefulSet 在创建的时候也可以声明需要一个或多个 PVC，然后 pvc 会在创建 pod 前绑定到 pod 上。StatefulSet 在缩容的时候依然会保留 pvc，这样不会导致数据的丢失，在扩容的时候也可以让 pvc 挂载到相同的 pod 上。

### 稳定的网络标识

在 k8s 中，Service 是用来将一组 Pod 暴露给外界访问的一种机制。Service 可以通过 DNS 的方式，代理到某一个 Pod，然后通过 DNS 记录的方式解析出被代理 Pod 的 IP 地址。

```yml
apiVersion: v1
kind: Service
metadata:
  name: nginx
  labels:
    app: nginx
spec:
  ports:
    - port: 80
      name: web
  clusterIP: None
  selector:
    app: nginx
```

这个 Service 会通过 Selector 代理所有携带了 app=nginx 标签的 Pod。其所代理的所有 Pod 的 IP 地址，都会被绑定到下述格式的 DNS 记录上。

```s
<pod-name>.<svc-name>.<namespace>.svc.cluster.local
```

通过这个 DNS 记录，StatefulSet 可以使用 DNS 记录来维持 Pod 的网络状态。

```yml
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: web
spec:
  serviceName: "nginx"
  replicas: 2
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
        - name: nginx
          image: nginx:1.20.0
          ports:
            - containerPort: 80
              name: web
          volumeMounts:
            - name: www
              mountPath: /usr/share/nginx/html
  volumeClaimTemplates:
    - metadata:
        name: www
      spec:
        accessModes: ["ReadWriteOnce"]
        resources:
          requests:
            storage: 1Gi
```

```sh
kubectl create -f nginx-service.yml
kubectl get service nginx
kubectl create -f nginx-ss.yml
kubectl get statefulset web

kubectl get pods -w -l app=nginx
kubectl exec web-0 -- sh -c 'hostname'
```

可以启动一个一次性的 pod 用 nslookup 命令，解析 Pod 对应的 Headless Service.

```sh
kubectl run -i --tty --image busybox:latest dns-test --restart=Never --rm /bin/sh
nslookup web-0.nginx
```

如果删除这两个 Pod，Kubernetes 会按照原先编号的顺序，创建两个新的 Pod，并分配原来的 DNS: web-0.nginx, web-1.nginx. `kubectl delete pod -l app=nginx`

### 提供稳定的专属存储

```yml
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: web
spec:
  serviceName: "nginx"
  replicas: 1
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
        - name: nginx
          image: nginx:1.20.0
          ports:
            - containerPort: 80
              name: web
          volumeMounts:
            - name: local-volume-a
              mountPath: /usr/share/nginx/html
  volumeClaimTemplates:
    - metadata:
        name: local-volume-a
      spec:
        accessModes:
          - ReadWriteMany
        storageClassName: "local-volume"
        resources:
          requests:
            storage: 512Mi
        selector:
          matchLabels:
            key: local-volume-a-0
```

通过 volumeClaimTemplates 字段声明对应的 PVC 的定义，规定这个 PVC 中使用的 storageClass 必须是 local-volume，需要的存储空间是 512Mi，并且这个 pvc 对应的 pv 的标签必须是 key: local-volume-a-0。

```yml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: local-volume-pv-0
  labels:
    key: local-volume-a-0
spec:
  capacity:
    storage: 0.5Gi
  volumeMode: Filesystem
  accessModes:
    - ReadWriteMany
  persistentVolumeReclaimPolicy: Retain
  storageClassName: local-volume
  local:
    path: /mnt/disks/vol
    # path: /mnt/c/Users/ycpang/Desktop
    # 经过试验，C:/Users/az/, C:\Users\az, C:\\Users\\az都不行
  nodeAffinity:
    required:
      nodeSelectorTerms:
        - matchExpressions:
            - key: kubernetes.io/arch
              operator: In
              values:
                - amd64
```

创建 StatefulSet 的时候会自动创建 PVC。

#### 缩容与扩容

如果我们把 StatefulSet 进行缩容，那么 StatefulSet 会删除将 pod 的顺序由大到小删除。在删除完相应的 pod 之后，对应的 PVC 并不会被删除，如果需要释放特定的持久卷时，需要手动删除对应的持久卷声明。

如果我们再把 StatefulSet 进行扩容，新创建的 pod 还是会和原来的 PVC 相互绑定，新的 pod 实例会运行到与之前完全一致的状态。

### 更新策略

#### OnDelete

如果 StatefulSet 的 `.spec.updateStrategy.type` 字段被设置为 OnDelete，当您修改 .spec.template 的内容时，StatefulSet Controller 将不会自动更新其 Pod。您必须手工删除 Pod，此时 StatefulSet Controller 在重新创建 Pod 时，使用修改过的 .spec.template 的内容创建新 Pod。

#### Rolling Updates

`.spec.updateStrategy.type` 字段的默认值是 RollingUpdate，该策略为 StatefulSet 实现了 Pod 的自动滚动更新。在更新完.spec.tempalte 字段后 StatefulSet Controller 将自动地删除并重建 StatefulSet 中的每一个 Pod。

- 删除的时候从序号最大的开始删，每删除一个会更新一个。
- 只有更新完的 pod 已经是 ready 状态了才往下继续更新。

---

当为 StatefulSet 的 RollingUpdate 字段的指定 partition 字段的时候，则所有序号大于或等于 partition 值的 Pod 都会更新。序号小于 partition 值的所有 Pod 都不会更新，即使它们被删除，在重新创建时也会使用以前的版本。

如果 partition 值大于其 replicas 数，则更新不会传播到其他 Pod。这样可以实现金丝雀发布 (Canary Deploy) 或者灰度发布。

使用`kubectl patch`以补丁的方式更新 API 对象中的指定字段。

`kubectl patch statefulset web -p '{"spec":{"updateStrategy":{"type":"RollingUpdate","rollingUpdate":{"partition":1}}}}'`

## 源码分析

- stateful_pod_control.go
- stateful_set.go
- stateful_set_control.go

StatefulSetController#sync–>StatefulSetController#syncStatefulSet–>defaultStatefulSetControl#UpdateStatefulSet–>defaultStatefulSetControl#performUpdate–>defaultStatefulSetControl#updateStatefulSet–>realStatefulPodControl 中的各个 pod 操作方法
