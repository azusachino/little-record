# NGINX

一个开源且高性能, 可靠的HTTP中间件, 代理服务

1. IO多路复用epoll
2. 轻量级
3. CPU亲和(Affinity)
4. sendfile

## 基础篇

### 快速安装

1.环境调适确认

1. 确认系统网络
2. 确认yum可用
3. 确认关闭iptables规则
4. 确认停用selinux
5. `yum -y install gcc gcc-c++ autoconf pcre pcre-devel make automake`
6. `yum -y install wget httpd-tools vim`
7. cd /opts; mkdir app download logs work backup

### 配置语法

```conf
user  nobody; # 设置nginx服务的系统使用用户
worker_processes  1; # 工作进程数

error_log  logs/error.log; # 错误日志
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

pid        logs/nginx.pid; nginx服务启动时候的日志

events {
    worker_connections  1024; # 每个进程允许的最大连接数
    use 10; # 工作进程数
}


http {
    include       mime.types; # accept mime types
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;
    limit_conn_zone $binary_remote_addr zone=conn_zone:1m;
    limit_req_zone $binary_remote_addr zone=req_zone:1m rate:1r/s;

    #gzip  on;
    # can config multiple server
    server {
        listen       80; # 监听端口
        server_name  localhost; # 服务名

        #charset koi8-r; # charset

        #access_log  logs/host.access.log  main;

        location / {
            root   html;
            index  index.html index.htm;
            #limit_conn conn_zone 1;
            #limit_req zone=req_zone burst=3 nodelay;
        }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }
}
```

### 默认模块

--with-http_stub_status_module Nginx的客户端状态

--with-http_random_index_module 目录中选择一个随机主页

random_index on | off

--with-http_sub_module HTTP内容替换

### Nginx的log

- error.log access_log

### 访问限制

#### http的请求和连接

- 链接频率限制 - limit_conn_module
- 请求频率限制 - limit_req_module
- 基于IP的访问控制 - http_access_module
- 基于用户的信任登录 - http_auth_basic_module

#### 请求限制和连接限制

- limit_conn_zone $binary_remote_addr zone=conn_zone:1m;
- limit_req_zone $binary_remote_addr zone=req_zone:1m rate:1r/s;

#### access模块配置语法

```conf
location ~ ^/admin {
    root /opt/app/code;
    deny 192.168.1.1;
    allow all;
    index index.html index.htm;
}
```

http_x_forwarded_for = ClientIP, Proxy(1)IP, Proxy(2)IP ...

#### 请求限制局限性

- 采用别的HTTP头信息控制访问
- 结合GEO模块
- 通过HTTP自定义变量传递

#### 基本安全认证

```conf
Syntax: auth_basic string | off;
Default: auth_basic off;
Context: server, location, limit_except

Syntax: auth_basic_user_file file;
Default: --
Context: http, server, location, limit_except
```

#### auth模块配置语法

```conf
location ~ ^/admin {
    root /opt/app/code;
    auth_basic "Please authenticate before enter!";
    auth_basic_user_file /etc/nginx/auth_conf; # 包含用户名和密码
    index index.html index.htm;
}
```

#### 安全认证局限性

1. 用户信息依赖文件方式
2. 操作管理机械, 效率低下

---

1. Nginx结合LUA实现高效验证
2. Nginx和LDAP打通, 利用nginx-auth-ldap模块

## 实践篇

### 静态资源web服务

#### 什么是静态资源

#### 静态资源服务场景

#### 静态资源服务配置

#### 客户端缓存

#### 静态资源压缩

#### 防盗链

#### 跨域访问

### 代理服务

### 负载均衡

### 缓存服务

## 深度学习篇

### 动静分离

### rewrite规则

### 进阶模块配置

### https服务

#### https协议

#### https配置语法

#### Nginx的https服务

#### 苹果要求的https服务

### Nginx与LUA开发

## 架构篇

### 常见问题

### Nginx中间件性能优化

#### 如何调适性能优化

#### 性能优化影响因素

#### 操作系统性能优化

#### Nginx性能优化

### Nginx与安全

### 新版本特性

### 中间件架构设计
