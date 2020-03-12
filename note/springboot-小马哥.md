# Learn springboot from xiaomage

## 一.关于微服务

### 1. 微服务介绍

>目的: 解决单体应用规模增加时所带来的问题  
>关键字: Monolithic SOA 架构

### 2. 微服务的挑战

> 注册与发现 路由 可靠性 延迟 热点  
> 短路 伸缩 异步 监控 配置 数据同步 安全

jconsole 日志 排查问题

## 二. spring boot web

### 1.标准优化技术

资源变化:

- 响应头: Last-Modified(200)
- 请求头: If-Modified-Since(304)

资源缓存:

- 响应头: ETag
- 请求头: If-None-Match

SpringMvc中的frontController实际上是 DispatcherServlet  
DispatcherServlet -> FrameworkServlet -> HttpServletBean -> HttpServlet  
@Controller实际上可以被称为application controller  
Http Request -> HttpServletServlet.service() -> DispatcherServlet.doDispatch() -> xxController.xx()  
springboot的jar包是fat jar, 是没有压缩率的zip文件  
模版语言是服务端语言, 需要服务端计算 (处理数据强)  
js属于客户端(浏览器)上执行 (动态效果)  
jsp是翻译式语言 jsp -> java -> class  
velocity解释执行 AST解析

### 2.REST理论基础

- 性能(Performance)
- 可伸缩性(Scalability)
- 统一接口简化性(Simplicity of a uniform interface)
- 组件可修改性(Modifiability of components)
- 组件通讯可见性(Visibility of communication between components)
- 组件可移植性(Portability of component)
- 可靠性(Reliability)

### 3.架构约束

- C/S架构(Client-Server)
- 无状态(Stateless)
- 可缓存(Cacheable)
- 分层系统(Layered System)
- 按需代码(Code on demand)
- 统一接口(Uniform interface)(资源识别, 资源操作, 字描述消息, 超媒体)

### 4.Spring Boot REST

#### 核心接口

- 定义相关(@Controller, @RestController)
- 映射相关(@RequestMapping, @PathVariable)
- 请求相关(@RequestParam, @RequestHeader, @CookieValue, RequestEntity)
- 响应相关(@ResponseBody, ResponseEntity)

@RequestParam(value=“aa”) == HttpServletRequest.getParameter(“aa”)  
@GetMapping(value = "abc", consumes=MediaType.APPLICATION_XML_VALUE, produces = MediaType.APPLICATION_XML_VALUE)

- consumes 消费请求头中的Accept
- produces 定义返回值类型

### 5.Servlet

#### 核心接口(since 3.0)

- javax.servlet.annotation.WebServlet
- java.servlet.annotation.WebFilter
- java.servlet.annotation.WebListener
- java.servlet.annotation.ServletSecurity
- java.servlet.annotation.HttpMethodConstraint
- java.servlet.annotation.HttpConstraint
- java.servlet.annotation.WebInitParam

自动装配 -- 初始器 -- javax.servlet.ServletContainerInitializer

#### 生命周期

- 初始化 -- 容器启动或第一次执行, 执行Servlet#init(ServletConfig)
- 处理请求 -- 当http请求到达容器时, 执行Servlet#service(ServletRequest, ServletResponse)
- 销毁 -- 容器关闭时, 执行Servlet#destroy方法, 销毁当前Servlet

#### Filter生命周期

- 初始化 -- 容器启动时, Filter#init(FitlerConfig)执行
- 处理请求 -- 当http请求到达容器时, Filter#doFilter(ServletRequest, ServletResponse, FilterChain)方法被执行, 拦截请求, 在service方法前执行
- 销毁 -- 容器关闭时, 执行Filter#destroy方法, 进行销毁

#### Servlet on Spring Boot

- 组件扫描 -- @org.springframework.boot.web.servlet.ServletComponentScan
- 组件 -- 继承 javax.servlet.HttpServlet implements Servlet
- 标记 -- @javax.servlet.annotation.WebServlet (name, urlPattern)

#### Filter on Spring Boot

- 组件 -- 实现 Filter (OncePerRequestFilter)
- 标记 -- @WebFilter(urlPatterns={""})

## 三. 传统Servlet容器和SpringBoot嵌入式Web容器

### 1.Eclipse Jetty

- Asynchronous HTTP Server
- Standards Based Servlet Container
- websocket server
- http/2 server
- Asynchronous Client(http/1.1, http/2, websocket)
- OSGI, JNDI, JMX, JASPI, AJP Support

### 2.Apache Tomcat

- 核心组件(Components)
- 静态资源处理
- 欢迎页面(Welcome file list)
- JSP处理
- 类加载(ClassLoading)
- 连接器(Connectors) (port, protocol, ThreadPool, Timeout)
- JDBC数据源(DataSource)
- JNDI(JAVA Naming and Directory Interface)

## 四. 数据源 事务 JDBC SpringBoot整合

### 1.数据源

- 通用数据源 (javax.sql.DataSource) (通用型数据库, 本地事务, 一般通过Socket方式连接)
- 分布式数据源 (javax.sql.XADataSource) (通用型数据库, 分布式事务, 一般Socket连接)
- 嵌入式数据源 (org.springframework.jdbc.datasource.embedded.EmbeddedDataSource) (本地文件系统数据库, 如 HSQL, H2, Derby)

### 2.事务 -- 用于提供数据完整性, 并在并发访问下确保数据视图的一致性

- 自动提交模式 (Auto-commit Mode) (触发时机: DML&DDL执行后,SELECT查询后结果集关闭后,存储过程执行后(返回结果集关闭后))
- 事务隔离级别 (Transaction isolation level) (脏读, 不可重现读, 幻影读)
- 保护点 (Savepoints) (部分事务回滚, 选择性释放)

## 五. Mybatis

### 1.配置(mybatis-config.xml)

- 属性(properties)
- 设置(settings)
- 类型别名(typeAlias)
- 类型处理器(typeHandlers)
- 对象工厂(objectFactory)
- 插件(plugins)
- 环境(environments)
- 数据库标识提供商(databaseIdProvider)
- SQL映射文件(mappers)

### 2.核心API

- org.apache.ibatis.session.SqlSessionFactoryBuilder (Builder模式)
- org.apache.ibatis.session.Configuration
- org.apache.ibatis.session.SqlSessionFactory (工厂模式)
- org.apache.ibatis.session.SqlSession (类似于JDBC中的Connection) (封装Connection, 屏蔽Statement的细节, 映射ResultSet到Java类型, 事务控制, 缓存, 代理映射(Mapper))

## 六.JPA

### 1.Java Persistence API

- 实体(entity) -- 轻量级持久化域对象
- 实体类(entity class) (实体类必须使用@Entity标注或XML描述, 至少包含一个默认构造器(public或protected),实体类必须是顶级类(非枚举&接口), 禁止是final类, 实体支持继承&多态关联和多态查询)
- 实体的双向关系是指两个实体之间不仅存在拥有方, 也存在倒装方, 主方决定了更新级联关系到数据库
- @OneToOne, @ManyToOne, @ManyToMany (mappedBy, cascadeType)
- 实体继承 (实体继承抽象类 @Inheritance)

### 2.Spring Data JPA

### 3.SpringBoot & JPA

- SimpleJpaRepository -> JpaRepositoryImplementation -> JpaRepository -> PagingAndSortingRepository -> CrudRepository -> Repository

## 七.NoSql

### 1.NoSQL(通常表示NoSQL数据库, 提供一种存储和获取数据的机制. 数据模型化,非关系数据库中的扁平关系)

- Non SQL
- non relational
- not only SQL

#### 设计目的

- 简化设计
- 简单的水平伸缩
- 细粒度控制

#### 数据结构

- 键值对(key-value) - Redis
- 宽列(wide column) - Hbase
- 图(gragh) - Neo4j
- 文档(document) - Mongodb

#### 相关原理

- CAP: (一致性(Consistency), 可靠性(Availability), 区块容错性(Partition Tolerance))
- ACID: (原子性(Atomicity), 一致性(Consistency), 隔离性(Isolation), 持久性(Durability))

### 2.ElasticSearch

FallOver 故障转移(容灾)

#### Core APIs

- ElasticsearchOperations
- ElasticsearchTemplate
- @EnableElasticsearchRepository
- ElasticsearchRepository

## 八.Java Cache & Redis

### 1. JavaCahce(JSR-107)

缓存是一种久经考验并且显著地提升应用性能以及伸缩性的技术。缓存用作临时存储信息复本，该复本未来可能被再次使用，减少再次加载或创建的成本。

#### 1.非规范目标

- 资源和内存限制配置
- 缓存存储和拓扑结构
- 管理
- 安全
- 外部资源同步

#### 2.核心接口

- CachingProvider
- CacheManager
- Cache
- Entry
- ExpiryPolicy

#### 3.Cache & Map similarity

- 缓存值均有关联键来存储
- 每个值可能仅关联单个键
- 特别注意key的可变性，可变的key可能会影响键的比较
- 自定义Key类应该添加合适的Object.hashCode方法

#### 4.Cache & Map difference

- 缓存的键和值禁止为null
- 缓存项可能会过期
- 缓存项可能被移除
- 缓存支持Compare-And-Swap（CAS）操作
- 缓存的键和值可能需要某种方式的序列化

#### 5.一致性

- 非阻塞锁(nonBlocking) (无法保证, HappenBefore)
- 乐观锁(optimistic locking) (保证, CAS)
- 消极锁 (pessimistic locking) (保证, lock & mutex)

### 2.SpringCache

- org.springframework.cache.CacheManager
- org.springframework.cache.Cache
- @Cacheable
- @CacheEvict
- @CachePut
- @Caching
- @CacheConfig
- @EnableCaching

### Q&A(八)

>缓存更新和失效的策略  

- @CacheEvict
- CacheManager
- Javax.cache.expiry.ExpiryPolicy

>IOC存放BeanDefinition的地方,用的并发map, 为什么加锁  

ConcurrentHashMap只能保证一次操作的原子性操作, 比如要判断大小, 同时获取或者比较等多次操作时, 需要加锁!

>@NoRepositoryBean的作用

SpringData的通用实现中, @NoRepositoryBean用作标记当前接口或类(抽象)不应该作为@RepositoryBean被注册到Spring上下文中, 是因为springData提供了自动代理@RepositoryBean的机制, 该机制的前提是接口或者类(抽象)必须实现Repository接口

## 九. 消息

### 1.Java Message Service (JSR-914)

面向消息中间件 - Message Oriented Middleware

- JMS 提供方（Provider）：实现JMS 接口的MOM
- JMS 客户端（Client）：生产或消费消息的应用或进程
- JMS 生产者（Producer）：创建和发送消息的JMS客户端
- JMS 消费者（Consumer）：接收消息的JMS客户端
- JMS 消息    （Message）：JMS客户端之间的传输数据对象
- JMS 队列    （Queue）：包含待读取消息的准备区域
- JMS 主题    （Topic）：发布消息的分布机制

RPC是同步的, JMS AMQP是异步的

### AMQP (高级消息队列协议)

Kafka 是一种分布式流式计算平台，用于构建实时的数据流水线以及流式计算应用，它是水平伸缩的、容错的、极其快速，并且运行在成千上万的公司的生产环境。

- 发使用容错的方式来存储流式记录
- 发布和订阅流式记录，类似于消息队列或企业消息系统
- 处理流式记录

优势

- 比构建实时的流式计算数据流水线
- 构建实时的流式计算应用

基本概念

- Kafka是集群式运行
- Kafka集群分类存储流式记录，这种分类称为主题(Topic)
- 每条记录包含键(Key), 值(Value)、以及时间戳(Timestamp)

四类核心API

- 生产者 API (能够让应用发布流式记录到一个或多个主题)
- 消费者 API (能够让应用订阅流式记录到一个或多个主题，并且处理他们)
- 流式 API (能够让应用充当流式处理器，消费一个或多个主题的输入流，生产一个或多个主题的输出流，并且高效地将输入流转化成输出流)
- 连接器 API (构建生产者和消费者之间连接)

### 实战

1. 启动zookeeper (zkServer.sh)
2. 启动kafka (kafka-server-start.sh [server.properties])
3. 创建topic (kafka-topic.sh --create --zookeeper localhost:2181 --replication-factor 1 --partition 1 --topic xx)
4. 启动producer (kafka-console-producer.sh --broker-list localhost:9092 --topic xx)
5. 启动consumer (kafka-console-consumer.sh
--bootstrap-server localhost:9092 --topic xx --from-beginning)

通过不同的properties 可以启动不同的kafka

### Q&A(九)

> http invoke()是RPC吗?

是的, 不过它用的HTTP协议, SOAP, WebServices也是HTTP

> IllegalArgumentException & NPE

IAE更能表达参数是非法的, NPQ更多的表达状态是null

## 十.验证

### 1.Apache commons-validator

#### 功能特性

- 可配置的校验引擎
- 可重用的原生校验手段

#### 第三方依赖

- commons-beanutils:1.9.2 (Bean工具类)
- commons-digester:1.8.1
- commons-logging:1.2
- commons-collections:3.2.2

#### 设计模式

- 单例模式（GoF23）
- 校验器模式

#### 验证器类型

- Date 与 Time 校验器
- 数值校验器
- 正则表达式校验器
- ISBN校验器
- IP 地址校验器
- 邮件地址校验器
- URL 校验器
- 域名校验器

### 2.Spring Validator

Spring Framework 提供了用于校验对象的Validator 接口，在校验过程中，与 Errors 对象配合。校验器可以通过Errors 对象报告校验失败的信息。

```java
public interface Validator {

/**
 * Can this {@link Validator} {@link #validate(Object, Errors) validate}
 * instances of the supplied {@code clazz}?
 * <p>This method is <i>typically</i> implemented like so:
 *<pre class="code">return Foo.class.isAssignableFrom(clazz);</pre>
 * (Where {@code Foo} is the class (or superclass) of the actual
 * object instance that is to be {@link #validate(Object, Errors) validated}.)
 * @param clazz the {@link Class} that this {@link Validator} is
 * being asked if it can {@link #validate(Object, Errors) validate}
 * @return {@code true} if this {@link Validator} can indeed
 * {@link #validate(Object, Errors) validate} instances of the
 * supplied {@code clazz}
 */
boolean supports(Class<?> clazz);

/**
 * Validate the supplied {@code target} object, which must be
 * of a {@link Class} for which the {@link #supports(Class)} method
 * typically has (or would) return {@code true}.
 * <p>The supplied {@link Errors errors} instance can be used to report
 * any resulting validation errors.
 * @param target the object that is to be validated
 * @param errors contextual state about the validation process
 * @see ValidationUtils
 */
void validate(Object target, Errors errors);

}
```

### Q&A(十)

> 嵌套bean如何验证

Bean Validation是自动感知嵌套的, 可以层层检测 进行validate

> springboot 对于验证的优先级是怎样的?

LocalValidatorFactoryBean 创建出一个Validator单例, 不存在多个同时校验  
如果需要多个bean, 需要额外处理

## 十一.WebSocket
