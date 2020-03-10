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
