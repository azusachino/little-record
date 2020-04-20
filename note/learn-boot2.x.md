# SpringBoot 2.x

## 第一章 系列总览

### 1.要点

- 组件自动装配: 模式注解, @Enable 模块, 条件装配, 加载机制
- 外部化配置: Environment 抽象, 生命周期, 破坏性变更
- 嵌入式容器: Servlet 容器, Reactive Web 容器
- SpringBootStarter: 依赖管理, 装配条件, 装配顺序

### 2.核心特性

- 自动装配: Web MVC, Web Flux, JDBC 等
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

### 3.传统 Servlet 应用

- Servlet
  - 实现
    - @WebServlet
    - `extends HttpServlet`
    - doGet/doPost
  - URL 映射
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

- Reactor 基础: Java Lambda, Mono, Flux
- 核心: Web MVC 注解, 函数式声明, 异步非阻塞

### 7.WebServer

tomcat, jetty, undertow, netty

### 8.Data

- jdbc: datasource, JdbcTemplate, autoconfigure
- JPA: 实体映射关系, 实体操作, 自动装配
- 事务: Spring 事务抽象, JDBC 事务处理, 自动装配

### 9.事务拓展

- SpringApplication: 失败分析, 应用特性, 事件监听等
- SpringBoot 配置: 外部化配置, profile, 配置属性
- starter 开发, 最佳实践

### 10.运维管理

- 端点: 各类 Web 和 JMX Endpoints
- 健康检查: Health, HealthIndicator
- 指标: 内建 Metrics, 自定义 Metrics

## 第二章 自动装配

### 1.手动装配

- 定义: 一种用于声明在应用中扮演”组件“角色的注解
- 例: @Component, @Service, @Configuraton
- 装配: <context:component-scan> 或 @ComponentScan

### 2.@Enable 模块装配

- 定义: 具备相同领域的功能组件集合, 组合所形成一个独立的单元
- 举例: @EnableWebMvc, @EnableAutoConfiguration
- 实现: 注解方式, 编程方式

### 3.条件装配

- 定义: Bean 装配的前置判断
- 举例: @Profile, @Conditional
- 实现: 注解方式, 编程方式

### 4.自动装配

- 定义: 约定大于配置的原则, 实现 Spring 组件自动装配的目的
- 装配: 模式注解, @Enable 模块, 条件装配, 工厂加载机制
- 实现: 激活自动装配, 实现自动装配, 配置自动装配实现

## 第三章 SpringApplication

### 1. What is SpringApplication

- Spring 模式注解
- Spring 应用上下文
- Spring 工厂加载机制
- Spring 应用上下文初始器
- Spring Environment 抽象
- Spring 应用事件/监听器

### 2.Spring Application

- SpringApplication
- SpringApplicationBuilder API
- SpringApplication Running Listener
- SpringApplication args
- SpringApplication 故障分析
- SpringBoot 应用事件/监听器

### 3.准备阶段

- 定义: Spring 应用引导类, 提供遍历的自定义行为方法
- 场景: 嵌入式 Web 应用和非 Web 应用
- 应用: SpringApplication#run(args...)

### 4.spring 配置 Bean

```java
 public static void main(String[] args) {
        SpringApplication springApplication = new SpringApplication();

        // source ->className, packageName,  xml location
        springApplication.setSources(Collections.singleton(Con.class.getName()));
        ConfigurableApplicationContext configurableApplicationContext = springApplication.run(args);
        configurableApplicationContext.getBean(Con.class.getName());
    }

    @SpringBootApplication
    public static class Con {}
```

### 5. 推断 Web 应用类型

```java
static WebApplicationType deduceFromApplicationContext(Class<?> applicationContextClass) {
    if (isAssignable(SERVLET_APPLICATION_CONTEXT_CLASS, applicationContextClass)) {
        return WebApplicationType.SERVLET;
    }
    if (isAssignable(REACTIVE_APPLICATION_CONTEXT_CLASS, applicationContextClass)) {
      return WebApplicationType.REACTIVE;
    }
    return WebApplicationType.NONE;
}
```

### 6. 推断引导类

```java
private Class<?> deduceMainApplicationClass() {
    try {
      StackTraceElement[] stackTrace = new RuntimeException().getStackTrace();
      for (StackTraceElement stackTraceElement : stackTrace) {
          if ("main".equals(stackTraceElement.getMethodName())) {
              return Class.forName(stackTraceElement.getClassName());
          }
      }
  } catch (ClassNotFoundException ex) {
    // Swallow and continue
    }
    return null;
    }
```

### 7. 加载应用上下文初始器 (ApplicationContextInitializer)

```java
private static Map<String, List<String>> loadSpringFactories(@Nullable ClassLoader classLoader) {
    MultiValueMap<String, String> result = cache.get(classLoader);
    if (result != null) {
      return result;
}

    try {
        Enumeration<URL> urls = (classLoader != null ? classLoader.getResources(FACTORIES_RESOURCE_LOCATION) :
        ClassLoader.getSystemResources(FACTORIES_RESOURCE_LOCATION));
        result = new LinkedMultiValueMap<>();
        while (urls.hasMoreElements()) {
            URL url = urls.nextElement();
            UrlResource resource = new UrlResource(url);
            Properties properties = PropertiesLoaderUtils.loadProperties(resource);
            for (Map.Entry<?, ?> entry : properties.entrySet()) {
            String factoryTypeName = ((String) entry.getKey()).trim();
            for (String factoryImplementationName : StringUtils.commaDelimitedListToStringArray((String) entry.getValue())) {
                result.add(factoryTypeName, factoryImplementationName.trim());
        }
        }
        }
            cache.put(classLoader, result);
            return result;
    }
    catch (IOException ex) {
        throw new IllegalArgumentException("Unable to load factories from location [" + FACTORIES_RESOURCE_LOCATION + "]", ex);
        }
    }
```

### 8. 加载应用事件监听器

ApplicationListener & ApplicationEvent

### 9.运行阶段

`AbstractApplicationContext#refresh()`

```java
 @Override
 public void refresh() throws BeansException, IllegalStateException {
  synchronized (this.startupShutdownMonitor) {
   // Prepare this context for refreshing.
   prepareRefresh();

   // Tell the subclass to refresh the internal bean factory.
   ConfigurableListableBeanFactory beanFactory = obtainFreshBeanFactory();

   // Prepare the bean factory for use in this context.
   prepareBeanFactory(beanFactory);

   try {
    // Allows post-processing of the bean factory in context subclasses.
    postProcessBeanFactory(beanFactory);

    // Invoke factory processors registered as beans in the context.
    invokeBeanFactoryPostProcessors(beanFactory);

    // Register bean processors that intercept bean creation.
    registerBeanPostProcessors(beanFactory);

    // Initialize message source for this context.
    initMessageSource();

    // Initialize event multicaster for this context.
    initApplicationEventMulticaster();

    // Initialize other special beans in specific context subclasses.
    onRefresh();

    // Check for listener beans and register them.
    registerListeners();

    // Instantiate all remaining (non-lazy-init) singletons.
    finishBeanFactoryInitialization(beanFactory);

    // Last step: publish corresponding event.
    finishRefresh();
   } catch (BeansException ex) {
    if (logger.isWarnEnabled()) {
     logger.warn("Exception encountered during context initialization - " +
       "cancelling refresh attempt: " + ex);
    }

    // Destroy already created singletons to avoid dangling resources.
    destroyBeans();

    // Reset 'active' flag.
    cancelRefresh(ex);

    // Propagate exception to caller.
    throw ex;
   } finally {
    // Reset common introspection caches in Spring's core, since we
    // might not ever need metadata for singleton beans anymore...
    resetCommonCaches();
   }
  }
 }
```

- 创建: 应用上下文, Environment
- 失败: 故障分析报告
- 回调: CommandLineRunner, ApplicationRunner

监听方法

- starting()
- environmentPrepared(ConfigurableEnvironment)
- contextPrepared(ConfigurableApplicationContext)
- contextLoaded(ConfigurableApplicationContext)
- started(ConfigurableApplicationContext)
- running(ConfigurableApplicationContext)
- failed(ConfigurableApplicationContext,Throwable)

阶段说明

- Spring 应用刚启动
- ConfigurableEnvironment 准备妥当，允许将其调整
- ConfigurableApplicationContext 准备妥当，允许将其调整
- ConfigurableApplicationContext 已装载，但仍未启动
- ConfigurableApplicationContext 已启动，此时 Spring Bean 已初始化完成
- Spring 应用正在运行
- Spring 应用运行失败

Spring Boot 事件 `ApplicationListener<xxxEvent>`

- ApplicationStartingEvent
- ApplicationEnvironmentPreparedEvent
- ApplicationPreparedEvent
- ApplicationStartedEvent
- ApplicationReadyEvent
- ApplicationFailedEvent

---

拓展SpringBoot: 注意`Ordered` | `@Order`接口 (加载顺序)

## 第四章 Web MVC 核心

- 理解 Spring Web MVC 架构
  - Servlet
    - 请求/响应式
    - 屏蔽网络通讯的细节
    - 面向HTTP协议
    - 完整生命周期
  - 指责
    - 处理请求 (DispatcherServlet)
    - 资源管理 (数据库连接, 消息连接, 其他)
    - 视图渲染 (ViewResolver)

- 认识 Spring Web MVC

- 实现Controller
- 配置WebMVC组件
- 配置DispatcherServlet
- 使用可执行Tomcat Maven插件

```java
@Controller
public class HelloWorldController {
    @RequestMapping("")
    public String index() {
        return "index";
    }
}
```

- 简化 Spring Web MVC

## 第五章 Web MVC 视图应用

### 新一代服务端模板引擎 Thymeleaf

```html
<tr>
<th th:text="#{name}">Name</th>
<th th:text="#{msg}">Msg</th>
</tr>
```

#### 核心要素

- 资源定位: 模板来源
  - 通用资源抽象
    - 文件资源: File
    - ClassPath资源: ClassLoader
    - 统一资源: URL
    - Web资源: ServletContext
  - Spring资源抽象
    - Spring资源: Resource
- 渲染上下文: 变量来源
  - Context: Thymeleaf渲染上下文
  - Model: SpringWebMVC模型
  - Attribute: Servlet上下文
- 模板引擎: 模板渲染
  - ITemplateEngine实现
    - TemplateEngine: Thymeleaf原生实现
    - SpringTemplateEngine: Spring实现
    - SpringWebFluxTemplateEngine: Spring WebFlux实现

```java
    public static void main(String[] args) {
        SpringTemplateEngine templateEngine = new SpringTemplateEngine();
        // 渲染上下文
        Context context = new Context();

        context.setVariable("message", "hello world");

        String result = "<p th:text=\"${message}\"></p>";

        System.out.println(templateEngine.process(result, context));
    }
```

#### 视图处理

- SpringWebMVC视图组件
  - ViewResolver: ThymeleafViewResolver
  - View: ThymeleafView
  - DispatcherServlet
  - ITemplateEngine: SpringTemplateEngine

#### 核心组件

- 视图解析
  - ContentNegotiatingViewResolver
    - InternalResourceViewResolver
    - BeanNameViewResolver
    - ThymeleafViewResolver
- 配置策略
  - 配置Bean: WebMvcConfigurer
  - 配置对象: ContentNegotiationConfigurer
- 策略管理
  - Bean: ContentNegotiationManager
  - FactoryBean: ContentNegotiationManagerFactoryBean
- 策略实现
  - ContentNegotationStrategy
    - 固定`MediaType`: FixedContentNegotiationStrategy
    - "Accept"请求头: HeaderContentNegotiationStrategy
    - 请求参数: ParameterCOntentNegotiationStrategy
    - 路径拓展名: PathExtensionContentNegotiationStrategy

---

- 理解  BeanNameViewResolver
- 理解HTTP Accept 请求头 与  View Content-Type 匹配
- 理解最佳  View  匹配规则
  - ViewResolver  优先规则
    - 自定义 - InternalResourceViewResolver
    - ThymeleafViewResolver
    - 默认 - InternalResourceViewResolver
  - MediaType  匹配规则
    - Accept头策略(Accept:text/html)
    - 请求参数策略(format=text/xml)

## 第六章 Web MVC REST 应用

### REST简介

REST = RESTful = Representational State Transfer，is one way of providing interoperability between computer systems on the Internet.

#### 架构约束

- 统一接口 (Uniform Interface)
- C/S架构 (Client/Server)
- 无状态 (Stateless)
- 可缓存 (Cacheable)
- 封层系统 (Layered System)
- 按需代码 (Code On Demand)

统一接口(Uniform interface):

- 资源识别(Identification of resources)
  - URI(Uniform Resource Identifier )
- 资源操作(Manipulation of resources through representations)
  - HTTP verbs:GET、PUT、POST、DELETE
- 自描述消息(Self-descriptive messages)
  - Content-Type
  - MIME-Type
  - Media Type: application/javascript、 text/html
- 超媒体(HATEOAS)
  - Hypermedia As The Engine Of Application State

### Web MVC REST支持

- 定义: `@Controller`, `@RestController`
- 映射: `@RequestMapping`, `@*Mapping`
- 请求: `@RequestParam`, `@RequestHeader`, `@CookieValue`, `@RequestBody`, `@PathVariable`, `RequestEntity`
- 相应: `@ResponseBody`, `ResponseEntity`, `ResponseCookie`
- 拦截: `@RestControllerAdvice`, `HandlerInterceptor`
- 跨域: `@CrossOrigin`, `CorsFilter`, `WebMvcConfigurer#addCorsMappings`

### Rest 内容协商

- 内容协商管理器: ContentNegotiationManager
- 媒体类型: MediaType
- 消费媒体类型: @RequestMapping#consumes
- 生产媒体类型: @RequestMapping#produces
- HTTP消息转换器: HttpMessageConverter
- Web MVC 配置器: WebMvcConfigurer
- 处理方法: HandlerMethod
- 处理方法参数解析器: HandlerMethodArgumentResolver
- 处理方法返回值解析器: HandlerMethodReturnValueHandler

![mvc](../resources/spring/web_mvc.png)

---

![converter](../resources/spring/converter.png)

### CORS

- `@CorssOrigin`
- `WebMvcConfiguer#addCorsMappings`

## 第七章 Servlet

### Servlet简介

核心组件API

- javax.servlet.Servlet - DispatcherServlet
- javax.servlet.Filter - CharacterEncodingFilter
- javax.servlet.ServletContext
- javax.servlet.AsyncContext
- javax.servlet.ServletContentListener
- javax.servlet.ServletRequestListener
- javax.servlet.HttpSessionListener
- javax.servlet.AsyncListener
- javax.servlet.ServletContainerIntializer

Servlet组件注册

- *Servlet
  - web.xml -> `<servlet> + <servlet-mapping>`
  - `@WebServlet`
  - `ServletContext#addServlet`
- *Filter
  - web.xml -> `<filter> + <filter-mapping>`
  - `@WebFilter`
  - `ServletContext#addFilter`
- *Listener
  - web.xml -> `<listener>`
  - `@WebListener`
  - `ServletContext#addListener`

### Spring Servlet Web

Servlet生命周期

1. 初始化: init(ServletConfig)
2. 服务: service(ServletRequest, ServletResponse)
3. 销毁: destroy()

Filter生命周期

1. 初始化: init(FilterConfig)
2. 服务: doFilter(ServletRequest,ServletResponse,FilterChain)
3. 销毁: destroy()

ServletContext生命周期

1. 初始化: contextInitialized(ServletContextEvent)
2. 销毁: contextInitialized(ServletContextEvent)

Servlet异步支持

- DeferredResult
- Callable
- CompletionStage

### Spring Boot Servlet Web

- ServletContextInitializer
  - RegistrationBean
    - ServletListenerRegistrationBean
      - @WebListener
    - FilterRegistrationBean
      - @WebFilter
    - ServletRegistrationBean
      - @WebServlet

@ServletComponentScan  扫描package ->  @Web* ->  RegistrationBean Bean 定义->  RegistrationBean Bean

### Servlet容器部署Spring Boot

- 基础接口：WebApplicationInitializer
- 编程驱动：AbstractDispatcherServletInitializer
- 注解驱动：AbstractAnnotationConfigDispatcherServletInitializer

## 第八章 Reactive到WebFlux

- 反应堆模式 (Reactor)
- Proactor模式
- 观察者模式
- 迭代器模式 (Iterator)
- Java并发模型

### 理解Reactive

- Reactive 是异步非阻塞编程
- Reactive 能够提升程序性能
- Reactive 解决传统编程模型遇到的困境

---

- Java9 Flow API
- RxJava
- Reactor

Future#get() 方法不得不等待任务执行完成，换言之，如果多个任务提交后，返回的多个 Future 逐一调用 get() 方法时，将会依次 blocking，任务的执行从并行变为串行。

### Reactive Programming 定义

- 响应的 (Responsive)
- 适应性强的 (Resilient)
- 弹性的 (Elastic)
- 消息驱动的 (Message Driven)
- 数据流 (Data Stream)
- 传播变化 (Propagation of Change)
- 变化响应 (Reacting to change)
- 非阻塞 (Non-Blocking)
- 观察者模式(Observer pattern )
- 数据/事件序列(Sequences of data and/or events )
- 序列操作符(Opeators)
- 屏蔽并发细节(abstracting away...)
- 观察者模式(Observer pattern )
- 响应流模式(Reactive streams pattern )
- 迭代器模式(Iterator pattern)
- 拉模式(pull-based)
- 推模式(push-based)
- 异步(asynchronous )
- 数据流(data streams)
- 并非新鲜事物(not anything new)
- 过于理想化(idea on steroids)

Reactive Programming:同步或异步非阻塞执行，数据传播被动通知  
Imperative programming:同步阻塞执行，数据主动获取

---

### 设计模式

- 扩展模式:观察者(Observer)
  - 推模式(push-based)
- 混合模式:反应堆(Reactor)、Proactor
- 对立模式:迭代器(Iterator)
  - 拉模式(pull-based)

---

Reactive Programming 作为观察者模式(Observer) 的延伸，不同于传统的命令编程方式( Imperative programming)同步拉取数据的方式，如迭代器模式(Iterator) 。而是采用数据发布者同步或异步地推 送到数据流(Data Streams)的方案。当该数据流(Data Steams)订阅者监听到传播变化时，立即作出 响应动作。在实现层面上，Reactive Programming 可结合函数式编程简化面向对象语言语法的臃肿性， 屏蔽并发实现的复杂细节，提供数据流的有序操作，从而达到提升代码的可读性，以及减少 Bugs 出现的 目的。同时，Reactive Programming 结合背压(Backpressure)的技术解决发布端生成数据的速率高于 订阅端消费的问题。

### React Streams

Reactive Streams is a standard and specification for Stream-oriented libraries for the JVM that process a potentially unbounded number of elementsin sequence, asynchronously passing elements between components, with mandatory non-blocking backpressure.

- Publisher
- Subscriber
- Subscription
- Processor

### Reactor API

- Mono: 0-1的非阻塞结果
- Flux: 0-N的非阻塞序列
- Scheduler: Reactor调度线程池

## 第九章 WebFlux核心

### 编程模型

- 注解驱动
- 函数式端点

映射路由接口 - RouterFunction

```java
RouterFunction<ServerResponse> route = route()
    .GET("/person/{id}", accept(APPLICATION_JSON), handler::getPerson)
    .GET("/person", accept(APPLICATION_JSON), handler::listPeople)
    .POST("/person", handler::createPerson)
    .build();
```

### 核心组件 API

HttpHandler

```java
public interface HttpHandler {

  /**
   * Handle the given request and write to the response.
   * @param request current request
   * @param response current response
   * @return indicates completion of request handling
   */
    Mono<Void> handle(ServerHttpRequest request, ServerHttpResponse response);

}
}
```

WebHandler

```java
public interface WebHandler {

    /**
     * Handle the web server exchange.
     * @param exchange the current server exchange
     * @return {@code Mono<Void>} to indicate when request handling is complete
     */
    Mono<Void> handle(ServerWebExchange exchange);
}
```

### 使用场景

#### 性能使用性

Performance has many characteristics and meanings. Reactive and non-blocking generally do not make applications run faster. They can, in some cases, (for example, if using the WebClient to execute remote calls in parallel). On the whole, it requires more work to do things the non-blocking way and that can increase slightly the required processing time.

#### 编程模型适用性

#### 并发模型适用性

## 第十章 超越外部化配置

### 理解"外部化配置"

- XML Bean定义的属性占位符
- @Value注入
- Environment读取
- @ConfigurationProperties绑定
- @ConditionalOnProperty判断

### PropertySource顺序

1. Devtools global settings properties in the $HOME/.config/spring-boot folder when devtools is active.
2. `@TestPropertySource` annotations on your tests.
3. properties attribute on your tests. Available on `@SpringBootTest` and the test annotations for testing a particular slice of your application.
4. Command line arguments.
5. Properties from SPRING_APPLICATION_JSON (inline JSON embedded in an environment variable or system property).
6. ServletConfig init parameters.
7. ServletContext init parameters.
8. JNDI attributes from java:comp/env.
9. Java System properties (System.getProperties()).
10. OS environment variables.
11. A RandomValuePropertySource that has properties only in random.*.
12. Profile-specific application properties outside of your packaged jar (application-{profile}.properties and YAML variants).
13. Profile-specific application properties packaged inside your jar (application-{profile}.properties and YAML variants).
14. Application properties outside of your packaged jar (application.properties and YAML variants).
15. Application properties packaged inside your jar (application.properties and YAML variants).
16. `@PropertySource` annotations on your @Configuration classes. Please note that such property sources are not added to the Environment until the application context is being refreshed. This is too late to configure certain properties such as logging.* and spring.main.* which are read before refresh begins.
17. Default properties (specified by setting SpringApplication.setDefaultProperties).

### XML Bean定义属性占位符

### @Value

- @Value字段注入
- @Value方法注入
- @Value默认值支持

### Environment读取

- 获取EnvironmentBean
- Environment方法/构造器注入
- Environment @Autowired依赖注入
- EnvironmentAware接口回调
- BeanFactory依赖查找Environment

执行顺序:  

1. @Autowired
2. BeanFactoryAware
3. EnvironmentAware

### @ConfigurationProperties Bean 绑定

```java
    @Bean
    @ConfigurationProperties("user")
    public Person p4() {
        return new Person();
    }
```

### @ConditionalOnProperty

`@ConditionalOnProperty(value = "person.id", matchIfMissing = true, havingValue = "123433")`

### Test相关

1. `@TestPropertySource#properties`
2. `@SpringBootTest#properties`
3. `@TestPropertySource#locations`

### PropertySource

带有名称的属性源, Properties文件, Map, YAML文件

### Environment抽象

Environment和PropertySource是1对1, PropertySources与PropertySource是1对N

### 拓展外部化配置属性源

1. `org.springframework.boot.SpringApplicationRunListener#environmentPrepared()`
2. `org.springframework.boot.SpringApplicationRunListener#contextPrepared()`
3. `org.springframework.boot.SpringApplicationRunListener#contextLoaded()`
4. `org.springframework.boot.context.event.ApplicationEnvironmentPreparedEvent`
5. `org.springframework.boot.env.EnvironmentPostProcessor`
6. `org.springframework.context.ApplicationContextInitializer`
7. `org.springframework.boot.context.event.ApplicationPreparedEvent`
