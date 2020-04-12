# QUESTIONS OF SPRING

## SpringMVC的Controller是如何将参数和前端传来的数据一一对应的

>

## Mybatis如何找到指定的Mapper的, 如何完成查询的

>

## Quartz是如何完成定时任务的

>

## 自定义注解的实现

>

## Spring使用了哪些设计模式

>

## Spring的IOC有什么优势

>

## Spring如何维护它拥有的bean

> 简单讲讲tomcat结构以及其类加载流程, 线程模型等

- 模块组成结构:
  - Container
  - Connector

> tomcat如何调优, 涉及哪些参数

主要从四个方面考虑: 吞吐量, ResponseTime, CPU Load, MemoryUsage

```sh
export JAVA_OPTS="-server -Xms1400M -Xmx1400M -Xss512k -XX:+AggressiveOpts -XX:+UseBiasedLocking
-XX:PermSize=128M -XX:MaxPermSize=256M -XX:+DisableExplicitGC -XX:MaxTenuringThreshold=31
-XX:+UseConcMarkSweepGC -XX:+UseParNewGC  -XX:+CMSParallelRemarkEnabled
-XX:+UseCMSCompactAtFullCollection -XX:LargePageSizeInBytes=128m  -XX:+UseFastAccessorMethods
-XX:+UseCMSInitiatingOccupancyOnly
-Djava.awt.headless=true"

```

> 讲讲Spring加载流程

初始化环境 -- 加载配置文件 -- 实例化Bean -- 调用Bean显示信息
> SpringAOP的实现原理

