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

    /**
     * Initialize the given application context.
     *
     * @param applicationContext the application to configure
     */
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
