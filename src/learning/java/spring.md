# 深入学习 SpringFramework

## Spring Framework 总览

### 1.特性总览

- core 特性
  - IoC 容器(IoC Container)
  - Spring 事件(Events)
  - 资源管理(Resources)
  - 国际化(i18n)
  - 校验(validation)
  - 数据绑定(data binding)
  - 类型转换(type conversion)
  - Spring 表达式(Spring Express Language)
  - 面向切面编程(AOP)
- 数据存储(Data Access)
  - JDBC
  - 事务抽象(Transactions)
  - DAO 支持(Dao Support)
  - O/R 映射(O/R mapping)
  - XML 编列(XML Marshalling)
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
  - Java 消息服务(JMS)
  - Java 连接架构(JCA)
  - Java 管理扩展(JMX)
  - Java 邮件客户端(Email)
  - 本地任务(Tasks)
  - 本地调度(Scheduling)
  - 缓存抽象(Caching)
  - Spring 测试(Testing)
- 测试(testing)
  - 模拟对象(Mock Objects)
  - TestContext 框架
  - SpringMVC 测试
  - Web 测试客户端(WebTestClient)

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

### 4.对 Java 语言特性应用

- 1.5 (枚举, 泛型, 注解, 封箱(解箱))
  - 注解 @Transactional
  - 枚举 Propagation
  - for-each AbstractApplicationContext
  - 泛型 ApplicationListener
- 1.6 `@Override`接口
- 1.7 Diamond 语法, 多个 catch, try(resource)
  - Diamond `DefaultListableBeanFactory`
  - try `ResourceBundleMessageSource`
- 1.8 Lambda 语法, 可重复注解, 类型注解
  - Lambda `PropertyEditorRegistrySupport`
- 1.9 模块化, 接口私有方法
- 1.10 局部变量类型推断

### 5.对 JDK API 实践

- 小于 Java5 API
  - 反射 MethodMatcher
  - Java Beans `CachedIntrospectionResults`
  - 动态代理(Dynamic Proxy) `JdkDynamicAopProxy`
- Java5 API
  - XML 处理 `XmlBeanDefinitionReader`
  - Java 管理扩展 `@ManagedResource`
  - Instrumentation `InstrumentationSavingAgent`
  - 并发框架 `ThreadPoolTaskScheduler`
  - 格式化 `DataFormatter`
- Java6 API
  - JDBC4.0 `JdbcTemplate`
  - Common `Annotations` CommonAnnotationBeanPostProcessor
  - JAXB2.0 Jaxb2Marshaller
  - Scripting in JVM StandardScriptFactory
  - 可插拔注解处理 API @Indexed
  - Java Compiler API TestCompiler
- Java7 API
  - Fork/Join 框架 ForkJoinPoolFactoryBean
  - NIO2 PathResource
- Java8 API
  - Data and Time API DateTimeContext
  - 可重复 Annotations @PropertySources
  - Stream API StreamConverter
  - CompletableFuture CompletableToListenableFutureAdapter

### 6.对 Java EE API 的整合

- Java EE web 技术相关
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
- Java EE Bean 技术相关
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

> 什么是 Spring Framework?

Spring makes it easy to create java enterprise applications. It provides everything you need to embrace the java language in an enterprise environment, with support for Groovy and Kotolin as alternative languages on the JVM, and with the flexibility to create many kinds of architectures depending on an application's needs.

> Spring Framework 有哪些核心模块?

- spring-core: Spring 基础 API 模块, 如资源管理, 范型处理
- spring-beans: Spring Bean 相关, 如依赖查找, 依赖注入
- spring-aop: Spring AOP 处理, 如动态代理, AOP 字节码提升
- spring-context: 事件驱动, 注解驱动, 模块驱动等
- spring-expression: Spring 表达式语言模块

> Spring Framework 的优势和不足有哪些?

## Spring Framework IOC

### IOC 主要实现策略, 和 DI 的关系

- Using a service locator pattern
- Using dependency injection(构造器注入, Setter 注入, 属性注入, 接口注入)
- Using a contextualized lookup
- Using template method design pattern
- Using strategy design pattern

### IOC 职责

- 通用职责
  - 依赖处理
    - 依赖查找
    - 依赖注入
  - 生命周期管理
    - 容器
    - 托管的资源(JavaBeans 或其他资源)
  - 配置
    - 容器
    - 外部化配置
    - 托管的资源

### 传统 IoC 容器

- Java Beans (`java.beans.PropertyEditor`)

### 面试题

> 什么是 IOC?

简单地说, IoC 是反转控制, 类似于好莱坞原则, 主要有依赖查找和依赖注入实现

> 依赖查找和依赖注入的区别?

依赖查找是主动或手动的依赖查找方式, 通常需要依赖容器或标准 API 实现; 而依赖注入则是手动绑定或自动依赖绑定的方式, 无需依赖特定的容器和 API.

> Spring 作为 IoC 容器有什么优势?

- 典型的 IoC 管理, 依赖查找和依赖注入
- AOP 抽象
- 事件抽象
- 事件机制
- SPI 扩展
- 强大的第三方整合
- 易测试性
- 更好的面向对象

## Spring IoC 容器概述

### 1.Spring IoC 依赖查找

- 根据 Bean 名称查找
  - 实时查找
  - 延迟查找
- 根据 Bean 类型查找
  - 单个 Bean 对象
  - 集合 Bean 对象
- 根据 Bean 名称+类型查找
- 根据 Java 注解查找
  - 单个 Bean 对象
  - 集合 Bean 对象

### 2.Spring Ioc 依赖注入

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

- 自定义 Bean
- 容器内建 Bean 对象
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

> BeanFactory 和 ApplicationContext 谁才是 Spring IoC 容器?

- The `BeanFactory` interface provides an advanced configuration mechanism capable of managing any any type of object.
- `ApplicationContext` is a sub-interface of BeanFactory. It adds:
  - Easier integration with Spring AOP features
  - Message resource handling
  - Event publication
  - Application-layer specific contexts (`WebApplicationContext`)

### 5.Spring 应用上下文

- ApplicationContext 除了 IoC 角色, 还提供:
  - 面向切面(AOP)
  - 配置元信息(Configurable Metadata)
  - 资源管理(Resources)
  - 事件(Events)
  - 国际化(i18n)
  - 注解(Annotations)
  - Environment 抽象

---

- BeanFacotry 是 Spring IoC 容器
- ApplicationContext 是具备应用特性的 BeanFactory 超集

### 6.Spring Ioc 生命周期

- 启动
- 运行
- 停止

### 7.面试题

> 什么是 Spring IoC 容器?

Spring Framework implementation of the Inversion of Control (IoC) principle. IoC is also known as dependency injection (DI). It is a process whereby objects define their dependencies (that is, the other objects they work with) only through constructor arguments, arguments to a factory method, or properties that are set on the object instance after it is constructed or returned from a factory method. The container then injects those dependencies when it creates the bean.

> BeanFactory 与 FactoryBean?

BeanFactory 是 IoC 底层容器  
FactoryBean 是创建 Bean 的一种方式, 帮助实现复杂的初始化逻辑

> Spring IoC 容器启动时做了哪些准备?

IoC 配置元信息读取和解析, IoC 容器生命周期, Spring 事件发布, 国际化等.

## Spring Bean 基础

### 1. 定义 Spring Bean

> 什么是 Bean Definition?

BeanDefinition 是 Spring Framework 中定义 Bean 的配置元信息接口, 包含:

- Bean 的类名
- Bean 行为配置元素, 如作用域, 自动绑定的模式, 生命周期回调等
- 其他 Bean 引用, 又可称为合作者或依赖
- 配置设置, 如 Bean 属性

### 2. BeanDefinition 元信息

- BeanDefinition 元信息:
  - Class Bean 全类名, 必须是具体类(不能用抽象类或接口)
  - Name Bean 的名称或 ID
  - Scope Bean 的作用域(Singleton, prototype)
  - Constructor arguments Bean 构造器参数(用于依赖注入)
  - Properties Bean 属性设置(用于依赖注入)
  - Autowiring mode Bean 自动绑定模式(如:byName)
  - Lazy initailization mode Bean 延迟初始化模式(延迟和非延迟)
  - Initialization method Bean 初始化回调方法名称
  - Destruction method Bean 销毁回调方法名称
- BeanDefinition 构建
  - 通过 BeanDefinitionBuilder
  - 通过 AbstractBeanDefinition 以及派生类

### 3. 命名 Spring Bean

每个 Bean 拥有一个或多个标识符(identifiers)，这些标识符在 Bean 所在的容器必须是唯一 的。通常，一个 Bean 仅有一个标识符，如果需要额外的，可考虑使用别名(Alias)来扩充  
在基于 XML 的配置元信息中，开发人员可用 id 或者 name 属性来规定 Bean 的 标识符。通常 Bean 的 标识符由字母组成，允许出现特殊字符。如果要想引入 Bean 的别名的话，可在 name 属性使用半角逗号(“,”)或分号(“;”) 来间隔  
Bean 的 id 或 name 属性并非必须制定，如果留空的话，容器会为 Bean 自动生成一个唯一的 名称。Bean 的命名尽管没有限制，不过官方建议采用驼峰的方式，更符合 Java 的命名约定

### 4. Spring Bean 的别名

- Bean 别名(Alias)的价值
  - 复用现有的 BeanDefinition
  - 更具有场景化的命名方法

### 5. 注册 Spring Bean

- XML 配置元信息
  - `<bean name=”...” ... />`
- Java 注解配置元信息
  - @Bean
  - @Component
  - @Import
- Java API 配置元信息
  - 命名方式: `BeanDefinitionRegistry#registerBeanDefinition(String,BeanDefinition)`
  - 非命名方式: `BeanDefinitionReaderUtils#registerWithGeneratedName(AbstractBeanDefinition,BeanDefinitionRegistry)`
- 配置类方式: `AnnotatedBeanDefinitionReader#register(Class...)`

`SingletonBeanRegistry#registerSingleton`

### 6. 实例化 Spring Bean

- 常规方式
  - 通过构造器(配置元信息:XML、Java 注解和 Java API )
  - 通过静态工厂方法(配置元信息:XML 和 Java API )
  - 通过 Bean 工厂方法(配置元信息:XML 和 Java API )
  - 通过 FactoryBean(配置元信息:XML、Java 注解和 Java API )
- 特殊方式
  - 通过 ServiceLoaderFactoryBean(配置元信息:XML、Java 注解和 Java API )
  - 通过 AutowireCapableBeanFactory#createBean(java.lang.Class, int, boolean)
  - 通过 BeanDefinitionRegistry#registerBeanDefinition(String,BeanDefinition)

### 7. 初始化 Spring Bean

- @PostConstruct 标注方法
- 实现 InitializingBean 接口的 afterPropertiesSet() 方法
- 自定义初始化方法
  - XML 配置: `<bean init-method=”init” ... />`
  - Java 注解: `@Bean(initMethod=”init”)`
  - Java API: `AbstractBeanDefinition#setInitMethodName(String)`

### 8. 延迟初始化 Spring Bean

- XML 配置: `<bean lazy-init=”true” ... />`
- Java 注解: `@Lazy(true)`

### 9. 销毁 Spring Bean

- @PreDestroy 标注方法 s
- 实现 DisposableBean 接口的 destroy() 方法
- 自定义销毁方法
  - XML 配置: `<bean destroy=”destroy” ... />`
  - Java 注解: `@Bean(destroy=”destroy”)`
  - Java API: `AbstractBeanDefinition#setDestroyMethodName(String)`

### 10. 垃圾回收 Spring Bean

1. 关闭 Spring 容器(应用上下文)
2. 执行 GC
3. Spring Bean 覆盖的 finalize() 方法被回调

### 11. 面试题

> 如何注册一个 Spring Bean?

通过 BeanDefinition 和外部单体对象来注册 (`SingletonBeanRegistry`)

> 什么是 Spring BeanDefinition?

参照上面的笔记

> Spring 容器是怎样管理注册 Bean

IoC 配置元信息读取和解析, 依赖查找和注入以及 Bean 生命周期

## Spring IoC 依赖查找

### 1. 依赖查找的今生前世

- 单一类型依赖查找
  - JNDI -> `javax.naming.Context#lookup(javax.naming.Name)`
  - JavaBeans -> `java.beans.beancontext.BeanContext`
- 集合类型依赖查找
  - `java.beans.beancontext.BeanContext`
- 层次性依赖查找
  - `java.beans.beancontext.BeanContext`

### 2. 单一类型依赖查找

- 单一类型依赖查找接口 - BeanFactory
  - 根据 Bean 名称查找
    - getBean(String)
    - Spring 2.5 覆盖默认参数:getBean(String,Object...)
  - 根据 Bean 类型查找
    - Bean 实时查找
      - Spring 3.0 getBean(Class)
      - Spring 4.1 覆盖默认参数:getBean(Class,Object...)
    - Spring 5.1 Bean 延迟查找
      - getBeanProvider(Class)
      - getBeanProvider(ResolvableType)
  - 根据 Bean 名称 + 类型查找:getBean(String,Class)

### 3. 集合类型依赖查找

- 集合类型依赖查找接口 - ListableBeanFactory
  - 根据 Bean 类型查找
    - 获取同类型 Bean 名称列表
      - getBeanNamesForType(Class)
      - Spring 4.2 getBeanNamesForType(ResolvableType)
    - 获取同类型 Bean 实例列表
      - getBeansOfType(Class) 以及重载方法
  - 通过注解类型查找
    - Spring 3.0 获取标注类型 Bean 名称列表
      - getBeanNamesForAnnotation(Class<? extends Annotation>)
    - Spring 3.0 获取标注类型 Bean 实例列表
      - getBeansWithAnnotation(Class<? extends Annotation>)
    - Spring 3.0 获取指定名称 + 标注类型 Bean 实例
      - findAnnotationOnBean(String,Class<? extends Annotation>)

### 4. 层次性依赖查找

- 层次性依赖查找接口 - HierarchicalBeanFactory
  - 双亲 BeanFactory:getParentBeanFactory()
  - 层次性查找
    - 根据 Bean 名称查找
      - 基于 containsLocalBean 方法实现
    - 根据 Bean 类型查找实例列表
      - 单一类型:BeanFactoryUtils#beanOfType
      - 集合类型:BeanFactoryUtils#beansOfTypeIncludingAncestors
    - 根据 Java 注解查找名称列表
      - BeanFactoryUtils#beanNamesForTypeIncludingAncestors

### 5. 延迟依赖查找

- Bean 延迟依赖查找接口
  - org.springframework.beans.factory.ObjectFactory
  - org.springframework.beans.factory.ObjectProvider
    - Spring 5 对 Java 8 特性扩展
      - 函数式接口
      - getIfAvailable(Supplier)
      - ifAvailable(Consumer)
      - Stream 扩展 - stream()

### 6. 安全依赖查找

- 不安全: `BeanFactory.getBean(), ObjectFactory.getObject()`
- 安全: `ObjectProvider.getIfAvailable(),ListableBeanFactory.getBeansOfType(), ObjectProvider.stream()`

层次性依赖查找的安全性取决于其扩展的单一或集合类型的 BeanFactory 接口

### 7. 内建可查找的依赖

- ConfigurationAnnotationProcessor
- AutowiredAnnotationBeanPostProcessor
- CommonAnnotationBeanPostProcessor
- EventListenerMethodProcessor
- DefaultEventListenerFactory
- PersistenceAnnotationBeanPostProcessor

### 8. 依赖查找中的经典异常

- NoSuchBeanDefinitionException
- NoUniqueBeanDefninitonException
- BeanInstantiationException
- BeanCreationException
- BeanDefinitionStoreException

### 9. 面试题精选

> ObjectFactory 与 BeanFactory 的区别?

ObjectFactory 与 BeanFactory 均提供依赖查找的能力。  
不过 ObjectFactory 仅关注一个或一种类型的 Bean 依赖查找，并且自身不具备依赖查找的能力，能力则由 BeanFactory 输出。  
BeanFactory 则提供了单一类型、集合类型以及层次性等多种依赖查找方式。

> BeanFactory.getBean 操作是否线程安全?

BeanFactory.getBean 方法的执行是线程安全的, 操作过程中会增加互斥锁

> Spring 依赖查找和注入在来源上的区别?

## Spring IoC 依赖注入

### 1. 依赖注入的模式和类型

- 手动模式 - 配置或编程的方式, 提前安排注入规则
  - XML 资源配置元信息
  - Java 注解元信息
  - API 配置元信息
- 自动模式 - 实现方提供依赖自动关联的方式, 按照内建的注入规则
  - Autowiring
- `<property name="user" ref="userBean"/>`
- `<constructor-arg name="user" ref="userBean"/>`
- `@Autowired User user`;
- `@Autowired public void user(User user){...}`
- `class MyBean implements BeanFactoryAware{...}`

### 2. 自动绑定(Autowiring)

The Spring container can autowire relationships between collaborating beans. You can let Spring resolve collaborators (other beans) automatically for your bean by inspecting the contents of the ApplicationContext.

### 3. 自动绑定模式

- no, 未激活自动绑定
- byName
- byType
- constructor

### 4. 自动绑定的限制和不足

### 5. Setter 方法依赖注入

- 手动模式
  - XML 资源配置元信息
  - Java 注解配置元信息
  - API 配置元信息
- 自动模式
  - byName
  - byType

### 6. 构造器依赖注入

- 手动模式
  - XML 资源配置元信息
  - Java 注解配置元信息
  - API 配置元信息
- 自动模式
  - constructor

### 7. 字段注入

`@Autowired` 会忽略掉静态字段

- Java 注解配置元信息
  - @Autowired
  - @Resource
  - @Inject(可选)

### 8. 方法注入

- Java 注解配置元信息
  - @Autowired
  - @Resource
  - @Inject(可选)
  - @Bean

### 9. 回调注入

- BeanFactoryAware: 获取 IoC 容器 - BeanFactory
- ApplicationContextAware: 获取 Spring 应用上下文 - ApplicationContext 对象
- EnvironmentAware: 获取 Environment 对象
- ResourceLoaderAware: 获取资源加载器 对象 - ResourceLoader
- BeanClassLoaderAware: 获取加载当前 Bean Class 的 ClassLoader
- BeanNameAware: 获取当前 Bean 的名称
- MessageSourceAware: 获取 MessageSource 对象，用于 Spring 国际化
- ApplicationEventPublisherAware: 获取 ApplicationEventPublishAware 对象，用于 Spring 事件
- EmbeddedValueResolverAware: 获取 StringValueResolver 对象，用于占位符处理

### 10. 依赖注入类型选择

- 低依赖: 构造器注入
- 多依赖: Setter 注入
- 便利性: 字段注入
- 声明类: 方法注入

### 11. 基础类型注入

### 12. 集合类型注入

- Array
- Collection: List, Set
- Map: Properties

### 13. 限定注入

### 14. 延迟依赖注入

- @Lazy
- ObjectFactory
- ObjectProvider

### 15. 依赖处理过程

- 入口 - DefaultListableBeanFactory#resolveDependency
- 依赖描述符 - DependencyDescriptor
- 自定绑定候选对象处理器

### 16. @Autowired 注入原理 `AutowiredAnnotationBeanPostProcessor`

- 元信息解析
- 依赖查找
- 依赖注入 (字段, 方法)

### 17. JSR-330 @Inject 注入原理

```java
public AutowiredAnnotationBeanPostProcessor() {
    this.autowiredAnnotationTypes.add(Autowired.class);
    this.autowiredAnnotationTypes.add(Value.class);
    try {
        this.autowiredAnnotationTypes.add((Class<? extends Annotation>)
        ClassUtils.forName("javax.inject.Inject", AutowiredAnnotationBeanPostProcessor.class.getClassLoader()));
        logger.trace("JSR-330 'javax.inject.Inject' annotation found and supported for autowiring");
    } catch (ClassNotFoundException ex) {
    // JSR-330 API not available - simply skip.
    }
}
```

### 18. Java 通用注解注入原理 `CommonAnnotationBeanPostProcessor`

- javax.xml.ws.WebServiceRef
- javax.ejb.EJB
- javax.annotation.Resource
- javax.annotation.PostConstruct
- javax.annotation.PreDestroy

### 19. 自定义依赖注入注解

- 生命周期处理
  - InstantiationAwareBeanPostProcessor
  - MergedBeanDefinitionPostProcessor
- 元数据
  - InjectedElement
  - InjectionMetadata

### 20. 面试题精选

> 有多少种依赖注入的方式?

- 构造器注入
- Setter 注入
- 字段注入
- 方法注入
- 接口回调注入

> 你偏好构造器注入还是 Setter 注入?

两种依赖注入的方式均可使用, 如果是必须依赖的话, 那么推荐使用构造器注入, Setter 注入用于可选依赖

> Spring 依赖注入的来源有哪些?

resolveDependency()

## Spring IoC 依赖来源

### 1. 依赖查找的来源

- Spring BeanDefinition
  - `<bean id="user" class="..." />`
  - `@Bean`
  - BeanDefinitionBuilder
- Singleton
  - API
- 非 Spring 容器管理对象

---

- Spring 内建 BeanDefinition
  - ConfigurationClassPostProcessor: 处理 Spring 配置类
  - AutowiredAnnotationBeanProcessor: 处理@Autowired 和@Value
  - CommonAnnotationBeanPostProcessor: (Conditional) 处理 JSR-250 注解(@PostConstruct)
  - EventListenerMethodProcessor: 处理标注@EventListener 的 Spring 事件监听方法
- Spring 内建单例对象
  - (environment)Environment: 外部化配置和 Profiles
  - (systemProperties)Properties: Java 系统属性
  - (systemEnvironment)Map: 操作系统环境变量
  - (messageSource)MessageSource: 国际化文案
  - (lifecycleProcessor)LifecycleProcessor: LifecycleBean 处理器
  - (applicationEventMulticaster)ApplicationEventMulticaster: Spring 广播器

### 2. 依赖注入的来源

- BeanDefinition
- 单例对象
- 非 Spring 容器管理对象作为依赖来源

### 3. Spring 容器管理和游离对象

### 4. Spring BeanDefinition 作为依赖来源

- 元数据:BeanDefinition
- 注册:BeanDefinitionRegistry#registerBeanDefinition
- 类型:延迟和非延迟
- 顺序:Bean 生命周期顺序按照注册顺序

### 5. 单例对象作为依赖来源

- 来源:外部普通 Java 对象(不一定是 POJO)
- 注册:SingletonBeanRegistry#registerSingleton
- 无生命周期管理
- 无法实现延迟初始化 Bean

### 6. 非 Spring 容器管理对象作为依赖来源

- 注册: `ConfigurableListableBeanFactory#registerResolvableDependency`
- 无生命周期管理
- 无法实现延迟初始化 Bean
- 无法通过依赖查找

### 7. 外部化配置作为依赖来源

- 非常规 Spring 对象依赖来源
- 无生命周期管理
- 无法实现延迟初始化 Bean
- 无法通过依赖查找

### 8. 面试题精选

> 依赖注入和依赖查找的来源是否相同?

否，依赖查找的来源仅限于 Spring BeanDefinition 以及单例对 象，而依赖注入的来源还包括 Resolvable Dependency 以及 @Value 所标注的外部化配置。

> 单例对象能在 IoC 容器启动后注册吗?

可以的，单例对象的注册与 BeanDefinition 不同，BeanDefinition 会 被 ConfigurableListableBeanFactory#freezeConfiguration() 方法影响， 从而冻结注册，单例对象则没有这个限制。

```java
@Override
public void freezeConfiguration() {
    this.configurationFrozen = true;
    this.frozenBeanDefinitionNames = StringUtils.toStringArray(this.beanDefinitionNames);
}
```

> Spring 依赖注入的来源有哪些?

- Spring BeanDefinition
- 单例对象
- Resolvable Dependency
- @Value 外部化配置

## Spring Bean 作用域

### 1. singleton

默认作用域, 一个 BeanFactory 有且仅有一个实例

### 2. prototype

原型作用域, 每次依赖查找和依赖注入生成新 Bean 对象, Spring 容器没有办法管理 prototypeBean 的完整生命周期, 也没有办法记录实例的存在. 销毁回调方法不会执行, 可以利用 BeanPostProcessor 进行清扫工作.

### 3. request

将 SpringBean 存储在 ServletRequest 上下文中

- XML - `<bean class="..." scope="request" />`
- Java - `@RequestScope` | `@Scope(WebApplicationContext.SCOPE_REQUEST)`

### 4. session

将 SpringBean 存储在 HttpSession 中

- XML - `<bean class="..." scope="session" />`
- Java - `@SessionScope` | `@Scope(WebApplicationContext.SCOPE_SESSION)`

### 5. application

将 SpringBean 存储在 ServletContext 中

- XML - `<bean class="..." scope="application" />`
- Java - `@ApplicationScope` | `@Scope(WebApplicationContext.SCOPE_APPLICATION)`

### 6. 自定义 Bean 作用域

- 实现 Scope: `org.springframework.beans.factory.config.Scope`
- 注册 Scope
  - API: `org.springframework.beans.factory.config.ConfigurableBeanFactory#registerScope`
  - XML:

```xml
<bean class="org.springframework.beans.factory.config.CustomScopeConfigurer">
<property name="scopes">
<map>
<entry key="..."> </entry>
</map>
</property>
</bean>
```

### 7. 面试题

> Spring 内建 Bean 的作用域有几种?

Singleton, prototype, request, session, application, websocket

> Sinleton Bean 是否在一个应用是唯一的?

否, singleton bean 仅在当前 Spring IoC 容器中是单例对象

> 'Application' Bean 是否有其他方案替代?

可以, 实际上, 'application' 和 'singleton' 的 Bean 没有本质区别

## Spring Bean 生命周期

### 1. Spring Bean 元信息配置阶段

- 面向资源
  - XML 配置
  - Properties 资源配置
- 面向注解
- 面向 API

### 2. Spring Bean 元信息解析阶段

- 面向资源 BeanDefinition 解析
  - BeanDefinitionReader
  - XML 解析器 - BeanDefinitionParser
- 面向注解 BeanDefinition 解析
  - AnnotatedBeanDefinitionReader

### 3. Spring Bean 注册阶段

- BeanDefinition 注册接口
  - BeanDefinitionRegistry

### 4. Spring BeanDefinition 合并阶段

- BeanDefinition 合并
  - 父子 BeanDefinition 合并
  - 当前 BeanFactory 查找
  - 层次性 BeanFactory 查找

### 5. Spring Bean Class 加载阶段

- ClassLoader 类加载
- JavaSecurity 安全控制
- ConfigurableBeanFactory 临时 ClassLoader

### 6. Spring Bean 实例化前阶段

`InstantiationAwareBeanPostProcessor#postProcessBeforeInstantiation`

### 7. Spring Bean 实例化阶段

- 传统实例化方式
  - 实例化策略 - InstantiationStrategy
- 构造器依赖注入

### 8. Spring Bean 实例化后阶段

`InstantiationAwareBeanPostProcessor#postProcessAfterInstantiation`

### 9. Spring Bean 属性赋值前阶段

- Bean 属性值元信息
  - PropertyValues
- Bean 属性赋值前回调
  - Spring 1.2 - 5.0:InstantiationAwareBeanPostProcessor#postProcessPropertyValues
  - Spring 5.1:InstantiationAwareBeanPostProcessor#postProcessProperties

### 10. Spring Bean Aware 接口回调阶段

- SpringAware 接口 (先后次序)
  - BeanNameAware
  - BeanClassLoaderAware
  - BeanFactoryAware
  - EnvironmentAware
  - EmbeddedValueResolverAware
  - ResourceLoaderAware
  - ApplicationEventPublisherAware
  - MessageSourceAware
  - ApplicationContextAware

### 11. Spring Bean 初始化前阶段

- 已完成
  - Bean 实例化
  - Bean 属性赋值
  - Bean Aware 接口回调
- 方法回调
  - `BeanPostProcessor#postProcessBeforeInitialization`

### 12. Spring Bean 初始化阶段

- Bean 初始化(Initialization)
  - @PostConstruct 标注方法
  - 实现 InitializingBean 接口的 afterPropertiesSet() 方法
  - 自定义初始化方法

### 13. Spring Bean 初始化后阶段

`BeanPostProcessor#postProcessAfterInitialization`

### 14. Spring Bean 初始化完成阶段

Spring4.1+: `SmartInitializingSingleton#afterSingletonsInstantiated`

### 15. Spring Bean 销毁前阶段

`DestructionAwareBeanPostProcessor#postProcessBeforeDestruction`

### 16. Spring Bean 销毁阶段

- Bean 销毁(Destroy)
  - @PreDestroy 标注方法
  - 实现 DisposableBean 接口的 destroy() 方法
  - 自定义销毁方法

### 17. Spring Bean 垃圾收集

- Bean 垃圾回收(GC)
  - 关闭 Spring 容器(应用上下文)
  - 执行 GC
  - Spring Bean 覆盖的 finalize() 方法被回调

### 18. 面试题

> BeanPostProcessor 的使用场景有哪些?

BeanPostProcessor 提供 SpringBean 初始化前后的生命周期回调, 分别对应 postProcessBeforeInitialization 以及 postProccessorAfterInitialization 方法, 允许对关心的 Bean 进行扩展, 甚至是替换  
其中, ApplicationContext 相关的 Aware 回调也是基于 BeanPostProcessor 实现, 即 ApplicationContextAwareProcessor

> BeanFactoryPostProcessor 与 BeanPostProcessor 的区别

BeanFactoryPostProcessor 是 Spring BeanFactory(实际为 ConfigurableListableBeanFactory) 的后置处理器，用于扩展 BeanFactory，或通过 BeanFactory 进行依赖查找和依赖注入。  
加分项:BeanFactoryPostProcessor 必须有 Spring ApplicationContext 执行，BeanFactory 无法与其直接交互。  
而 BeanPostProcessor 则直接与 BeanFactory 关联，属于 N 对 1 的关系。

> BeanFactory 是怎样处理 Bean 的生命周期?

BeanFactory 的默认实现为 DefaultListableBeanFactory，其中 Bean 生命周期与方法映射如下:

- BeanDefinition 注册阶段 - registerBeanDefinition
- BeanDefinition 合并阶段 - getMergedBeanDefinition
- Bean 实例化前阶段 - resolveBeforeInstantiation
- Bean 实例化阶段 - createBeanInstance
- Bean 初始化后阶段 - populateBean
- Bean 属性赋值前阶段 - populateBean
- Bean 属性赋值阶段 - populateBean
- Bean Aware 接口回调阶段 - initializeBean
- Bean 初始化前阶段 - initializeBean
- Bean 初始化阶段 - initializeBean
- Bean 初始化后阶段 - initializeBean
- Bean 初始化完成阶段 - preInstantiateSingletons
- Bean 销毁前阶段 - destroyBean
- Bean 销毁阶段 - destroyBean
