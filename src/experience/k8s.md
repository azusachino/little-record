# K8S EXP

## Install MiniKube on 1CPU machine

- download the latest release: `curl -Lo minikube https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64`
- start with force cpu 1 & aliyun proxy: `minikube start --driver=none --extra-config=kubeadm.ignore-preflight-errors=NumCPU --force --cpus 1 --image-repository=registry.cn-hangzhou.aliyuncs.com/google_containers`

## PV PVC clean order

1. 确认 deployment 是否已挂载该 pvc,如果挂载了的话需要删除该 pvc，然后重新部署该 deployment。
2. 重新部署的 deploymeny 为 running 状态后，此时可以删除该 pvc
3. pvc 删除后，可以删除 pv

## 创建 NFS 服务端

### 关闭服务器

`systemctl stop firewalld && systemctl disable firewalld`

### 安装 nfs-utils 和 rpcbind

`yum install -y nfs-util rpcbind`

### 创建数据存储文件夹

```sh
mkdir /nfs
chown -R nfsnobody:nfsnobody /nfs
```

### 配置 NFS 服务端

指定共享文件夹目录以及能够使用共享文件夹的 IP 段

```sh
vi /etc/exports

/nfs 192.168.2.0/24(rw,async,no_root_squash)
# 所有ip生效
/nfsall *(rw,async,no_root_squash)
```

### 启动 NFS 服务端

```sh
systemctl restart rpcbind
systemctl enable nfs && systemctl restart nfs
```

### 创建 ServiceAccount

```yml
apiVersion: v1
kind: ServiceAccount
metadata:
  name: nfs-client-provisioner
  namespace: kube-system
---
kind: ClusterRole
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  name: nfs-client-provisioner-runner
rules:
  - apiGroups: [""]
    resources: ["persistentvolumes"]
    verbs: ["get", "list", "watch", "create", "delete"]
  - apiGroups: [""]
    resources: ["persistentvolumeclaims"]
    verbs: ["get", "list", "watch", "update"]
  - apiGroups: ["storage.k8s.io"]
    resources: ["storageclasses"]
    verbs: ["get", "list", "watch"]
  - apiGroups: [""]
    resources: ["events"]
    verbs: ["create", "update", "patch"]
---
kind: ClusterRoleBinding
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  name: run-nfs-client-provisioner
subjects:
  - kind: ServiceAccount
    name: nfs-client-provisioner
    namespace: kube-system
roleRef:
  kind: ClusterRole
  name: nfs-client-provisioner-runner
  apiGroup: rbac.authorization.k8s.io
---
kind: Role
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  name: leader-locking-nfs-client-provisioner
  namespace: kube-system
rules:
  - apiGroups: [""]
    resources: ["endpoints"]
    verbs: ["get", "list", "watch", "create", "update", "patch"]
---
kind: RoleBinding
apiVersion: rbac.authorization.k8s.io/v1
metadata:
  name: leader-locking-nfs-client-provisioner
  namespace: kube-system
subjects:
  - kind: ServiceAccount
    name: nfs-client-provisioner
    namespace: kube-system
roleRef:
  kind: Role
  name: leader-locking-nfs-client-provisioner
  apiGroup: rbac.authorization.k8s.io
```

### 部署 NFS-Subdir-External-Provisioner

```yml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nfs-client-provisioner
  labels:
    app: nfs-client-provisioner
spec:
  replicas: 1
  strategy:
    type: Recreate ## 设置升级策略为删除再创建(默认为滚动更新)
  selector:
    matchLabels:
      app: nfs-client-provisioner
  template:
    metadata:
      labels:
        app: nfs-client-provisioner
    spec:
      serviceAccountName: nfs-client-provisioner
      containers:
        - name: nfs-client-provisioner
          #image: gcr.io/k8s-staging-sig-storage/nfs-subdir-external-provisioner:v4.0.0
          image: registry.cn-beijing.aliyuncs.com/mydlq/nfs-subdir-external-provisioner:v4.0.0
          volumeMounts:
            - name: nfs-client-root
              mountPath: /persistentvolumes
          env:
            - name: PROVISIONER_NAME ## Provisioner的名称,以后设置的storageclass要和这个保持一致
              value: nfs-client
            - name: NFS_SERVER ## NFS服务器地址,需和valumes参数中配置的保持一致
              value: 192.168.2.11
            - name: NFS_PATH ## NFS服务器数据存储目录,需和valumes参数中配置的保持一致
              value: /nfs/data
      volumes:
        - name: nfs-client-root
          nfs:
            server: 192.168.2.11 ## NFS服务器地址
            path: /nfs/data ## NFS服务器数据存储目录
```

### 创建 NFS StorageClass

```yml
apiVersion: storage.k8s.io/v1
kind: StorageClass
metadata:
  name: nfs-storage
  annotations:
    storageclass.kubernetes.io/is-default-class: "false" ## 是否设置为默认的storageclass
provisioner: nfs-client ## 动态卷分配者名称，必须和上面创建的"provisioner"变量中设置的Name一致
parameters:
  archiveOnDelete: "true" ## 设置为"false"时删除PVC不会保留数据,"true"则保留数据
mountOptions:
  - hard ## 指定为硬挂载方式
  - nfsvers=4 ## 指定NFS版本,这个需要根据NFS Server版本号设置
```

### 测试应用

```yml
kind: PersistentVolumeClaim
apiVersion: v1
metadata:
  name: test-pvc
spec:
  storageClassName: nfs-storage ## 需要与上面创建的storageclass的名称一致
  accessModes:
    - ReadWriteOnce
  resources:
    requests:
      storage: 1Mi
---
kind: Pod
apiVersion: v1
metadata:
  name: test-pod
spec:
  containers:
    - name: test-pod
      image: busybox:latest
      command:
        - "/bin/sh"
      args:
        - "-c"
        - "touch /mnt/SUCCESS && exit 0 || exit 1" ## 创建一个名称为"SUCCESS"的文件
      volumeMounts:
        - name: nfs-pvc
          mountPath: "/mnt"
  restartPolicy: "Never"
  volumes:
    - name: nfs-pvc
      persistentVolumeClaim:
        claimName: test-pvc
```
