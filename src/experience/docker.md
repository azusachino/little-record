# Docker Experience

## 清理 docker 无效资源

```sh
# 通过筛选出none，然后docker rmi
docker rmi $(docker images | grep "none" | awk '{print $3}')
# 清理docker占用资源
docker prune [target resource] # eg docker prune image
```

## install docker

Please check docker [docs](https://docs.docker.com/engine/install/centos/)

### install docker (centos)

```sh
sudo yum update (optional)
sudo yum install -y yum-utils
sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
sudo yum install docker-ce docker-ce-cli containerd.io
```

### install docker-compose

```sh
# not always the latest version of docker-compose
sudo curl -L "https://github.com/docker/compose/releases/download/1.29.1/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose
```

### start services

Few samples of docker run command.

```sh
docker run --name postgres -e POSTGRES_PASSWORD=azusa520 -p 5432:5432 -d postgres
docker run -itd --name mysql-save -p 3306:3306 -e MYSQL_ROOT_PASSWORD=azusa520 mysql
docker run --rm -it -d --name halo -p 8090:8090 -v ~/.halo:/root/.halo ruibaby/halo
docker run -d -p 15672:15672 -p 5672:5672 -e RABBITMQ_DEFAULT_USER=admin -e RABBITMQ_DEFAULT_PASS=admin --name my-rabbitmq rabbitmq
docker run -d --privileged=true -p 6379:6379 --name iredis -v /home/docker/redis/conf/redis.conf:/etc/redis/redis.conf -v /home/docker/redis/data:/data -d redis redis-server /etc/redis/redis.conf --appendonly yes
docker run -d --name=mygogs -p 10022:22 -p 10080:3000 -v /var/gogs:/data gogs/gogs
docker run --name docker_nginx -d -p 80:80 -v /home/nginx/nginx.conf:/etc/nginx/nginx.conf --volume /home/little-idea/dist:/usr/share/nginx/html nginx
```

## docker 映射文件夹删除导致异常

容器启动时会和映射的文件夹进行关联，产生句柄。文件夹删除之后, 即使再新建相同名称的文件夹, 也已经不是同一个句柄了。故 io 操作会产生异常。

## dockerd with new runtime

```bash
## make sure docker.service is stopped
dockerd --experimental --add-runtime="youki=$(pwd)/target/x86_64-unknown-linux-gnu/debug/youki"

docker run -it --rm --run-time youki busybox
```

## uninstall docker (centos)

```bash
# clean docker rpm
yum remove docker-ce docker-ce-cli containerd.io

## remove data rpm
unmount /var/run/docker/netns/default

rm -rf /var/run/docker
rm -rf /var/lib/docker
```

## The mechanism of `docker stop`

The main process inside the container will receive `SIGTERM`, and after a grace period, `SIGKILL`. The first signal can be changed with the `STOPSIGNAL` instruction in the container’s Dockerfile, or the `--stop-signal` option to `docker run`.
