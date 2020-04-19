# QUESTIONS OF SPRING

> SpringMVC的Controller是如何将参数和前端传来的数据一一对应的

TODO

> Mybatis如何找到指定的Mapper的, 如何完成查询的

TODO

> Quartz是如何完成定时任务的

TODO

> 自定义注解的实现

TODO

> Spring使用了哪些设计模式

TODO

> Spring的IOC有什么优势

TODO
> Spring如何维护它拥有的bean

TODO
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

AOP（Aspect-OrientedProgramming，面向切面编程）：是OOP的补充和完善。OOP引入了封装、继承、多态性等建立一种对象层次结构（从上到下的关系）。当需要为分散的对象引入公共行为的时候（从左到右的关系），OOP就显得无能为力。例如：日志功能。日志代码往往水平的分散所有对象层次中，与对象的核心功能毫无关系。这种代码被称为横切（cross-cutting）代码还有像安全性、异常处理、透明的持续性等都称为横切代码。在OOP设计中，它们导致了大量代码的重复，不利于模块的重用。
AOP 与 OOP相反，利用“横切”技术将影响多个类的公共行为封装到一个可重用模块，称为 Aspect。简单点，就是将那些与业务无关，却被业务模块所共同调用的逻辑封装起来，便于减少系统的重复代码，降低模块间的耦合度，并有利于未来的可操作性和可维护性。AOP的核心思想就是“将应用程序中的商业逻辑同对其提供支持的通用服务进行分离。”
Spring 提供了两种方式生成代理对象：JDKProxy 和 Cglib具体使用哪种方式生成由 AopProxyFactory根据 AdvisedSupport对象的配置来决定。默认的策略是如果目标类是接口，则使用JDK动态代理技术，否则使用Cglib来生成代理。

```java

public Object getProxy(ClassLoader classLoader) {
   if (logger.isDebugEnabled()) {
      logger.debug("Creating JDK dynamic proxy: target source is " +this.advised.getTargetSource());
   }
   //获取代理类要实现的接口,除了Advised对象中配置的,还会加上SpringProxy, Advised(opaque=false)
   Class[] proxiedInterfaces =AopProxyUtils.completeProxiedInterfaces(this.advised);
   //检查上面得到的接口中有没有定义 equals或者hashcode的接口
   findDefinedEqualsAndHashCodeMethods(proxiedInterfaces);
   //调用Proxy.newProxyInstance创建代理对象
   return Proxy.newProxyInstance(classLoader, proxiedInterfaces, this);
}


//获取可以应用到此方法上的Interceptor列表
List chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method,targetClass);
//如果没有可以应用到此方法的通知(Interceptor)，此直接反射调用 method.invoke(target, args)
if (chain.isEmpty()) {
  retVal = AopUtils.invokeJoinpointUsingReflection(target,method, args);
} else {
  //创建MethodInvocation
  invocation = newReflectiveMethodInvocation(proxy, target, method, args, targetClass, chain);
  retVal = invocation.proceed();
}
```

> 讲讲Spring事务的传播属性

- Required
- Requires_new
- Supports
- Not_supported
- Mandatory
- Nested
- Never

> Spring怎么管理事务

Spring 事务管理主要包括3个接口，Spring 事务主要由以下三个共同完成的：

- PlatformTransactionManager：事务管理器，主要用于平台相关事务的管理。主要包括三个方法：
  - commit：事务提交。
  - rollback：事务回滚。
  - getTransaction：获取事务状态。
- TransacitonDefinition：事务定义信息，用来定义事务相关属性，给事务管理器 PlatformTransactionManager使用这个接口有下面四个主要方法：
  - getIsolationLevel：获取隔离级别。
  - getPropagationBehavior：获取传播行为。
  - getTimeout获取超时时间。
  - isReadOnly：是否只读（保存、更新、删除时属性变为false--可读写，查询时为true--只读）事务管理器能够根据这个返回值进行优化，这些事务的配置信息，都可以通过配置文件进行配置。
- TransationStatus：事务具体运行状态，事务管理过程中，每个时间点事务的状态信息。例如：
  - hasSavepoint()：返回这个事务内部是否包含一个保存点。
  - isCompleted()：返回该事务是否已完成，也就是说，是否已经提交或回滚。
  - isNewTransaction()：判断当前事务是否是一个新事务。

> Spring是如何配置事务的

- xml
- 注解驱动
  - `@EnableTransactionManagement`
  - `@Transactional`

```xml

  <!-- 数据元信息 -->  
  <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">  
  <property name="driverClassName" value="oracle.jdbc.driver.OracleDriver"/>  
  <property name="url" value="jdbc:oracle:thin:@rj-t42:1521:elvis"/>  
  <property name="username" value="root"/>  
  <property name="password" value="root"/>  
  </bean>  
  
  <!-- 管理事务的类,指定我们用谁来管理我们的事务-->  
  <bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">  
  <property name="dataSource" ref="dataSource"/>  
  </bean>
  
  <!-- 首先我们要把服务对象声明成一个bean  例如HelloService -->  
  <bean id="helloService" class="com.yintong.service.HelloService"/>  
  
  <!-- 然后是声明一个事物建议tx:advice,spring为我们提供了事物的封装，这个就是封装在了<tx:advice/>中 -->
  <!-- <tx:advice/>有一个transaction-manager属性，我们可以用它来指定我们的事物由谁来管理。
      默认：事务传播设置是 REQUIRED，隔离级别是DEFAULT -->
  <tx:advice id="txAdvice" transaction-manager="txManager">  
   <!-- 配置这个事务建议的属性 -->  
   <tx:attributes>  
     <!-- 指定所有get开头的方法执行在只读事务上下文中 -->  
     <tx:method name="get*" read-only="true"/>  
     <!-- 其余方法执行在默认的读写上下文中 -->  
     <tx:method name="*"/>  
   </tx:attributes>  
  </tx:advice>  

  <!-- 我们定义一个切面，它匹配FooService接口定义的所有操作 -->  
  <aop:config>  
  <!-- <aop:pointcut/>元素定义AspectJ的切面表示法，这里是表示com.yintong.service.helloService包下的任意方法。 -->
 <aop:pointcut id="helloServiceOperation" expression="execution(* com.yintong.service.helloService.*(..))"/>  
 <!-- 然后我们用一个通知器：<aop:advisor/>把这个切面和tx:advice绑定在一起，表示当这个切面：fooServiceOperation执行时tx:advice定义的通知逻辑将被执行 -->
    <aop:advisor advice-ref="txAdvice" pointcut-ref="helloServiceOperation"/>  
  </aop:config>  

```

> 说说你对Spring的理解. 非单例注入的原理? 它的生命周期? 循环注入的原理, AOP的实现原理, 说说AOP中的几个术语, 它们是怎么相互工作的

【Spring的理解】：【1】Spring 是一个开源框架，主要是为简化企业级应用开发而生。可以实现 EJB可以实现的功能，Spring是一个 IOC和 AOP容器框架。

 ♧ 控制反转（IOC）：Spring 容器使用了工厂模式为我们创建了所需要的对象，我们使用时不需要自己去创建，直接调用Spring为我们提供的对象即可，这就是控制反转的思想。

 ♧ 依赖注入（DI）：Spring 使用 Java Bean对象的Set方法或者带参数的构造方法为我们在创建所需对象时将其属性自动设置所需要的值的过程就是依赖注入的基本思想。

 ♧ 面向切面编程（AOP）：在面向对象编程(OOP)思想中，我们将事物纵向抽象成一个个的对象。而在面向切面编程中，我们将一个个对象某些类似的方面横向抽象成一个切面，对这个切面进行一些如权限验证，事物管理，记录日志等公用操作处理的过程就是面向切面编程的思想。

【2】在 Spring中，所有管理的都是 JavaBean对象，而 BeanFactory和 ApplicationContext就是 Spring框架的那个 IOC容器，现在一般使用 ApplicationContext，其不但包括了 BeanFactory的作用，同时还进行了更多的扩展。

【非单例注入原理】：在大部分情况下，容器中的 bean都是 singleton类型的。如果一个 singleton bean要引用另外一个singleton bean或者一个非 singleton bean要引用另外一个非 singleton，通常情况下将一个 bean定义为另一个 bean的 property值就可以了。不过对于具有不同生命周期的bean来说这样做就会有问题了，比如在调用一个 singleton类型 bean A的某个方法时，需要引用另一个非 singleton（prototype）类型的 bean B，对于 bean A来说，容器只会创建一次，这样就没法在需要的时候每次让容器为bean A 提供一个新的的bean B实例。

> SpringMvc中DispatcherSerlvet初始化过程

TODO
> netty的线程模型, netty如何基于reactor模型上实现的

TODO
> 为什么选择netty

TODO
> 提前加载一个class?

```java
static {
    // Eagerly load the ContextClosedEvent class to avoid weird classloader issues
    // on application shutdown in WebLogic 8.1. (Reported by Dustin Woods.)
    ContextClosedEvent.class.getName();
}
```
