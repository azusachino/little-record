# Depolyment

## Sample

```yml
apiVersion: apps/v1 # 版本
kind: Deployment # 种类
metadata: # 元信息
  name: nginx-deployment
spec: 
  selector:
    matchLabels: # 通过app:nignx匹配Pod
      app: nginx
  replicas: 2 # 副本数量
  template: # 定义了这个deployment管理的pod应该是什么样的，具有什么属性
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.7.9
        ports:
        - containerPort: 80
```

通过spec.replicas字段定义了Pod的副本数是2；

通过spec.selector字段定义了被打上app: nginx的标签的Pod才会被管理；

tmplate字段定义了这个Deployment管理的Pod应该是怎样的，具有怎样的属性；

![pic](https://img.luozhiyun.com/20200726182710.png)

## ReplicaSet

ReplicaSet是一个副本控制器，可以用select来控制Pod的数量。Departments是一个更高层次的概念，它管理ReplicaSets，并提供对Pod的声明性更新以及许多其他的功能。

### 水平扩展与滚动更新

`kubectl create -f nginx-deployment.yml --record`, `--record`参数作用是记录下每次操作所执行的命令。

通过kubectl scale进行水平拓展，`kubectl scale deployment nginx-deployment --replicas=4`  
通过`kubectl rollout status`查看滚动更新的状态，`kubectl rollout status deployment/nginx-deployment`

在运行了一个Deployment修改后，Deployment Controller会创建一个副本为ReplicaSet，并会生成一串随机字符，ReplicaSet 会把这个随机字符串加在它所控制的所有 Pod 的标签里，从而保证这些 Pod 不会与集群里的其他 Pod 混淆。通过`kubectl get pods -show-labels`进行查看。

通过`set image`修改deployment中的镜像，`kubectl set image deployment/nginx-deployment nginx=nginx:1.16.1 --record`；然后通过`kubectl describe`查看滚动更新的过程，`kubectl describe deployment nginx-deployment`

### 版本控制和回滚

通过`rollout undo`进行版本回滚，`kubectl rollout undo deployment/nginx-deployment`
通过kubectl rollout history来看到相应版本的具体信息，`kubectl rollout history deployment/nginx-deployment --revision=2`

### Pausing & Resuming

一次性执行多条命令然后再一次性滚动更新，那么可以先pause Deployment然后操作完之后再resume 它。

```bash
kubectl rollout pause deployment/nginx-deployment
# 由于此时 Deployment 正处于“暂停”状态，所以我们对 Deployment 的所有修改，都不会触发新的“滚动更新”，也不会创建新的 ReplicaSet。
kubectl rollout resume deployment/nginx-deployment
# 在 kubectl rollout pause 指令之后的这段时间里，我们对 Deployment 进行的所有修改，最后只会触发一次“滚动更新”。
```
