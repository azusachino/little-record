# Linux 相关经验

## Ubuntu 更新 APT 源

```sh
cd /etc/apt/
cp sources.list sources.list.bak
true > sources.list
vi /etc/apt/sources.list # 将上面的源复制到文件中, 保存退出
apt update # 更新本地 apt 源
```

国内的源地址

```properties
# 1. 阿里源
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
# 2. 中科大源
deb https://mirrors.ustc.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb-src https://mirrors.ustc.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb-src https://mirrors.ustc.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb-src https://mirrors.ustc.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb-src https://mirrors.ustc.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb https://mirrors.ustc.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src https://mirrors.ustc.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
# 3. 163源
deb http://mirrors.163.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ bionic-backports main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ bionic main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ bionic-security main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ bionic-updates main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ bionic-backports main restricted universe multiverse
# 4. 清华源
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-proposed main restricted universe multiverse
```

## Centos 安装 Docker

### 1. 删除旧版本

```bash
sudo yum remove docker \
                docker-client \
                docker-client-latest \
                docker-common \
                docker-latest \
                docker-latest-logrotate \
                docker-logrotate \
                docker-engine
```

### 2. 配置 repo

```bash
sudo yum install -y yum-utils

sudo yum-config-manager \
    --add-repo \
    https://download.docker.com/linux/centos/docker-ce.repo
```

### 3. 安装最新版 docker

`sudo yum install docker-ce docker-ce-cli containerd.io`

## Centos 安装 Docker-Compose

### 1. 下载

`sudo curl -L "https://github.com/docker/compose/releases/download/1.28.0/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose`

### 2. 赋予执行权限

`sudo chmod +x /usr/local/bin/docker-compose`

## sh 文件无法执行

/bin/bash^M: bad interpreter: 没有那个文件或目录  
问题在于，文件可能在 windows 环境下编辑过，产生了不兼容问题，解决方案：

```sh
vim demo.sh
:set ff # 查看是否dos，如果是dos执行后续操作
:set fileformat=unix
wq
```

## Shell 变量 字符串截取

```shell
val='Hello/World/Shell/Script'

# #号截取，删除左边字符，保留右边字符。(仅匹配一次)
echo ${val#*/} # => World/Shell/Script
# */匹配 任意字符 + /

# ##号截取，删除左边字符，保留右边字符。(匹配所有)
echo ${val##*/} # => Script

# %号截取，删除右边字符，保留左边字符。(仅匹配一次)
echo ${val%/*} # => Hello/World/Shell

# %%截取，删除右边字符，保留左边字符。(匹配所有)
echo ${val%%/*} # => Hello

# 从左边第几个字符开始，以及字符的个数
echo ${val:0:5} # => Hello

# 从左边的第几字符开始，一直到结束
echo ${val:6} # => World/Shell/Script

# 从右边第几个字符开始，以及字符的个数
echo ${val:0-8:4} # => l/Sc

# 从右边的第几个字符开始，一直到结束
echo ${val:0-6} # => Script
```

## 内存占用较高

### 查看内存情况

```sh
# 按MB查看
free -m
```

- total:总计物理内存的大小。
- used:已使用多大。
- free:可用有多少。
- Shared:多个进程共享的内存总额。
- Buffers/cached:磁盘缓存的大小。

```sh
# 内存使用情况
cat /proc/meminfo
pidstat -r -p 12345 1 5
ps aux | head -1; ps aux | grep -v PID | sort -rn -k +4 | head -20
slatop
```

### Cached 占用过高

buffer,cached 的作用：

- cached 主要负责缓存文件使用, 日志文件过大造成 cached 区内存增大把内存占用完
- Free 中的 buffer 和 cache：（它们都是占用内存）：
- buffer : 作为 buffer cache 的内存，是块设备(磁盘)的缓冲区，包括读、写磁盘
- cache: 作为 page cache 的内存, 文件系统的 cache，包括读、写文件，如果 cache 的值很大，说明 cache 住的文件数很多。

```sh
#常用方法是

sync

echo 1 > /proc/sys/vm/drop_caches

#清除后要还原系统默认配置：
echo 0 > /proc/sys/vm/drop_caches

#查看设置
sysctl -a | grep drop_caches
补充： echo 字符串 > 文件 就是把字符串内容从定向到文件中
```

## 配置 SSH 链接(centos)

1. 在当前机器生成 ssh key
2. 将公钥拷贝到目标机器的`~/.ssh/authorized_keys`中
3. 修改`/etc/ssh/sshd_config`后保存
4. 重启 sshd 服务`systemctl restart sshd.service`

```sh
# 主要为了保证下面两行启用
RSAAuthentication yes
PubkeyAuthentication yes
```

## 在 shell 脚本中读取.env

```sh
eval "$(echo $(cat .env))"
```

## 服务器被暴力访问

```sh
# 查看密码登录失败的ip
grep "Failed password for invalid user" /var/log/secure | awk '{print $13}' | sort | uniq -c | sort -nr

# 设置禁止访问
cat /var/log/secure |  grep "Failed password for invalid user" | awk '{print $13}' | sort | uniq -c | sort -n | tail -10 |awk '{print "sshd:"$2":deny"}' >> /etc/hosts.allow
```

### 修改 sshd_config，禁止密码访问

```sh
vim /etc/ssh/sshd_config
# ==> PasswordAuthentication no
systemctl restart sshd.service
```

## 查看登录 session

```sh
# 查看当前session
who am i
# 查看全部session
w

# 清退某个session
pkill -kill -t pts/0
```

## 卸载程序

```sh
# 查看所有已安装程序
rpm -qa

# 卸载程序
rpm -e xxx.xxx

# 解决相互依赖问题
rpm -e xxx.xxx --nodeps
```
