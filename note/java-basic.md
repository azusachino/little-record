# JAVA NOTE

## java中锁的结构图

> ![结构图](../resources/java/lock.png)

## JAVA各版本特性

- 1.0(1996) 初始版本
- 1.1(1997) JDBC, 支持内部类, RMI, 反射, Java Bean
- 1.2(1998) 集合框架, JIT编译器, 对打包的java文件进行数字签名, JFC, java插件, JDBC可滚动结果集, Applet添加声音支持
- 1.3(2000) Java Sound Api, jar文件索引
- 1.4(2002) XML处理, java打印服务, Logging API, JAVA Web Start, JDBC 3.0 API, 断言, Preferences API, 链式异常处理, 支持IPV6, 支持正则表达式, 引入Image I/O API
- 1.5(2004) 泛型, 增强循环(迭代), 自动装箱与自动拆箱, 类型安全的枚举, 可变参数, 静态引入, 元数据(注解), Instrumentation
- 1.6(2006) 支持脚本语言, JDBC 4.0 API, Java Compiler API, 可插拔注解, 支持Native PKI & Java GSS & Kerberos & LDAP, 继承Web Service
- 1.7(2011) switch语句允许以字符串作为条件, 创建泛型对象时应用类型推断, 在一个语句块中捕捉多种异常, 支持动态语言, try-with-resource, 引入Java NIO.2, 数值类型可以用二进制字符串表示(下划线), 钻石型语法, null值自动处理
- 1.8(2014) lambda表达式, 方法应用(FunctionInterface), 默认方法, 新工具, Stream API, DateTime API, Optional, Nashorn(JavaScript)引擎
- 1.9(2017) 模块系统, REPL, HTTP2, 改进的JavaDoc, 多版本兼容jar包, 集合工厂方法, 私有接口方法, 进程API, 改进的StreamAPI, 改进try-with-resource, 改进的@Deprecated, 改进钻石操作符, 改进Optional类, 多分辨率图像API, 改进的CompletableFuture API, 轻量级的JSON API, 响应式流API
- 1.10(2018) var局部变量类型推断, 统一的垃圾回收接口, G1垃圾回收器的并行完整垃圾回收
- 1.11(2018) ...
- 1.12(2019) switch语句扩展
- 1.13(2020) ...

## 双亲委派

>双亲委派：如果一个类加载器收到了加载某个类的请求,则该类加载器并不会去加载该类,而是把这个请求委派给父类加载器,每一个层次的类加载器都是如此,因此所有的类加载请求最终都会传送到顶端的启动类加载器;只有当父类加载器在其搜索范围内无法找到所需的类,并将该结果反馈给子类加载器,子类加载器会尝试去自己加载。  

### 加载流程

1. 启动类加载器（Bootstrap ClassLoader）：C++实现，在java里无法获取，负责加载/lib下的类。
2. 扩展类加载器（Extension ClassLoader）： Java实现，可以在java里获取，负责加载/lib/ext下的类。
3. 系统类加载器/应用程序类加载器（Application ClassLoader）：是与我们接触对多的类加载器，我们写的代码默认就是由它来加载，ClassLoader.getSystemClassLoader返回的就是它。  

### 加载器

- 启动类加载器（Bootstrap ClassLoader）：C++实现，在java里无法获取，负责加载/lib下的类。
- 扩展类加载器（Extension ClassLoader）： Java实现，可以在java里获取，负责加载/lib/ext下的类。
- 系统类加载器/应用程序类加载器（Application ClassLoader）：是与我们接触对多的类加载器，我们写的代码默认就是由它来加载，ClassLoader.getSystemClassLoader返回的就是它。  
![classLoader](../resources/java/classLoader.png)

## 破坏双亲委派

### 原因

因为在某些情况下父类加载器需要委托子类加载器去加载class文件。受到加载范围的限制，父类加载器无法加载到需要的文件，以Driver接口为例，由于Driver接口定义在jdk当中的，而其实现由各个数据库的服务商来提供，比如mysql的就写了MySQL Connector，那么问题就来了，DriverManager（也由jdk提供）要加载各个实现了Driver接口的实现类，然后进行管理，但是DriverManager由启动类加载器加载，只能记载JAVA_HOME的lib下文件，而其实现是由服务商提供的，由系统类加载器加载，这个时候就需要启动类加载器来委托子类来加载Driver实现，从而破坏了双亲委派。

### 实现

![Driver实现](../resources/java/classLoader-practice.png)

## 讲讲HashMap

### 顺序

- 1.7和1.8的数据结构
- 构造方法引出几个参数
- 1.7和1.8解决hash冲突的几种思路
- resize和rehash
- 用ConcurrentHashMap(线程安全性结尾)

## @Transactional

### 可以作用在哪些地方

- @Transactional 可以作用在接口、类、类方法。
  - 作用于类：当把@Transactional 注解放在类上时，表示所有该类的public方法都配置相同的事务属性信息。
  - 作用于方法：当类配置了@Transactional，方法也配置了@Transactional，方法的事务会覆盖类的事务配置信息。
  - 作用于接口：不推荐这种使用方法，因为一旦标注在Interface上并且配置了Spring AOP 使用CGLib动态代理，将会导致@Transactional注解失效

### 有哪些属性

@Transactional注有哪些属性？

- propagation属性
  - propagation 代表事务的传播行为，默认值为 Propagation.REQUIRED，其他的属性信息如下：
  - Propagation.REQUIRED：如果当前存在事务，则加入该事务，如果当前不存在事务，则创建一个新的事务。( 也就是说如果A方法和B方法都添加了注解，在默认传播模式下，A方法内部调用B方法，会把两个方法的事务合并为一个事务 ）
  - Propagation.SUPPORTS：如果当前存在事务，则加入该事务；如果当前不存在事务，则以非事务的方式继续运行。
  - Propagation.MANDATORY：如果当前存在事务，则加入该事务；如果当前不存在事务，则抛出异常。
  - Propagation.REQUIRES_NEW：重新创建一个新的事务，如果当前存在事务，暂停当前的事务。( 当类A中的 a 方法用默认Propagation.REQUIRED模式，类B中的 b方法加上采用 Propagation.REQUIRES_NEW模式，然后在 a 方法中调用 b方法操作数据库，然而 a方法抛出异常后，b方法并没有进行回滚，因为Propagation.REQUIRES_NEW会暂停 a方法的事务 )
  - Propagation.NOT_SUPPORTED：以非事务的方式运行，如果当前存在事务，暂停当前的事务。
  - Propagation.NEVER：以非事务的方式运行，如果当前存在事务，则抛出异常。
  - Propagation.NESTED ：和 Propagation.REQUIRED 效果一样。
- isolation 属性
  - isolation ：事务的隔离级别，默认值为 Isolation.DEFAULT。
  - Isolation.DEFAULT：使用底层数据库默认的隔离级别。
  - Isolation.READ_UNCOMMITTED
  - Isolation.READ_COMMITTED
  - Isolation.REPEATABLE_READ
  - Isolation.SERIALIZABLE
- timeout 属性
  - timeout ：事务的超时时间，默认值为 -1。如果超过该时间限制但事务还没有完成，则自动回滚事务。
- readOnly 属性
  - readOnly ：指定事务是否为只读事务，默认值为 false；为了忽略那些不需要事务的方法，比如读取数据，可以设置 read-only 为 true。
- rollbackFor 属性
  - rollbackFor ：用于指定能够触发事务回滚的异常类型，可以指定多个异常类型。
- noRollbackFor属性
  - noRollbackFor：抛出指定的异常类型，不回滚事务，也可以指定多个异常类型。
  