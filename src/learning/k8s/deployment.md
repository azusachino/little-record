# Depolyment

Deployment 可以做到很便捷的管理 Pod，只需要在 Deployment 中描述一下希望的 Pod 状态是什么，包括定义 Pod 副本数、滚动升级和回滚应用、扩容和缩容、暂停和继续 Deployment 等，然后 Deployment Controller 就可以帮我们实现我们想要达到的状态。

## Sample

```yml
apiVersion: apps/v1 # 版本
kind: Deployment # 种类
metadata: # 元信息
  name: nginx-deployment
spec:
  selector:
    matchLabels: # 通过app:nignx这个label匹配Pod
      app: nginx
  replicas: 2 # 副本数量
  template: # 定义了这个deployment管理的pod应该是什么样的，具有什么属性
    metadata:
      labels:
        app: nginx
    spec:
      containers:
        - name: nginx
          image: nginx:1.20.0
          ports:
            - containerPort: 80
```

Deployment，是一个定义多副本应用的对象，Deployment 还负责在 Pod 定义发生变化时，对每个副本进行滚动更新。

通过 spec.replicas 字段定义了 Pod 的副本数是 2；

通过 spec.selector 字段定义了被打上 app: nginx 的标签的 Pod 才会被管理；

template 字段定义了这个 Deployment 管理的 Pod 应该是怎样的，具有怎样的属性；

![pic](https://img.luozhiyun.com/20200726182710.png)

## ReplicaSet

ReplicaSet 是一个副本控制器，可以用 selector 来控制 Pod 的数量。Deployments 是一个更高层次的概念，它管理 ReplicaSets，并提供对 Pod 的声明性更新以及许多其他的功能。

Deployment 通过控制 ReplicaSet 的个数来和属性，进而实现“水平扩展 / 收缩”和“滚动更新”这两个编排动作。

### 水平扩展与滚动更新

`kubectl create -f nginx-deployment.yml --record`, `--record`参数作用是记录下每次操作所执行的命令。

通过 kubectl scale 进行水平拓展，`kubectl scale deployment nginx-deployment --replicas=4`  
通过`kubectl rollout status`查看滚动更新的状态，`kubectl rollout status deployment/nginx-deployment`  
通过`kubectl get rs`查看 ReplicaSet 状态  
在运行了一个 Deployment 修改后，Deployment Controller 会创建一个副本为 ReplicaSet，并会生成一串随机字符，ReplicaSet 会把这个随机字符串加在它所控制的所有 Pod 的标签里，从而保证这些 Pod 不会与集群里的其他 Pod 混淆。通过`kubectl get pods --show-labels`进行查看。  
通过`set image`修改 deployment 中的镜像，`kubectl set image deployment/nginx-deployment nginx=nginx:1.16.1 --record`；然后通过`kubectl describe`查看滚动更新的过程，`kubectl describe deployment nginx-deployment`

### 版本控制和回滚

通过`rollout undo`进行版本回滚，`kubectl rollout undo deployment/nginx-deployment`  
通过`kubectl rollout history`来看到相应版本的具体信息，`kubectl rollout history deployment/nginx-deployment --revision=2`  
通过`kubectl rollout undo`命令来回滚到相应的版本：`kubectl rollout undo deployment/nginx-deployment --to-revision=2`

### Pausing & Resuming

可以一次性执行多条命令，然后再一次性滚动更新 => 先 pause Deployment 然后操作完之后再 resume 它。

```bash
kubectl rollout pause deployment/nginx-deployment
# 由于此时 Deployment 正处于“暂停”状态，所以我们对 Deployment 的所有修改，都不会触发新的“滚动更新”，也不会创建新的 ReplicaSet。
kubectl rollout resume deployment/nginx-deployment
# 在 kubectl rollout pause 指令之后的这段时间里，我们对 Deployment 进行的所有修改，最后只会触发一次“滚动更新”。
```
