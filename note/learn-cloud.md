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

## 2.配置客户端

### 技术回顾

- Spring Environment
  - Environment是一种在容器内以配置(profile)和属性(properties)为模型的应用环境抽象整合
    - StandardEnvironment
    - StandardServletEnvironment
- Spring Profile
  - 在Spring容器中, Profile是一种命名的Bean定义逻辑组
  - 一个Spring应用可以同时激活多个Profile(应用部署环境(test, stage, prod), 单元测试)
  - 通过调用ConfigurableEnvironment接口控制Profile的激活
    - setActiveProfiles(String...)
    - addActiveProfile(String)
    - setDefaultProfiles(String...)
- Spring Properties
  - 属性又称之为配置项, key-value的形式(PropertySources, PropertySource)
- Spring 时间监听器
  - 事件 (ApplicationEvent)
  - 事件监听器 (ApplicationListener)
- ConfigFileApplicationListener
  - 用于读取默认以及Profile关联的配置文件(application.properties)

### Environment

### Bootstrap

参考`BootstrapApplicationListener`实现

```java
String configName = environment.resolvePlaceholders("${spring.cloud.bootstrap.name:bootstrap");
```

- Bootstrap配置文件

```properties
## application.properties
## 通过调整 spring.cloud.bootstrap.enabled = false，尝试关闭 bootstrap 上下文
## 实际测试结果，没有效果
spring.cloud.bootstrap.enabled = false
```

>注意: `BootstrapApplicationListener` 加载实际早于 `ConfigFileApplicationListener`  
>原因:  
>`ConfigFileApplicationListener` 的DEFAULT_ORDER = Ordered.HIGHEST_PRECEDENCE + 10 (第十一位)
>`BootstrapApplicationListener` 的DEFAULT_ORDER = Ordered.HIGHEST_PRECEDENCE + 5 (第六位)  

如果需要调整控制Bootstrap上下文行为配置, 需要更高优先级, Order需要< `Ordered.HIGHEST_PRECEDENCE + 5` (越小越优先), 比如使用启动参数

```properties
--spring.cloud.bootstrap.enabled = false
```

- 调整Bootstrap配置文件路径
- 覆盖远程配置属性
- 自定义Bootstrap配置

创建`META-INF/spring.factories`文件（类似于 Spring Boot 自定义 Starter）

自定义 Bootstrap 配置 Configuration

```java
@Configuration
public class MyConfiguration implements ApplicationContextInitializer<ConfigurableApplicationContext> {

    /--
     - Initialize the given application context.
     -
     - @param applicationContext the application to configure
     -/
    @Override
    public void initialize(ConfigurableApplicationContext applicationContext) {
        // 从 ConfigurableApplicationContext 获取 ConfigurableEnvironment 实例
        ConfigurableEnvironment environment = applicationContext.getEnvironment();
        // 获取 PropertySources
        MutablePropertySources propertySources = environment.getPropertySources();
        // 定义一个新的 PropertySource，并且放置在首位
        propertySources.addFirst(createPropertySource());
    }

    private PropertySource<?> createPropertySource() {
        Map<String, Object> source = new HashMap<>();
        source.put("name", "小马哥");
        return new MapPropertySource("my-property-source", source);

    }
}
```

配置`META-INF/spring.factories`文件，关联Key `org.springframework.cloud.bootstrap.BootstrapConfiguration`

   ```properties
   org.springframework.cloud.bootstrap.BootstrapConfiguration= \
   cn.az.cloud.intro.config.MyConfiguration
   ```

- 自定义Bootstrap配置属性源

 实现`PropertySourceLocator`

```java
public class MyPropertySourceLocator implements PropertySourceLocator {

    @Override
    public PropertySource<?> locate(Environment environment) {
        if (environment instanceof ConfigurableEnvironment) {
            ConfigurableEnvironment configurableEnvironment = (ConfigurableEnvironment) environment;
            // 获取 PropertySources
            MutablePropertySources propertySources = configurableEnvironment.getPropertySources();
            // 定义一个新的 PropertySource，并且放置在首位
            propertySources.addFirst(createPropertySource());
        }
        return null;
    }

    private PropertySource<?> createPropertySource() {

        Map<String, Object> source = new HashMap<>();

        source.put("spring.application.name", "spring cloud programme");
        // 设置名称和来源
        return new MapPropertySource("over-bootstrap-property-source", source);
    }

}
```

配置`META-INF/spring.factories`

   ```properties
   org.springframework.cloud.bootstrap.BootstrapConfiguration= \
   cn.az.cloud.intro.config.MyConfiguration,\
   cn.az.cloud.intro.config.MyPropertySourceLocator
   ```

> 关于Spring Boot启动参数PropertySource处理 (Spring Framework)

## 3.配置服务器

### 分布式配置

- 分布式配置架构
- SpringCloud配置服务器
- 服务端Environment仓储
- SpringCloud配置客户端

### SpringCloud配置服务器

- Spring Cloud Config Server
  - SpringCloud配置服务器分布式, 动态化集中管理应用配置信息的能力
  - @EnableConfigServer
- {application} : 配置客户端应⽤名称，即配置项：`spring.application.name`
- {profile}：  配置客户端应⽤当前激活的Profile，即配置项：`spring.profiles.active`
- {label}：  配置服务端标记的版本信息，如Git 中的分⽀名

### 服务端配置映射

- /{application}/{profile}[/{label}]
- /{application}-{profile}.yml
- /{label}/{application}-{profile}.yml
- /{application}-{profile}.properties
- /{label}/{application}-{profile}.properties

### 动态配置Bean

post -> `http://localhost:8080/env`

> spring-cloud-config-monitor integration
---
> 哪些配置应该写在bootstrap.yml里, 哪些在application.yml里面

- bootstrap.yml配置SpringCloud属性
- application.yml 配置当前springboot应用属性
- 动态配置: 比如开关, 数据阈值, 可以配置在服务器

> 如何理解endpoint

endpoint是一种管理端点, 提供一种以http通讯方式的类似于JMX功能

## 服务发现/注册

### 服务发现 (Service Discovery)

在计算机网络中, 一种自动发现设备或服务的技术, 通过服务发现协议(Service Discovery Protocol)实现

### 服务注册(Service Registration)

在计算机网络中, 为了更好地治理多个设备或服务, 这些设备或者服务主动或被动注册到管理中心, 以便服务被发现和消费

- Apache Zookeeper (高一致性)
- Netflix Eureka (高可用性)
- Consul

### 高可用性(High Availability)

一种系统特性, 致力于确保可接受程度的操作执行, 通常采用上线时间作为基准

- 基本原则
  - 消灭单点故障
  - 可靠性交迭
  - 故障探测

## 高可用服务器

### Spring Cloud Eureka高可用

- 高可用客户端

多用于生产环境, 客户端应用关联或者配置注册中心服务集群, 避免注册中心单点故障

- 多注册中心主机
- 注册中心DNS
- 广播

---

- 应用元信息
  - 获取间隔 `eureka.client.registryFetchIntervalSeconds`
  - 同步间隔 `eureka.client.instanceInfoReplicationIntervalSeconds`

---

- 高可用注册中心

高可用注册中心不但需要提供集群环境, 解决单点故障的问题. 同时, 也优雅地处理注册中心之间信息同步的问题

- 配置属性 (sync)
  - `eureka.server.host1: eureka.client.serviceUrl,defaultZone=http//${eureka.server2.host}:${eureka.server2.port}/eureka`
  - `eureka.server.host2: eureka.client.serviceUrl,defaultZone=http//${eureka.server1.host}:${eureka.server1.port}/eureka`

### Spring Cloud Consul

- Consul 组件
  - 服务发现 (Service Discovery)
  - 健康检查 (Health Check)
  - 键值存储 (KV Store)
  - 多数据中心 (Multi Data Center)

## 4.负载均衡

- 理论基础
  - 客户端负载均衡
  - 服务端负载均衡
  - 调度算法
    - 先来先服务(First Come First Served)
    - 最早截止时间优先(Earliest deadline first)
    - 最短保留时间优先(Shortest remaining time first)
    - 固定优先级(Fixed Priority)
    - 轮询(Round-Robin)
    - 多级别队列(MultiLevel Queue)
  - 特性
    - 非对称负载(Aysmmetric load)
    - 分布式拒绝服务攻击保护
    - 直接服务返回
    - 健康检查
    - 优先级队列
- 技术回顾
  - Spring Framework (RestTemplate)
    - 序列化/反序列化: HttpMessageConvertor
    - 实现适配: ClientHttpRequestFactory
    - 请求拦截: ClientHttpRequestInterceptor

### Netflix Ribbon

```java
@SpringBootApplication
// 多个 Ribbon 定义
@RibbonClients({
        @RibbonClient(name = "spring-cloud-service-provider")
})
public class SpringCloudLesson6Application {

    public static void main(String[] args) {
        SpringApplication.run(SpringCloudLesson6Application.class, args);
    }

    //声明 RestTemplate
    @Bean
    @LoadBalance
    public RestTemplate restTemplate(){
        return new RestTemplate();
    }
}
```

实际请求客户端

- LoadBalancerClient
  - RibbonLoadBalancerClient

负载均衡上下文

- LoadBalancerContext
  - RibbonLoadBalancerContext

负载均衡器

- ILoadBalancer
  - BaseLoadBalancer
  - DynamicServerListLoadBalancer
  - ZoneAwareLoadBalancer
  - NoOpLoadBalancer

负载均衡规则

核心规则接口

- IRule
  - 随机规则：RandomRule
  - 最可用规则：BestAvailableRule
  - 轮训规则：RoundRobinRule
  - 重试实现：RetryRule
  - 客户端配置：ClientConfigEnabledRoundRobinRule
  - 可用性过滤规则：AvailabilityFilteringRule
  - RT权重规则：WeightedResponseTimeRule
  - 规避区域规则：ZoneAvoidanceRule

PING 策略

核心策略接口

- IPingStrategy

PING 接口

- IPing
  - NoOpPing
  - DummyPing
  - PingConstant
  - PingUrl

Discovery Client 实现

- NIWSDiscoveryPing

## 负载均衡客户端

### LoadBalanceClient

- 主要职责
  - 转化URI: 将含应用名称URI转化成具体主机+端口的形式
  - 选择服务实例: 通过负载算法, 选择指定服务中的一台机器实例
  - 请求执行回调: 针对选择后服务实例, 执行具体的请求回调操作
- 默认实现: RibbonLoadBalancerClient
- 自动装配源: `RibbonAutoConfiguration#loadBalancerClient()`

### 负载均衡器上下文 - LoadBalancerContext

- 主要职责
  - 转化URI: 将含应用名称URI转化成具体主机+端口的形式
  - 组件关联: 关联RetryHandler, ILoadBalancer等
  - 记录服务统计信息: 记录请求相应时间, 错误数量等
- 默认实现: `RibbonLoadBalancerContext`
- 自动装配源: `RibbonClientCOnfiguration#ribbonLoadBalancerContext(...)`

### 负载均衡器 - ILoadBalancer

- 主要职责
  - 增加服务器
  - 获取服务器: 通过关键key获取(所有服务列表, 可用服务列表)
  - 服务器状态: 标记服务器宕机
- 默认实现: `ZoneAwareLoadBalancer`
- 自动装配源: `RibbonClientCOnfiguration#ribbonLoadBalancer(...)`

### 规则接口 - IRule

- 主要职责
  - 选择服务器: 根据负载均衡器以及关联key获取候选的服务器
- 默认实现: `ZoneAvoidanceRule`
- 自动装配源: `RibbonClientCOnfiguration#ribbonRule(...)`

### Ping策略 - IPing

- 主要职责
  - 活动检测: 根据指定的服务器, 检测其是否活动
- 默认实现: `DummyPing`
- 自动装配源: `RibbonClientCOnfiguration#ribbonPing(...)`

### 服务器列表 - ServerList

- 主要职责
  - 获取初始化服务器列表
  - 获取更新服务器列表
- 默认实现: `ConfigurationBasedServerList` | `DiscoveryEnabledNIWSServerList`
- 自动装配源: `RibbonClientCOnfiguration#ribbonServerList(...)`

### Ribbon 自动装配

- RibbonAutoConfiguration
  - LoadBalancerClient
  - PropertiesFactory
- LoadBalancerAutoConfiguration
  - @LoadBalanced
  - RestTemplate

## 服务短路

### 核心概念

系统整体性保护

- 服务容错(Fault Tolerance): 强调容忍错误, 不至于整体故障
- 服务降级(DownGrade): 强调服务非强依赖, 不影响主要流程

### Spring Cloud Netflix Hystrix

- 整合Spring
  - @EnableHystrix
- 编程模型
  - 注解模式: @HystrixCommand
  - 编程模式: HystrixCommand
- 整合Spring Cloud
  - 激活：@EnableCircuitBreaker
  - 依赖：org.springframework.cloud:spring-cloud-starter-hystrix
- 端点
  - Endpoint：/hystrix.stream

```java
/**
     * 获取所有用户列表
     *
     * @return
     */
    @HystrixCommand(
            commandProperties = { // Command 配置
                    // 设置操作时间为 100 毫秒
                    @HystrixProperty(name = "execution.isolation.thread.timeoutInMilliseconds", value = "100")
            },
            fallbackMethod = "fallbackForGetUsers" // 设置 fallback 方法
    )
    @GetMapping("/user/list")
    public Collection<User> getUsers() throws InterruptedException {

        long executeTime = random.nextInt(200);

        // 通过休眠来模拟执行时间
        System.out.println("Execute Time : " + executeTime + " ms");

        Thread.sleep(executeTime);

        return userService.findAll();
    }
    /**
     * {@link #getUsers()} 的 fallback 方法
     *
     * @return 空集合
     */
    public Collection<User> fallbackForGetUsers() {
        return Collections.emptyList();
    }
```

### 生产准备特性

- 整合Spring Cloud
  - 激活：@EnableHystrixDashboard
  - 依赖：org.springframework.cloud:spring-cloud-starter-hystrix-dashboard
  - 收集

## Hystrix源码解读

### `@EnableCircuitBreaker`

职责：激活 Circuit Breaker

#### 初始化顺序

- `@EnableCircuitBreaker`
  - `EnableCircuitBreakerImportSelector`
    - `HystrixCircuitBreakerConfiguration`

### HystrixCircuitBreakerConfiguration

#### 初始化组件

- `HystrixCommandAspect`
- `HystrixShutdownHook`
- `HystrixStreamEndpoint`：Servlet
- `HystrixMetricsPollerConfiguration`

### `HystrixCommandAspect`

#### 依赖组件

- `MetaHolderFactory`
- `HystrixCommandFactory`: 生成`HystrixInvokable`
- `HystrixInvokable`
  - `CommandCollapser`
  - `GenericObservableCommand`
  - `GenericCommand`

### RxJava

```java
 Single.just("Hello,World") // 仅能发布单个数据
        .subscribeOn(Schedulers.io()) // 在 I/O 线程执行
        .subscribe(RxJavaDemo::println) // 订阅并且消费数据
;
```

```java
List<Integer> values = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9);

Observable.from(values) //发布多个数据
        .subscribeOn(Schedulers.computation()) // 在 I/O 线程执行
        .subscribe(RxJavaDemo::println); // 订阅并且消费数据

Observable.from(values) //发布多个数据
        .subscribeOn(Schedulers.newThread()) // 在 newThread 线程执行
        .subscribe(value -> {
            if (value > 2)
                throw new IllegalStateException("数据不应许大于 2");
            //消费数据
            println("消费数据：" + value);
        }, e -> {
            // 当异常情况，中断执行
            println("发生异常 , " + e.getMessage());
        }, () -> {
            // 当整体流程完成时
            println("流程执行完成");
        });
```

```java

import java.util.concurrent.Flow;
import java.util.concurrent.SubmissionPublisher;

public class SubmissionPublisherDemo {

    public static void main(String[] args) throws InterruptedException {

        try (SubmissionPublisher<Integer> publisher =
                     new SubmissionPublisher<>()) {

            //Publisher(100) => A -> B -> C => Done
            publisher.subscribe(new IntegerSubscriber("A"));
            publisher.subscribe(new IntegerSubscriber("B"));
            publisher.subscribe(new IntegerSubscriber("C"));

            // 提交数据到各个订阅器
            publisher.submit(100);
        }
        Thread.currentThread().join(1000L);

    }

    private static class IntegerSubscriber implements
            Flow.Subscriber<Integer> {

        private final String name;
        private Flow.Subscription subscription;

        private IntegerSubscriber(String name) {
            this.name = name;
        }

        @Override
        public void onSubscribe(Flow.Subscription subscription) {
            System.out.printf(
                    "Thread[%s] Current Subscriber[%s] " +
                            "subscribes subscription[%s]\n",
                    Thread.currentThread().getName(),
                    name,
                    subscription);
            this.subscription = subscription;
            subscription.request(1);
        }

        @Override
        public void onNext(Integer item) {
            System.out.printf(
                    "Thread[%s] Current Subscriber[%s] " +
                            "receives item[%d]\n",
                    Thread.currentThread().getName(),
                    name,
                    item);
            subscription.request(1);
        }

        @Override
        public void onError(Throwable throwable) {
            throwable.printStackTrace();
        }

        @Override
        public void onComplete() {
            System.out.printf(
                    "Thread[%s] Current Subscriber[%s] " +
                            "is completed!\n",
                    Thread.currentThread().getName(),
                    name);
        }

    }

}
```

## Spring Cloud 服务调用

### 远程过程调用(RPC)

⼀个计算机通信协议。该协议允许运⾏于⼀台计算机的程序调⽤另⼀台计算机的⼦程序，⽽程序员⽆需额外地为这个交互作⽤编程。如果涉及的软件采⽤⾯向对象编程，那么远程过程调⽤亦可称作远程调⽤或远程⽅法调⽤

- RMI (二进制协议)
- WebSerices (文本协议)

### 服务调用核心概念

- 消息传递
  - RPC 是⼀种请求-响应协议，⼀次RPC在客户端初始化，再由客户端将请求消息传递到远程的服务器，执⾏指定的带有参数的过程。经过远程服务器执⾏过程后，将结果作为响应内容返回到客户端。
- 存根（Stub）
  - 在⼀次分布式计算RPC 中，客户端和服务器转化参数的⼀段代码。 由于存根的参数转化，RPC执⾏过程如同本地执⾏函数调⽤。存根必须在客户端和服务器两端均装载，并且保持兼容

### Spring Cloud Feign

- `org.springframework.cloud:spring-cloud-starter-openfeign`
- `@EnableFeignClients`
- `@FeignClient`

- 默认组件
  - Decoder/Encoder : ResponseEntityDecoder / SpringEncoder
  - Logger : Slf4jLogger
  - Contract : SpringMvcContract
  - Feign.Builder : HystrixFeign.Builder
  - Client : LoadBalancerFeignClient

```java
@SpringBootApplication
@RibbonClient("user-service-provider") // 指定目标应用名称
@EnableCircuitBreaker // 使用服务短路
@EnableFeignClients(clients = UserService.class) // 申明 UserService 接口作为 Feign Client 调用
public class UserServiceClientApplication {

    public static void main(String[] args) {
        SpringApplication.run(UserServiceClientApplication.class, args);
    }

    /**
     * 将 {@link MyRule} 暴露成 {@link Bean}
     *
     * @return {@link MyRule}
     */
    @Bean
    public IRule myRule() {
        return new MyRule();
    }

    /**
     * 申明 具有负载均衡能力 {@link RestTemplate}
     *
     * @return
     */
    @Bean
    @LoadBalanced
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }

}
```

## Spring Cloud 服务网关

⽹关是程序或者系统之间的连接节点，扮演着程序或系统之间的⻔户，允许它们之间通过通讯协议
交换信息，它们可能是同构和异构的系统。

- REST API 网关
- WebServices 网关

---

- 使用场景
  - 监控（Monitoring）
  - 测试（Testing）
  - 动态路由（Dynamic Routing）
  - 服务整合（Service Integration）
  - 负荷减配（Load Shedding）
  - 安全(Security)
  - 静态资源处理(Static Resources handling)
  - 活跃流量管理(Active traffic management)

### 服务网关 - 核心概念

- 数据来源
  - 服务发现
  - 服务注册
- 通讯⽅式
  - 协议：⼆进制、本⽂
  - ⽅式：同步、异步
- 服务接⼝
  - 平台⽆关
    - XML、JSON、HTML
  - 平台相关
    - IDL、RMI、Hession

### Spring Cloud Zuul

- 依赖
  - `org.springframework.cloud:spring-cloud-starter-zuul`
- 激活
  - `@EnableZuulProxy`
- 配置
  - zuul.*
- 路由设置
  - 配置模式
    - 服务-映射：`zuul.routes.${service-id} = ${url-pattern}`
  - 路径模式
    - 当前层级匹配：/*
    - 递归层级匹配：/**
- HTTP 客户端
  - HttpClient（默认）
    - 装配：`HttpClientRibbonConfiguration`
  - OkHttp（条件）
    - 激活配置：`ribbon.okhttp.enabled = true`
    - 装配：`OkHttpRibbonConfiguration`
- 端点（Endpoint）
  - 实现：`RoutesEndpoint`
  - 路径：/routes
- 过滤器：ZuulFilter

```java
@EnableZuulProxy
@EnableDiscoveryClient
@SpringBootApplication
public class ZuulProxyApplication {

    public static void main(String[] args) {
        SpringApplication.run(ZuulProxyApplication.class, args);
    }

}
```

ZuulFilter 调用链

`ZuulFilter#run()` <- `ZuulFilter#runFilter()` <- `FilterProcessor#runFilters`

- `FilterProcessor#preRoute()`
- `FilterProcessor#route()`
- `FilterProcessor#postRoute()`

## Zuul 自动装配

`ZuulServletFilter` 适用范围更大，可以拦截所有的`Servlet`，包括 `ZuulServlet`

`ZuulServlet` 会有URL 匹配的模式，url-pattern

Zuul 有两种的激活模式：

### `@EnableZuulProxy`

导入`ZuulProxyMarkerConfiguration`，随后生成一个`ZuulProxyMarkerConfiguration.Marker()` Bean，这个Bean 作为`ZuulProxyAutoConfiguration` 的装配前置条件。

请注意：`ZuulProxyMarkerConfiguration` 扩展了 `ZuulServerAutoConfiguration`，所以 `ZuulServlet` 和`ZuulController`会被自动装配

`ZuulController` 有 `DispatcherServlet` 来在控制,它的映射地址是："/*"，

`DispatcherServlet` 中注册了一个`ZuulHandlerMapping` ，它控制映射到`ZuulController`，可以参考`ZuulServerAutoConfiguration`:

```java
   @Bean
   public ZuulController zuulController() {
    return new ZuulController();
   }

   @Bean
   public ZuulHandlerMapping zuulHandlerMapping(RouteLocator routes) {
    ZuulHandlerMapping mapping = new ZuulHandlerMapping(routes, zuulController());
    mapping.setErrorController(this.errorController);
    return mapping;
   }
 ```

通过源码分析，`ZuulController`  将请求委派给`ZuulServlet`，所以所有的`ZuulFilter` 实例都会被执行。

> 因此，访问 <http://localhost:6060/user-service-client/user/find/all> ，实际将请求递交给 DispatcherServlet
> 发送请求"/user-service-client/user/find/all"

- `DispatcherServlet`
  - `ZuulHandlerMapping`
    - `ZuulController`
      - `ZuulServlet`
        - `RibbonRoutingFilter`

#### `@EnableZuulServer`

导入`ZuulServerMarkerConfiguration` ，随后生成一个 `ZuulServerMarkerConfiguration.Marker()` Bean ，主要用作引导装配`ZuulServerAutoConfiguration`

`ZuulServerAutoConfiguration`与 父类 `ZuulProxyAutoConfiguration` 区别：

父类`ZuulProxyAutoConfiguration` 提供了`RibbonRoutingFilter`

调用层次：

- `DispatcherServlet`
  - `ZuulHandlerMapping`
    - `ZuulController`
      - `ZuulServlet`
        - `ZuulFilter`

## Spring Cloud 消息整合

### Spring Cloud Stream

消息驱动的微服务应⽤

- 简化编码
- 统一抽象

#### 主要概念

1. 应用模型
2. Binder抽象
3. 持久化 发布/订阅支持
4. 消费分组支持
5. 分区支持

#### 基本概念

• Source：Stream 发送源
  • 近义词：Producer、Publisher
• Sink：Stream 接收器
  • 近义词：Consumer、Subscriber
• Processor

#### 编程模型

• 激活: @EnableBinding
  • @Configuration
  • @EnableIntegration
• Source
  • @Output
  • MessageChannel
• Sink
  • @Input
  • SubscribableChannel
  • @ServiceActivator
  • @StreamListener
