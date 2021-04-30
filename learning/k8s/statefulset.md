# StatefulSet 控制器

## StatefulSet 概念

用来对有状态应用提供支持的控制器。

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

这个 Service 会通过 Selector 代理所有携带了 app=Nginx 标签的 Pod。其所代理的所有 Pod 的 IP 地址，都会被绑定到下述格式的 DNS 记录上。

```s
<pod-name>.<svc-name>.<namespace>.svc.cluster.local
```
