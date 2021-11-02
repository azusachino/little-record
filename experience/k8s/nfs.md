# NFS-Subdir-External-Provisioner

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

## 创建 ServiceAccount

`nfs-rbac.yml`

## 部署 NFS-Subdir-External-Provisioner

`nfs-provisioner-deploy.yml`

## 创建 NFS StorageClass

`nfs-storageclass.yml`
