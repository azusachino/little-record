# 学习Docker&Kubernetes&DevOps

## docker架构和底层技术

- docker提供了一个开发, 打包, 运行app的平台
- 把app和底层infrastructure隔离开来

### docker engine

- dockerd-后台进程
- RestAPI Server
- CLI接口

### 底层技术支持

1. Namespaces: 做隔离pid, net, ipc, mnt, uts
2. Control groups: 做资源限制
3. Union file systems: Container和image的分层

### image

- 文件和metadata的集合(root filesystem)
- 分层的, 并且每一层都可以添加改变删除文件, 成为一个新的image
- 不同的image可以共享相同的layer
- Image本身是readonly的

#### image的获取

> build from Dockerfile

```Dockerfile
FROM ubuntu:14.04
LABEL maintainer= 'az'
RUN apt-get update & apt-get install -y redis-server
EXPOSE 6379
ENTRYPOINT ["/usr/bin/redis-server"]
```

> pull from registry

```bash
docker pull ubuntu:14.05
```

#### 制作一个image

```bash
touch hello.c
vim hello.c
#include<stdio.h>

int main(){
    printf("hello docker\n");
}
yum install gcc glibc-static
gcc -static hello.c -o hello => 编译成可执行文件
vim Dockerfile
`
FROM scratch
ADD hello /
CMD ["/hello"]
`
docker build -t azusachino/hello .

docker run azusachino/hello
```

> docker history [id] 查看image的layer

## Container

- 通过image创建(copy)
- 在Image Layer之上建立一个container layer(可读写)
- 类比面向对象: 类和实例
- Image负责app的存储和分发, Container负责运行app

```shell
docker run `image`
docker run -it ->  以交互模式打开

docker ps -aq == docker ps -a | awk {'print$1'} => 打印出第一列(id)
docker rm $(docker ps -aq)
docker ps -f "status=exited" => 列举出已退出的容器
```

### docker commit

```bash
docker commit [container] [repo[:tag]] [flags]
docker commit some azusachino/
```

---

```Dockerfile
FROM centos
RUN yum install -y vim => 在临时容器中执行CMD
```

## Dockerfile

### 语法

1. FROM 制作base image
2. LABEL 定义一些变量 METADATA(maintainer, version, description)
3. RUN 运行cmd
4. WORKDIR /root 工作目录
5. ADD hello / 添加xx到指定目录(ADD还可以解压缩)
6. COPY hello test/ 复制到指定目录(优先于ADD)
7. ENV MYSQL_VERSION 5.6 设置常量(增加可维护性)
8. VOLUME 存储
9. EXPOSE 暴露端口
10. CMD
11. ENTRYPOINT

```Dockerfile
RUN yum update && yum install -y yum \ python-dev
RUN apt-get update && apt-get install -y perl \
    pwgen --no-install-recommands && rm -rf \
    /var/lib/apt/lists/* # clean cache
RUN /bin/bash -c 'source $HOME/.bashrc; echo $HOME'

ENV MYSQL_VERSION 5.6
RUN apt-get install -y mysql-server="{MYSQL_VERSION}" \
    && rm -rf /var/lib/apt/lists/*
```

### RUN, CMD, ENTRYPOINT

- RUN: 执行命令并创建新的Image Layer
- CMD: 设置容器启动后默认执行的命令和参数
- ENTRYPOINT: 设置容器启动时运行的命令

```Dockerfile
# Shell格式
RUN apt-get install -y yum
CMD echo "hello docker"
ENTRYPOINT echo "hello docker"

# Exec格式
RUN ["apt-get", "install", "-y", "vim"]
CMD ["/bin/echo", "hello docker"]
ENTRYPOINT ["/bin/echo", "hello docker"] => 无法替换常量(ENV)
ENTRYPOINT ["/bin/bash", "-c", "echo hello $name"]
```

#### CMD

1. 容器启动时默认执行的命令
2. 如果docker run制定了其他命令, CMD命令会被忽略
3. 如果定义了多个CMD, 只有最后一个会执行

#### ENTRYPOINT

1. 让容器以应用程序或者服务的形式运行
2. 不会被忽略, 一定被执行
3. 最佳实践: 写一个shell脚本作为entrypoint

```Dockerfile
COPY docker-entrypoint.sh /usr/local/bin
ENTRYPOINT ["docker-entrypoint.sh"]

EXPOSE 27017
CMD ["mongod"]
```

docker push azusachino/hello:latest => 推送到docker hub(需要登陆)

