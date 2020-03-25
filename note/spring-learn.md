# 深入学习SpringFramework

## Spring Framework总览

### 1.特性总览

- core特性
  - IoC容器(IoC Container)
  - Spring事件(Events)
  - 资源管理(Resources)
  - 国际化(i18n)
  - 校验(validation)
  - 数据绑定(data binding)
  - 类型转换(type conversion)
  - Spring表达式(Spring Express Language)
  - 面向切面编程(AOP)
- 数据存储(Data Access)
  - JDBC
  - 事务抽象(Transactions)
  - DAO支持(Dao Support)
  - O/R映射(O/R mapping)
  - XML编列(XML Marshalling)
- Web Servlet
  - Spring MVC
  - WebSocket
  - SockJS
- Web Reactive
  - Spring WebFlux
  - WebClient
  - WebSocket
- 技术整合
  - 远程调用(Remoting)
  - Java消息服务(JMS)
  - Java连接架构(JCA)
  - Java管理扩展(JMX)
  - Java邮件客户端(Email)
  - 本地任务(Tasks)
  - 本地调度(Scheduling)
  - 缓存抽象(Caching)
  - Spring测试(Testing)
- 测试(testing)
  - 模拟对象(Mock Objects)
  - TestContext框架
  - SpringMVC测试
  - Web测试客户端(WebTestClient)

### 2.版本特性

- 1.x -> 1.3+ -> J2EE 1.3+
- 2.x -> 1.4.2+ -> J2EE 1.3+
- 3.x -> 5+ -> J2EE 1.4 & 1.5
- 4.x -> 6+ -> Java EE 6 & 7
- 5.x -> 8+ -> Java EE 7

### 3.模块化设计

- spring-aop
- spring-aspect
- spring-context-indexer
- spring-context-support
- spring-context
- spring-core
- spring-expression
- spring-instrument
- spring-jcl
- spring-jdbc
- spring-jms
- spring-messaging
- spring-orm
- spring-oxm
- spring-test
- spring-tx
- spring-web
- spring-webflux
- spring-webmvc
- spring-websocket

### 4.对Java语言特性应用

- 1.5 (枚举, 泛型, 注解, 封箱(解箱))
  - 注解 @Transactional
  - 枚举 Propagation
  - for-each AbstractApplicationContext
  - 泛型 ApplicationListener
- 1.6 `@Override`接口
- 1.7 Diamond语法, 多个catch, try(resource)
  - Diamond DefaultListableBeanFactory
  - try ResourceBundleMessageSource
- 1.8 Lambda语法, 可重复注解, 类型注解
  - Lambda PropertyEditorRegitrySupport
- 1.9 模块化, 接口私有方法
- 1.10 局部变量类型推断

### 5.对JDK API实践

- 小于Java5 API
  - 反射 MethodMatcher
  - Java Beans CachedIntrospectionResults
  - 动态代理(Dynamic Proxy) JdkDynamicAopProxy
- Java5 API
  - XML处理 XmlBeanDefinitionReader
  - Java管理扩展 @ManagedResource
  - Instrumentation InstrumentationSavingAgent
  - 并发框架 ThreadPoolTaskScheduler
  - 格式化 DataFormatter
- Java6 API
  - JDBC4.0 JdbcTemplate
  - Common Annotations CommonAnnotationBeanPostProcessor
  - JAXB2.0 Jaxb2Marshaller
  - Scripting in JVM StandardScriptFactory
  - 可插拔注解处理 API @Indexed
  - Java Compiler API TestCompiler
- Java7 API
  - Fork/Join框架 ForkJoinPoolFactoryBean
  - NIO2 PathResource
- Java8 API
  - Data and Time API DateTimeContext
  - 可重复Annotations @PropertySources
  - Stream API StreamConverter
  - CompletableFuture CompletableToListenableFutureAdapter

### 6.对Java EE API的整合

- Java EE web技术相关
  - Servlet+jsp DispatcherServlet
  - JSTL JstlView
  - JavaServer Faces FacesCOntextUtils
  - Portlet DispatcherPorlet
  - SOAP SoapFaultException
  - WebServices CommonAnnotationBeanPostProcessor
  - WebSocket WebSocketHandler
- Java EE 数据存储相关
  - JDO JdoTemplate
  - JTA JtaTransactionManager
  - JPA JpaTransactionManager
  - Java Caching API JCacheCache
- Java EE Bean技术相关
  - JMS JmsTemplate
  - EJB 2.0 AbstractStatefulSesionBean
  - Dependency Injection AutowiredAnnotationBeanPostProcessor
  - Bean Validation LocalValidatorFactoryBean

### 7.编程模型

- 面向对象编程 OOP
- 面向切面编程 AOP
- 面向元编程 METADATA
- 函数驱动
- 模块驱动

### 8.核心价值

- 生态系统
- API 抽象设计
- 编程模型
- 设计思想
- 设计模块
- 用户基础

### 9.面试题

> 什么是Spring Framework?

Spring makes it easy to create java enterprise applications. It provides everything you need to embrace the java language in an enterprise environment, with support for Groovy and Kotolin as alternative languages on the JVM, and with the flexibility to create many kinds of architectures depending on an application's needs.
> Spring Framework有哪些核心模块?

- spring-core: Spring基础API模块, 如资源管理, 范型处理
- spring-beans: Spring Bean相关, 如依赖查找, 依赖注入
- spring-aop: Spring AOP处理, 如动态代理, AOP字节码提升
- spring-context: 事件驱动, 注解驱动, 模块驱动等
- spring-expression: Spring表达式语言模块

> Spring Framework的优势和不足有哪些?

## Spring Framework IOC

### IOC主要实现策略, 和DI的关系

- Using a service locator pattern
- Using dependency injection(构造器注入, Setter注入, 属性注入, 接口注入)
- Using a contextualized lookup
- Using template method design pattern
- Using strategy design pattern

### IOC职责

- 通用职责
  - 依赖处理
    - 依赖查找
    - 依赖注入
  - 生命周期管理
    - 容器
    - 托管的资源(JavaBeans或其他资源)
  - 配置
    - 容器
    - 外部化配置
    - 托管的资源

### 传统 IoC 容器

- Java Beans (`java.beans.PropertyEditor`)

### 面试题

> 什么是IOC?

简单地说, IoC是反转控制, 类似于好莱坞原则, 主要有依赖查找和依赖注入实现
> 依赖查找和依赖注入的区别?

依赖查找是主动或手动的依赖查找方式, 通常需要依赖容器或标准API实现; 而依赖注入则是手动绑定或自动依赖绑定的方式, 无需依赖特定的容器和API.
> Spring作为IoC容器有什么优势?

- 典型的IoC管理, 依赖查找和依赖注入
- AOP 抽象
- 事件抽象
- 事件机制
- SPI 扩展
- 强大的第三方整合
- 易测试性
- 更好的面向对象

## Spring IoC容器钙素

### 1.Spring IoC依赖查找

- 根据Bean名称查找
  - 实时查找
  - 延迟查找
- 根据Bean类型查找
  - 单个Bean对象
  - 集合Bean对象
- 根据Bean名称+类型查找
- 根据Java注解查找
  - 单个Bean对象
  - 集合Bean对象

### 2.Spring Ioc依赖注入

- 根据 Bean 名称注入
- 根据 Bean 类型注入
  - 单个 Bean 对象
  - 集合 Bean 对象
- 注入容器內建 Bean 对象
- 注入非 Bean 对象
- 注入类型
  - 实时注入
  - 延迟注入

### 3.Spring IoC 依赖来源

- 自定义Bean
- 容器内建Bean对象
- 容器内建依赖

### 4.Spring IoC 配置元信息

- Bean 定义配置
  - 基于 XML 文件
  - 基于 Properties 文件
  - 基于 Java 注解
  - 基于 Java API(专题讨论)
- IoC 容器配置
  - 基于 XML 文件
  - 基于 Java 注解
  - 基于 Java API (专题讨论) - 外部化属性配置
- 基于 Java 注解

---
> BeanFactory和ApplicationContext 谁才是Spring IoC容器?

- The `BeanFactory` interface provides an advanced configuration mechanism capable of managing any any type of object.
- `ApplicationContext` is a sub-interface of BeanFactory. It adds:
  - Easier integration with Spring AOP features
  - Message resource handling
  - Event publication
  - Application-layer specific contexts (`WebApplicationContext`)

### 5.Spring 应用上下文

- ApplicationContext除了IoC角色, 还提供:
  - 面向切面(AOP)
  - 配置元信息(Configurable Metadata)
  - 资源管理(Resources)
  - 事件(Events)
  - 国际化(i18n)
  - 注解(Annotations)
  - Environment抽象

---

- BeanFacotry是Spring IoC容器
- ApplicationContext是具备应用特性的BeanFactory超集

### 6.Spring Ioc 生命周期

- 启动
- 运行
- 停止

### 7.面试题

> 什么是Spring IoC容器?

Spring Framework implementation of the Inversion of Control (IoC) principle. IoC is also known as dependency injection (DI). It is a process whereby objects define their dependencies (that is, the other objects they work with) only through constructor arguments, arguments to a factory method, or properties that are set on the object instance after it is constructed or returned from a factory method. The container then injects those dependencies when it creates the bean.
> BeanFactory 与 FactoryBean?

BeanFactory是IoC底层容器  
FactoryBean是创建Bean的一种方式, 帮助实现复杂的初始化逻辑
> Spring IoC容器启动时做了哪些准备?

IoC配置元信息读取和解析, IoC容器生命周期, Spring事件发布, 国际化等.
