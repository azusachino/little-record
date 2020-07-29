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

```Dockerfile
FROM python:2.7
LABEL maintainer = "AzusaChino<azusachino@yahoo.com>"
RUN pip install flask
COPY app.py /app/

WORKDIR /app
EXPOSE 5000
CMD ["python", "app.py"]
```

- docker exec -it 'hash' /bin/bash => 在运行的container中执行命令

```Dockerfile
FROM ubuntu
RUN apt-get update && apt-get install -y stress
ENTRYPOINT ["/usr/bin/stress"]
CMD []
```

> docker run -it az/ubuntu-stress -vm 1 --verbose => 直接利用CMD执行命令

---

- docker run --memory = 200M 限定container的内存暂用
- docker run --cpu-shares=10 --name=test1 xx/xx --cpu 1 => cpuShares 权重

## Docker Network

- 单机
  - Bridge Network
  - Host Network
  - None Network
- 联机
  - Overlay Network

### 网络基础概念

- 基于数据包的通信方式
- IP地址和路由
- 公有IP和私有IP
  - A类 10.0.0.0 -- 10.255.255.255 (10.0.0.0/8)
  - B类 172.16.0.0 -- 172.31.255.255 (172.16.0.0/12)
  - C类 192.168.0.0 -- 192.168.255.255 (192.168.0.0/16)
- 网络地址转换NAT
- ping 检查ip的可达性
- telnet 验证服务的可用性

---

docker network ls

## Docker的持久化存储和数据共享

- 基于本地文件系统的Volume
- 基于plugin的Volume (NAS, AWS)

### Volume的类型

- 受管理的data volume, 由docker后台自动创建
- 绑定挂载的volume, 具体挂载位置可以由用户指定

---

- docker run -d --name mysql_test -e MYSQL_ALLOW_EMPTY_PASSWORD = true mysql
- docker volume ls => 查看volume信息
- docker volume inspect 'vol_id'

```Dockerfile
VOLUME ["/var/lib/mysql"]
```

- docker run -v mysql:/var/lib/mysql

```Dockerfile
FROM nginx:latest
WORKDIR /usr/share/nginx/html
COPY index.html index.html
```

- docker run -d -p 80:80 --name web az/nginx-html
- docker run -d -p 80:80 -v $(pwd):/usr/share/nginx/html --name web az/nginx-html => 将当前目录映射到xx目录
- docker exec -it web /bin/bash

## Docker Composer => 批处理

- docker compose是一个工具
- 通过yml文件定义多容器的docker应用
- 通过一条命令根据yml文件的定义去创建或者管理多个容器

### Services

- 一个service代表一个container, 可以是拉取的, 也可以是本地build的
- service的启动类似docker run, 可以指定network和volume

```yml
services:
  db:
    image: postgres:9.4
    volumes:
      - "db-data:/var/lib/postgresql/data"
    networks:
    - back-tire
```

等价于 `docker run -d --network back-tier -v db-data:/var/lib/postgresql/data postgres:9.4`

```yaml
services:
  worker:
    build: ./worker
    links:
      - db
      - redis
    networks:
      - back-tier
```

完整示例

```yml
version: '3'

services:
  wordpress:
    image: wordpress
    ports:
      - 8080:80
    environment:
      WORDPRESS_DB_HOST: mysql
      WORDPRESS_DB_PASSWORD: root
    networks:
      - my-bridge

  mysql:
    image: mysql
    environment:
      MYSQL_ROOT_PASSWORD: root
      MYSQL_DATABASE: wordpress
    volumes:
      - mysql-data:/var/lib/mysql
    networks:
      - my-bridge

volumes:
  mysql-data:

networks:
  my-bridge:
    driver: bridge

```

### 安装docker-compose

`sudo curl -L "https://github.com/docker/compose/releases/download/1.26.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose`

`sudo chmod +x /usr/local/bin/docker-compose`

### 相关命令

- docker-compose up => create and start containers
- docker-compose -f docker-compose.yml up
- docker-compose stop => stop services
- docker-compose down => stop and remove all related folders
- docker-compose scale web=3 -d => set number of containers to run for a service

## Docker Swarm (容器编排)

raft协议

- Manager
- Worker

### Swarm相关命令

```bash
docker swarm init --advertise-addr=192.168.205.10

docker node ls
```
