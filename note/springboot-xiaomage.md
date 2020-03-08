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
