# SpringBoot 2.0

## 系列总览

### 1.要点

- 组件自动装配: 模式注解, @Enable模块, 条件装配, 加载机制
- 外部化配置: Environment抽象, 生命周期, 破坏性变更
- 嵌入式容器: Servlet容器, Reactive Web容器
- SpringBootStarter: 依赖管理, 装配条件, 装配顺序

### 2.核心特性

- 自动装配: Web MVC, Web Flux, JDBC等
- 嵌入式容器: Tomcat, Jetty, Undertow
- 生产准备特性: 指标, 健康检查, 外部化配置等

#### 自动装配(auto configure)

1. 激活: @EnableAutoConfiguration
2. 配置: /META-INF/spring.factories
3. 实现: xxAutoConfiguration

#### 嵌入式容器

1. tomcat
2. netty

#### 生产准备特性

1. 指标: /actuator/metrics
2. 健康检查: /actuator/health
3. 外部化配置: /actuator/configprops

### 3.传统Servlet应用

- Servlet
  - 实现
    - @WebServlet
    - `extends HttpServlet`
    - doGet/doPost
  - URL映射
    - @WebServlet(urlPattern="/")
  - 注册
    - @ServletComponentScan(basePackage="xxx/xxx")
- Filter
- Listener

### 4. 异步非阻塞

```java
@WebServlet(urlPattern="/", asyncSupported=true)

AsyncContext asyncContext = req.startAsync()
asyncContext.start(() -> {})
// trigger complete
asyncContext.complete();
```

### 5.Spring MVC

- 视图; 模版引擎, 内容协商, 异常处理
- REST: 资源服务, 资源跨域, 服务发现
- Core: 核心架构, 处理流程, 核心组件

### 6.Spring Web Flux

- Reactor基础: Java Lambda, Mono, Flux
- 核心: Web MVC注解, 函数式声明, 异步非阻塞

### 7.WebServer

tomcat, jetty, undertow, webflux

### 8.Data

- jdbc: datasource, JdbcTemplate, autoconfigure
- JPA: 实体映射关系, 实体操作, 自动装配
- 事务: Spring事务抽象, JDBC事务处理, 自动装配

### 9.事务拓展

- SpringApplication: 失败分析, 应用特性, 事件监听等
- SpringBoot配置: 外部化配置, profile, 配置属性
- starter开发, 最佳实践

### 10.运维管理

- 端点: 各类Web和JMX Endpoints
- 健康检查: Health, HealthIndicator
- 指标: 内建Metrics, 自定义Metrics

## 自动装配

### 1.手动装配

- 定义: 一种用于声明在应用中扮演”组件“角色的注解
- 例: @Component, @Service, @Configuraton
- 装配: <context:component-scan> 或 @ComponentScan

### 2.@Enable模块装配

- 定义: 具备相同领域的功能组件集合, 组合所形成一个独立的单元
- 举例: @EnableWebMvc, @EnableAutoConfiguration
- 实现: 注解方式, 编程方式

### 3.条件装配

- 定义: Bean装配的前置判断
- 举例: @Profile, @Conditional
- 实现: 注解方式, 编程方式

### 4.自动装配

- 定义: 约定大于配置的原则, 实现Spring组件自动装配的目的
- 装配: 模式注解, @Enable模块, 条件装配, 工厂加载机制
- 实现: 激活自动装配, 实现自动装配, 配置自动装配实现
