# K8S Practice

## Pod 中的重要概念

### Pod 的生命周期

1. Pending。这个状态意味着，Pod 的 YAML 文件已经提交给了 Kubernetes，API 对象已经被创建并保存在 Etcd 当中。但是，这个 Pod 里有些容器因为某种原因而不能被顺利创建。比如，调度不成功。
2. Running。这个状态下，Pod 已经调度成功，跟一个具体的节点绑定。它包含的容器都已经创建成功，并且至少有一个正在运行中。
3. Succeeded。这个状态意味着，Pod 里的所有容器都正常运行完毕，并且已经退出了。这种情况在运行一次性任务时最为常见。
4. Failed。这个状态下，Pod 里至少有一个容器以不正常的状态（非 0 的返回码）退出。这个状态的出现，意味着你得想办法 Debug 这个容器的应用，比如查看 Pod 的 Events 和日志。
5. Unknown。这是一个异常状态，意味着 Pod 的状态不能持续地被 kubelet 汇报给 kube-apiserver，这很有可能是主从节点（Master 和 Kubelet）间的通信出现了问题。

还包括一组 Conditions：PodScheduled, Ready, Initialized, Unschedulable.

### Pod 中的资源限制

Kubernetes 通过 cgroups 限制容器的 CPU 和内存等计算资源，包括 requests 和 limits。

```properties
spec.containers[].resources.limits.cpu
spec.containers[].resources.limits.memory
spec.containers[].resources.limits.hugepages-<size>
spec.containers[].resources.requests.cpu
spec.containers[].resources.requests.memory
spec.containers[].resources.requests.hugepages-<size>
```

内存的约束和请求以字节为单位：E, P, T, G, M, K.

### Pod 中的网络资源

Pod 是一个逻辑概念，是一组共享了某些资源的容器。Pod 里的所有容器，共享的是同一个 Network Namespace，并且可以声明共享同一个 Volume。

在 Kubernetes 项目里有一个中间容器，这个容器叫作 Infra 容器。Infra 容器永远都是第一个被创建的容器，而其他用户定义的容器，则通过 Join Network Namespace 的方式，与 Infra 容器关联在一起。

在 Kubernetes 项目里，Infra 容器一定要占用极少的资源，所以它使用的是一个非常特殊的镜像，叫作：k8s.gcr.io/pause。这个镜像是一个用汇编语言编写的、永远处于“暂停”状态的容器，解压后的大小也只有 100~200 KB 左右。

```yml
apiVersion: v1
kind: Pod
metadata:
  name: two-containers
spec:
  restartPolicy: Never
  volumes: # 定义了shared-data，指定宿主机的挂载路径
    - name: shared-data
      hostPath:
        path: /data
  containers:
    - name: nginx-container
      image: nginx
      volumeMounts: # 在Container中的使用方式，指定container内部的挂载路径
        - name: shared-data
          mountPath: /usr/share/nginx/html
    - name: debian-container
      image: debian
      volumeMounts:
        - name: shared-data
          mountPath: /pod-data
      command: ["/bin/sh"]
      args:
        ["-c", "echo Hello from the debian container > /pod-data/index.html"]
```

`kubectl exec -it two-containers --container debian-container -- /bin/bash`

### Pod 中的重要字段

#### NodeSelector: 是一个供用户将 Pod 与 Node 进行绑定的字段

```yml
apiVersion: v1
kind: Pod
---
spec:
  nodeSelector:
    disktype: ssd # pod只能运行在携带了“disktype:ssd”标签(label)的node上
```

#### HostAliases：定义了 Pod 的 hosts 文件

```yml
apiVersion: v1
kind: Pod
---
spec:
  hostAliases:
    - ip: "10.1.2.3"
      hostnames:
        - "foo.remote"
        - "bar.remote"
```

对应的 hosts 文件如下

```plain
cat /etc/hosts
# Kubernetes-managed hosts file.
127.0.0.1 localhost
...
10.244.135.10 hostaliases-pod
10.1.2.3 foo.remote
10.1.2.3 bar.remote
```

#### shareProcessNamespace：表示这个 Pod 的容器共享 PID Namespace

```yml
apiVersion: v1
kind: Pod
metadata:
  name: nginx
spec:
  shareProcessNamespace: true
  containers:
    - name: nginx
      image: nginx
    - name: shell
      image: busybox
      stdin: true
      tty: true
```

整个 Pod 里的每个容器的进程，对于所有容器来说都是可见的。

#### ImagePullPolicy：定义了拉取镜像的策略

default: IfNotPresent, 当容器的镜像是类似于 nginx 或者 nginx:latest 这样的名字时，ImagePullPolicy 会被认为 Always ，即总是会去拉取最新的镜像。

#### LifeCycle：可以在容器状态发生变化时触发一系列生命周期钩子

```yml
apiVersion: v1
kind: Pod
metadata:
  name: lifecycle-demo
spec:
  containers:
    - name: lifecycle-demo-container
      image: nginx
      lifecycle:
        postStart:
          exec:
            command:
              [
                "/bin/sh",
                "-c",
                "echo Hello from the postStart handler > /usr/share/message",
              ]
        preStop:
          exec:
            command: ["/usr/sbin/nginx", "-s", "quit"]
```

### Init Containers

一个 Pod 中可以有多个 container，所以它也能有一个或多个 init container，init container 能在其他 container 开始前运行。每个 init container 都会运行完成后下一个 container 才能开始。

如果 Pod 中的 init container 运行失败，Kubernetes 会重启 Pod 直到 init container 运行成功；如果这个 Pod 的 restartPolicy 策略是 Never，那么 Kubernetes 在 init container 运行失败后将不会启动。

### Projected Volume

用来为容器提供预先定义好的数据

- Secret
- ConfigMap
- Downward API
- ServiceAccountToken

#### Secret：把 Pod 想要访问的加密数据存放到 etcd 中

```plain
$ cat ./username.txt
admin
$ cat ./password.txt
c1oudc0w!

$ kubectl create secret generic user --from-file=./username.txt
$ kubectl create secret generic pass --from-file=./password.txt

kubectl get secrets
```

```yml
# create secrets with yml
apiVersion: v1
kind: Secret
metadata:
  name: mysecret
type: Opaque
data:
  user: YWRtaW4=
  pass: MWYyZDFlMmU2N2Rm
```

```yml
apiVersion: v1
kind: Pod
metadata:
  name: test-projected-volume
spec:
  containers:
    - name: test-secret-volume
      image: busybox
      args:
        - sleep
        - "86400"
      volumeMounts:
        - name: mysql-cred
          mountPath: "/projected-volume"
          readOnly: true
  volumes:
    - name: mysql-cred
      projected:
        sources:
          - secret:
              name: user
          - secret:
              name: pass
```

#### ConfigMap：保存不需要加密的、应用所需的配置信息

```plain
# .properties文件的内容
$ cat example/ui.properties
color.good=purple
color.bad=yellow
allow.textmode=true
how.nice.to.look=fairlyNice

# 从.properties文件创建ConfigMap
$ kubectl create configmap ui-config --from-file=example/ui.properties

# 查看这个ConfigMap里保存的信息(data)
$ kubectl get configmaps ui-config -o yaml
apiVersion: v1
data:
  ui.properties: |
    color.good=purple
    color.bad=yellow
    allow.textmode=true
    how.nice.to.look=fairlyNice
kind: ConfigMap
metadata:
  name: ui-config
  ...
```

#### Downward API：可以让 Pod 里的容器能够直接获取到这个 PodAPI 对象本身的信息

```yml
apiVersion: v1
kind: Pod
metadata:
  name: test-downwardapi-volume
  labels:
    zone: us-est-coast
    cluster: test-cluster1
    rack: rack-22
spec:
  containers:
    - name: client-container
      image: k8s.gcr.io/busybox
      command: ["sh", "-c"]
      args:
        - while true; do
          if [[ -e /etc/podinfo/labels ]]; then
          echo -en '\n\n'; cat /etc/podinfo/labels; fi;
          sleep 5;
          done;
      volumeMounts:
        - name: podinfo
          mountPath: /etc/podinfo
          readOnly: false
  volumes:
    - name: podinfo
      projected:
        sources:
          - downwardAPI:
              items:
                - path: "labels"
                  fieldRef:
                    fieldPath: metadata.labels
```

Downward API 支持的字段

```plain
1. 使用fieldRef可以声明使用:
spec.nodeName - 宿主机名字
status.hostIP - 宿主机IP
metadata.name - Pod的名字
metadata.namespace - Pod的Namespace
status.podIP - Pod的IP
spec.serviceAccountName - Pod的Service Account的名字
metadata.uid - Pod的UID
metadata.labels['<KEY>'] - 指定<KEY>的Label值
metadata.annotations['<KEY>'] - 指定<KEY>的Annotation值
metadata.labels - Pod的所有Label
metadata.annotations - Pod的所有Annotation

2. 使用resourceFieldRef可以声明使用:
容器的CPU limit
容器的CPU request
容器的memory limit
容器的memory request
```

#### Service Account：K8S 系统内置的一种“服务账户”，可以进行权限分配

Service Account 的授权信息和文件保存在它所绑定的一个特殊的 Secret 对象 ServiceAccountToken 中。

任意一个运行在 Kubernetes 集群里的每一个 Pod，都已经自动声明一个类型是 Secret：名为 default-token-xxxx 的 Volume，然后自动挂载在每个容器的一个固定目录上。

```bash
$ kubectl describe pod nginx-deployment-5c678cfb6d-lg9lw
Containers:
...
  Mounts:
    /var/run/secrets/kubernetes.io/serviceaccount from default-token-s8rbq (ro)
Volumes: # 默认ServiceAccount对应的ServiceAccountToken
  default-token-s8rbq:
  Type:       Secret (a volume populated by a Secret)
  SecretName:  default-token-s8rbq
  Optional:    false
```

### Pod 的健康检查和恢复机制

Pod 中的健康检查是 kubectl 在对 Container 的一个定期的检查。主要有三种实现方式：

- 命令执行 ExecAction：在 Container 中会定期的执行一行命令，如果这个命令能执行成功，那么就认为是健康的；
- TCP Socket Action：对 Container 中的 ip 和端口进行 TCP 检查，如果端口是通的，那么就认为是健康的；
- HTTP GET Action：通过 http 请求的方式来检查 Container 中的服务是否正常。

```yml
apiVersion: v1
kind: Pod
metadata:
  labels:
    test: liveness
  name: test-liveness-exec
spec:
  containers:
    - name: liveness
      image: busybox
      args:
        - /bin/sh
        - -c
        - touch /tmp/healthy; sleep 30; rm -rf /tmp/healthy; sleep 600 # 创建文件，30s后删除
      livenessProbe: # 是否存活探针
        exec:
          command: # 执行命令
            - cat
            - /tmp/healthy
        initialDelaySeconds: 5
        periodSeconds: 5 # 执行周期
```

Pod 的恢复机制：

- Always：在任何情况下，只要容器不在运行状态，就自动重启容器；
- OnFailure: 只在容器 异常时才自动重启容器；
- Never: 从来不重启容器

只要 Pod 的 restartPolicy 指定的策略允许重启异常的容器（比如：Always），那么这个 Pod 就会保持 Running 状态，并进行容器重启。

对于包含多个容器的 Pod，只有它里面所有的容器都进入异常状态后，Pod 才会进入 Failed 状态。

### Pod 预设置：PodPreset

```yml
apiVersion: settings.k8s.io/v1alpha1
kind: PodPreset
metadata:
  name: allow-database
spec:
  selector:
    matchLabels:
      role: frontend
  env:
    - name: DB_PORT
      value: "6379"
  volumeMounts:
    - mountPath: /cache
      name: cache-volume
  volumes:
    - name: cache-volume
      emptyDir: {}
```

在这个 PodPreset 的定义中，首先是一个 selector，表示只会作用于 selector 所定义的、带有“role: frontend”标签的 Pod 对象。

```yml
apiVersion: v1
kind: Pod
metadata:
  name: website
  labels:
    app: website
    role: frontend # 根据role: frontend匹配PodPreset
spec:
  containers:
    - name: website
      image: nginx
      ports:
        - containerPort: 80
```

PodPreset 里定义的内容，只会在 Pod API 对象被创建之前追加在这个对象本身上，而不会影响任何 Pod 的控制器的定义。

## 总结

1. 与 k8s 进行交互尽量选择 yaml 文件交互；
2. 可以使用 kubectl create 命令创建一个 pod；
3. 想要获取目前 pod 的状态可以使用 kubectl get pods 命令；
4. 使用 kubectl describe pod 可以查看某个 pod 的详细信息；
5. 如果想要对 pod 更新最好使用 kubectl apply -f ，这样可以做到更加无感的创建 pod 或更新；
6. 可以使用 Volumes 来挂载卷；
7. 使用 kubectl delete -f 可以删除一个 pod；
