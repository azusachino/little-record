# Java 微服务事件

## 1.初体验

### 微服务基本介绍

SpringCloud为开发人员提供了快速构建分布式系统的一些通用模式, 其中包括: 配置管理, 服务发现, 服务短路, 智能路由, 微型网关, 控制总线, 一次性令牌, 全局锁, 领导选举, 分布式会话和集群状态.

- SOA (Service-orientated architecture)
- MAS (Microservice architecture)
- EDA (Event-driven architecture)

### 12 factors

- Codebase
- Dependencies
- Config
- Backing Services
- Build, release, run
- Processes
- Port Binding
- Concurrency
- Disposability
- Dev/Prod parity
- Logs
- Admin processes

### Bootstrap 上下文

- Bootstrap 上下⽂文是 Spring Cloud 新引⼊入的，与传统 Spring 上下⽂文相同，系 ConfigurableApplicationContext 实例例，由 BootstrapApplicationListener 在监听 ApplicationEnvironmentPreparedEvent 时创建。
  - ApplicationEvent / ApplicationListener
- SpringApplication 是 Spring Boot 引导启动类，与 Spring 上下⽂文、事件、监听器器以及环境等组件 关系紧密，其中提供了了控制 Spring Boot 应⽤特征的⾏为方法。
  - SpringApplicationRunListener
- Spring Boot context
  - ⾮非 Web 应⽤用:AnnotationConfigApplicationContext
  - Web 应⽤用:AnnotationConfigEmbeddedWebApplicationContext
- 先于ApplicationContext加载

### Actuator Endpoints

在SpringBoot使用场景中表示“为生成而准备的特性”, 通过http端口或JMX的形式, 帮助相关人员管理和监控应用.