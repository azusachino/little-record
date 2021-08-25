# Docker

## 删除 none 的 image

```sh
docker rmi $(docker images | grep "none" | awk '{print $3}')
```

## install docker

```md
# install docker (centos)

sudo yum update (optional)
sudo yum install -y yum-utils
sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
sudo yum install docker-ce docker-ce-cli containerd.io

# install docker-compose

sudo curl -L "https://github.com/docker/compose/releases/download/1.29.1/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
sudo chmod +x /usr/local/bin/docker-compose

# start postgres

docker run --name postgres -e POSTGRES_PASSWORD=azusa520 -p 5432:5432 -d postgres
docker run -itd --name mysql-save -p 3306:3306 -e MYSQL_ROOT_PASSWORD=azusa520 mysql
docker run --rm -it -d --name halo -p 8090:8090 -v ~/.halo:/root/.halo ruibaby/halo
docker run -d -p 15672:15672 -p 5672:5672 -e RABBITMQ_DEFAULT_USER=admin -e RABBITMQ_DEFAULT_PASS=admin --name my-rabbitmq rabbitmq
docker run -d --privileged=true -p 6379:6379 --name iredis -v /home/docker/redis/conf/redis.conf:/etc/redis/redis.conf -v /home/docker/redis/data:/data -d redis redis-server /etc/redis/redis.conf --appendonly yes
docker run -d --name=mygogs -p 10022:22 -p 10080:3000 -v /var/gogs:/data gogs/gogs

sudo yum install java-1.8.0-openjdk
npm install --global --production windows-build-tools

nohup java -jar xx.jar & => 需要追加一个`&`

docker run --name docker_nginx -d -p 80:80 -v /home/nginx/nginx.conf:/etc/nginx/nginx.conf --volume /home/little-idea/dist:/usr/share/nginx/html nginx
```

## docker 映射文件夹删除导致异常

容器启动时会和映射的文件夹进行关联,产生句柄. 文件夹删除之后, 即使再新建相同名称的文件夹, 也已经不是同一个句柄了。故 io 操作会产生异常。
