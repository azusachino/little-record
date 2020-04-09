# NGINX

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

### 默认模块

### Nginx的log

### 访问限制

#### http的请求和连接

#### 请求限制和连接限制

#### access模块配置语法

#### 请求限制局限性

#### 基本安全认证

#### auth模块配置语法

#### 安全认证局限性

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
