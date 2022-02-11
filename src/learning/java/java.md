# Java

## JAVA 各版本特性

- 1.0(1996) 初始版本
- 1.1(1997) JDBC, 支持内部类, RMI, 反射, Java Bean
- 1.2(1998) 集合框架, JIT 编译器, 对打包的 java 文件进行数字签名, JFC, java 插件, JDBC 可滚动结果集, Applet 添加声音支持
- 1.3(2000) Java Sound Api, jar 文件索引
- 1.4(2002) XML 处理, java 打印服务, Logging API, JAVA Web Start, JDBC 3.0 API, 断言, Preferences API, 链式异常处理, 支持 IPV6, 支持正则表达式, 引入 Image I/O API
- 1.5(2004) 泛型, 增强循环(迭代), 自动装箱与自动拆箱, 类型安全的枚举, 可变参数, 静态引入, 元数据(注解), Instrumentation
- 1.6(2006) 支持脚本语言, JDBC 4.0 API, Java Compiler API, 可插拔注解, 支持 Native PKI & Java GSS & Kerberos & LDAP, 继承 Web Service
- 1.7(2011) switch 语句允许以字符串作为条件, 创建泛型对象时应用类型推断, 在一个语句块中捕捉多种异常, 支持动态语言, try-with-resource, 引入 Java NIO.2, 数值类型可以用二进制字符串表示(下划线), 钻石型语法, null 值自动处理
- 1.8(2014) lambda 表达式, 方法引用(`FunctionInterface`), 默认方法(default), 新工具类(Objects,Collections), Stream API, `LocalDateTime` API, `Optional<T>`, Nashorn(JavaScript)引擎
- 1.9(2017) 模块系统, REPL, HTTP2, 改进的 JavaDoc, 多版本兼容 jar 包, 集合工厂方法, 私有接口方法, 进程 API, 改进的 StreamAPI, 改进 try-with-resource, 改进的@Deprecated, 改进钻石操作符, 改进 Optional 类, 多分辨率图像 API, 改进的 CompletableFuture API, 轻量级的 JSON API, 响应式流 API
- 1.10(2018) var 局部变量类型推断, 统一的垃圾回收接口, G1 垃圾回收器的并行完整垃圾回收
- 1.11(2018) ...
- 1.12(2019) switch 语句扩展
- 1.13(2020) ...

## 双亲委派

双亲委派：如果一个类加载器收到了加载某个类的请求,则该类加载器并不会去加载该类,而是把这个请求委派给父类加载器,每一个层次的类加载器都是如此,因此所有的类加载请求最终都会传送到顶端的启动类加载器;只有当父类加载器在其搜索范围内无法找到所需的类,并将该结果反馈给子类加载器,子类加载器会尝试去自己加载。

### 加载器

- 启动类加载器（Bootstrap ClassLoader）：C++实现，在 java 里无法获取，负责加载/lib 下的类。
- 扩展类加载器（Extension ClassLoader）： Java 实现，可以在 java 里获取，负责加载/lib/ext 下的类。
- 系统类加载器/应用程序类加载器（Application ClassLoader）：是与我们接触对多的类加载器，我们写的代码默认就是由它来加载，ClassLoader.getSystemClassLoader 返回的就是它。

## 破坏双亲委派

### 原因

在某些情况下父类加载器需要委托子类加载器去加载 class 文件。受到加载范围的限制，父类加载器无法加载到需要的文件，以 Driver 接口为例，由于 Driver 接口定义在 jdk 当中，而其实现由各个数据库的服务商来提供，比如 mysql 就写了 MySQL Connector，那么问题就来了，DriverManager（也由 jdk 提供）要加载各个实现了 Driver 接口的实现类，然后进行管理，但是 DriverManager 由启动类加载器加载，只能记载 JAVA_HOME 的 lib 下文件，而其实现是由服务商提供的，由系统类加载器加载，这个时候就需要启动类加载器来委托子类来加载 Driver 实现，从而破坏了双亲委派。

## 讲讲 HashMap

### 顺序

- 1.7 和 1.8 的数据结构
- 构造方法引出几个参数
- 1.7 和 1.8 解决 hash 冲突的几种思路
- resize 和 rehash
- 用 ConcurrentHashMap(线程安全性结尾)

## @Transactional

### 可以作用在哪些地方

- @Transactional 可以作用在接口、类、类方法。
  - 作用于类：当把@Transactional 注解放在类上时，表示所有该类的 public 方法都配置相同的事务属性信息。
  - 作用于方法：当类配置了@Transactional，方法也配置了@Transactional，方法的事务会覆盖类的事务配置信息。
  - 作用于接口：不推荐这种使用方法，因为一旦标注在接口上并且配置了 Spring AOP 使用 CGLib 动态代理，将会导致@Transactional 注解失效

### 有哪些属性

@Transactional 注有哪些属性？

- propagation 属性
  - propagation 代表事务的传播行为，默认值为 Propagation.REQUIRED，其他的属性信息如下：
  - Propagation.REQUIRED：如果当前存在事务，则加入该事务，如果当前不存在事务，则创建一个新的事务。( 也就是说如果 A 方法和 B 方法都添加了注解，在默认传播模式下，A 方法内部调用 B 方法，会把两个方法的事务合并为一个事务 ）
  - Propagation.SUPPORTS：如果当前存在事务，则加入该事务；如果当前不存在事务，则以非事务的方式继续运行。
  - Propagation.MANDATORY：如果当前存在事务，则加入该事务；如果当前不存在事务，则抛出异常。
  - Propagation.REQUIRES_NEW：重新创建一个新的事务，如果当前存在事务，暂停当前的事务。( 当类 A 中的 a 方法用默认 Propagation.REQUIRED 模式，类 B 中的 b 方法加上采用 Propagation.REQUIRES_NEW 模式，然后在 a 方法中调用 b 方法操作数据库，然而 a 方法抛出异常后，b 方法并没有进行回滚，因为 Propagation.REQUIRES_NEW 会暂停 a 方法的事务 )
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
- noRollbackFor 属性
  - noRollbackFor：抛出指定的异常类型，不回滚事务，也可以指定多个异常类型。

## java 面向过程编程

- 核心要素
  - 数据结构: 原生类型, 对象类型, 数组类型, 集合类型
  - 方法调用: 访问性, 返回类型, 方法参数, 异常等
  - 执行流程: 赋值, 逻辑, 迭代(循环), 递归等

## Java 面向对象编程

(java9 之前)

- public: all
- protected: 继承 + 同包 (不能用于修饰最外层类)
- (default): 同包
- private: 当前类 (不能用于修饰最外层类)

---

- 面向对象基本特性
  - 封装性
  - 继承性
  - 多态性
- 面向对象设计模式
  - GoF 23: 构建, 结构, 行为
  - 方法设计: 名称, 访问性, 参数, 返回类型, 异常
  - 泛型设计: 类级别, 方法级别
  - 异常设计: 层次性, 传播性

---

- 单元：一个类或者一组类（组件）
  - 类采用名词结构
    - 动词过去式+名词
      - ContextRefreshedEvent
    - 动词 ing + 名词
      - InitializingBean
    - 形容词 + 名词
      - ConfigurableApplicationContext
- 执行：某个方法
  - 方法命名：动词
    - execute
    - callback
    - run
  - 方法参数：名词
  - 异常：
    - 根（顶层）异常
      - Throwable
        - checked 类型：Exception
        - unchecked 类型：RuntimeException
        - 不常见：Error
    - Java 1.4 `java.lang.StackTraceElement`
      - 添加异常原因（cause）
        - 反模式：吞掉某个异常
        - 性能：注意 `fillInStackTrace()` 方法的开销，避免异常栈调用深度
          - 方法一：JVM 参数控制栈深度（物理屏蔽）
          - 方法二：logback 日志框架控制堆栈输出深度（逻辑屏蔽）

## Java 函数式基础

- 面向函数编程(Since Java 8)
  - Lambda 表达式
  - 默认方法
  - 方法引用
- 匿名内置类
  - 典型场景
    - Java Event/Listener
    - Java Concurrent
    - Spring Template
  - 基本特点
    - 基于多态(多数基于接口编程)
    - 实现类无需名称
    - 允许多个抽象方法
  - 局限性
    - 代码臃肿
    - 强类型约束
    - 接口方法升级
- Lambda 表达式
  - 基本特点
    - 流程编排清晰
    - 函数类型编程
    - 改善代码臃肿
    - 兼容接口升级
  - 实现手段
    - @FunctionInterface 接口
    - Lambda 语法
    - 方法引用
    - 接口 default 方法实现
  - 局限性
    - 单一抽象方法
    - Lambda 调试困难
    - Stream API 操作能力有限

## Java 模块化基础

- Java 9 模块化
  - 动机
    - 强封装的实施与精确的模块依赖声明使得大型应用和框架更好的维护
    - 安全提升
    - 增快应用模块中类型检测的速度, 提升应用性能
    - 瘦身 JDK 以及 SE 的体积, 有利于在小型计算设备使用和云端部署
  - 收益
    - 提升平台伸缩性
    - 提升平台完整性
    - 提升性能
  - 定义模块
    - 模块声明
    - 模块依赖
    - 模块导出
  - 模块结构
    - 模块描述文件
    - 平台模块
    - 模块 artifacts
- 基本特性
  - 并非所有的 `public` Class 都可以被运用，需要 `exports` 来配合
  - `exports` 所配置的 `package` 下必须要有 Class
- 负面问题
  - 对人的要求高 (对 Class 透明化)
  - 必须了解相关 `module-info.java` 语义
  - 需要了解某些类的依赖
  - 需要了解某些类职责
- 个人观点
  - 收益不大，代价不小
  - 对团队要求极高，容易出现互喷的情况
  - Java 9 之前采用 jar 文件管理，Java 9 模块化之后，编程了 module-info.java 管理，还需要考虑与 Maven 依赖管理组件如何配合

## Java 接口设计

### 通⽤设计 - 可继承性

### 具体类设计 -常⻅场景

- 功能组件
  - HashMap
- 接⼝/抽象类实现
  - HashMap <- AbstractMap <- Map
- 数据对象
  - POJO
- ⼯具辅助
  - \*Utils
  - ViewHelper
  - Helper

### 抽象类设计-常⻅场景

- 接⼝通⽤实现（模板模式）

  - AbstractList
  - AbstractSet
  - AbstractMap

- 状态/⾏为继承

### 接口设计

- Serializable
- Cloneable
- AutoCloseable
- EventListener

## Java 枚举设计

- 枚举(enum) 实际是 final class
- 枚举(enum) 成员修饰符为 public static final
- `values()` 是 Java 编译器做的字节码提升

## Java 范型设计

### 范型使用场景

- 编译时强类型检查
- 避免类型强转
- 实现通用算法

### 范型类型

- 调用泛型类型
- 实例例化泛型
- Java 7 Diamond 语法
- 类型参数命名约定

### 类型参数命名约定

- E:表示集合元素(Element)
- V:表示数值(Value)
- K:表示键(Key)
- T:表示类型(Type)

## Java 方法设计

- 方法命名设计
- 方法返回类型设计
- 方法参数类型设计
- 方法参数名称设计
- 方法参数量设计

## 函数式设计

### 理解@FunctionInterface

- 用于函数式接口类型声明的注解类型, 这些接口的实例被 Lambda 表达式, 方法引用或构造器应用创建.
- 函数式接口只能有一个抽象方法, 并排除接口默认方法以及声明中覆盖 Object 的公开方法的同级.
- 不能标注在注解, 类以及枚举上
- 只要满足函数式接口的要求, 注解不是必须的

---

- 提供类型- `Supplier<T>`
- 消费类型- `Consumer<T>`
- 转换类型- `Function<T, R>`
- 断定类型- `Predicate<T>`
- 隐藏类型- `Action`

### 函数式接口设计

- Supplier - `T get()`
- Consumer - `void accept()`
- Function - `R apply(T t)`
- Predicate - `boolean test(T t)`

### 函数式在框架中的运用

- Supplier - 数据来源, 代码替换接口
- Consumer - 执行 callback
- Function - 类型转换, 业务处理
- Predicate - 过滤, 对象比较

### Stream API 设计

- 转换: `Stream#map(Function)`
- 过滤: `Stream#filter(Predicate)`
- 排序: `Stream#sorted()/(Comparator)`
- 串行: Stream(默认类型)
- 并行: Stream#parallel() / isParallel()
- Collect, 分组, 聚合, flatMap, reduce

## 模块化设计

### 1. Java 模块化基础回顾

- 强封装的实施与精确的模块依赖声明使得大型应用和框架更好的维护
- 安全提升
- 增快应用模块中类型检测的速度, 提升应用性能
- 瘦身 JDK 以及 SE 的体积, 有利于在小型计算设备使用和云端部署

---

```java
module second.part {
    // requires 内部依赖
    // 命名模块
    // artifact 资源根 package 存有 module-info.class（包含模块名称）
    requires java.base; // 默认依赖
    requires java.sql;  // 传递依赖 requires transitive
    requires java.compiler;  // exports 控制可访问的 API 包

    // 非命名模块
    requires transitive spring.context;
    requires transitive org.apache.commons.lang3;  // 规律：
    // Maven artifactId（jar或者war文件）中横划（画）线 "-"
    // 在模块化名称使用 "."
    requires transitive commons.collections;

    // exports 供外部访问
    exports cn.az.java.modular;
}
```

### 2. Java 模块化核心概念

- 模块路径可能是单个 artifact,或者是多个 artifacts 的目录, 存在于宿主机器上
- 模块路径的差异性
  - 定位整个模块而非类型
  - 运行时和编译时, 同一目录下不允许出现同名模块

### 3. 模块化迁移

- 迁移分析
  - 需要明确应用实现依赖的 JDK 模块
  - 需要名曲二方或三方 jar 所依赖的 JDK 模块
  - 需要微服务化应用

### 4. 模块化反射

- 获取模块
  - 获取模块 - Class#getModule()
  - 模块接口 - Module
  - 模块描述文件接口 - ModuleDescriptor

## 集合框架基础运用

### 1. 集合框架总览

集合框架是一个用于展现和操作集合的同意框架, 其实现细节能够让集合独立地操作

---

- 主要优势
  - Reduces programming effort
  - Increase performance
  - Provides interoperability between unrelated APIs
  - Reduces the effort required to learn APIs
  - Reduces the effort required to design and implement APIs
  - Fosters software reuse
- 基本组成
  - Collection interfaces
  - Infrastructure
  - General-purpose implementations
  - Abstract implementations
  - Legacy implementations
  - Convenience implementations
  - Wrapper implementations
  - Special-purpose implementations
  - Array Utilities
  - Algorithms
  - Concurrent implementations
- 集合接口
  - `java.util.Collection`
    - List
    - Set
    - SortedSet
    - NavigableSet
    - Queue
    - Deque
  - `java.util.concurrent`
    - BlockingQueue
    - BlockingDeque
    - TransferQueue
    - ConcurrentMap
    - ConcurrentNavigableMap
  - `java.util.Map`
    - SortedMap
    - NavigableMap

### 2. 集合框架内建实现

- Legacy
  - Vector
  - Stack
  - Hashtable
  - Enumeration
  - BitSet
- Common
  - HashSet, TreeSet, LinkedHashSet
  - ArrayList, LinkedList
  - ArrayDeque, LinkedList
  - HashMap, TreeMap, LinkedHashMap
- Concurrent
  - BlockingQueue, TransferQueue, BlockingDeque, ConcurrentMap, ConcurrentNavigableMap
  - LinkedBlockingQueue, ArrayBlockingQueue, PriorityBlockingQueue, DelayQueue, SynchronousQueue
  - LinkedBlockingDeque, LinkedTransferQueue, CopyOnWriteArrayList, CopyOnWriteArraySet, ConcurrentSkipListSet
  - ConcurrentHashMap, ConcurrentSkipListMap

### 3. 集合框架抽象实现

- AbstractCollection
  - AbstractList
  - AbstractSet
  - AbstractQueue
- Map
  - AbstractMap

### 4. 集合框架面试题

> 为什么 Collection 没有继承 Clonable 和 Serializable

实例类自行选择是否实现 cloneable 和序列化

### 5. Java 集合遍历实现

高性能的最小化实现接口

Collections, Arrays, BitSet, EnumSet

- 接口类型
  - 单例集合接口 (Collections.singleton\*)(List, Set, Map) -- 不变集合(Immutable)
  - 空集合接口 (Collections.empty\*)
    - 枚举：Collections.emptyEnumeration()
    - 迭代器：emptyIterator()、emptyListIterator()
    - List：emptyList()
    - Set: emptySet()、emptySortedSet()、emptyNavigableSet()
    - Map：emptyMap()、emptySortedMap()、emptyNavigableMap()
  - 转换集合接口 (`Collections.*` , `Arrays.*`)
    - Enumeration: `Collections.enumeration(Collection)`
    - List: `Collections.list(Enumeration<T>)`, `Arrays.asList(T…)`
    - Set: `Collections.newSetFromMap(Map<E, Boolean>)`
    - Queue: `Collections.asLifoQueue(Deque<T>)`
    - HashCode: Arrays.hashCode(…)
    - String: Arrays.toString(…)
  - 列举集合接口 (\*.of(...))

### 6. 集合包装实现

功能性添加, 比如同步以及其他实现: Wrapper 模式原则, 入参集合类型与返回类型相同或其子类

- 同步包装接口 (Collections.synchronized\*)
- 只读包装接口 (Collections.unmodifiable\*)
- 类型安全包装接口 (Collections.checked\*)

### 7. 集合特殊实现

## 算法

### 1.排序算法

- 计算复杂度: 最佳, 最坏以及平均复杂度
- 内存使用: 空间复杂度
- 递归算法: 排序算法是否用到了递归
- 稳定性: 当相同的键存在时, 经过排序后, 其值也保持相对的顺序
- 比较排序: 集合中的两个元素进行比较排序
- 串行或并行: 是否运用传销

---

- 冒泡排序 (Bubble Sort): 最佳 O(n), 平均 O(n^2), 最坏 O(n^2)
- 插入排序 (Insertion Sort): 最佳 O(n), 平均 O(n^2), 最坏 O(n^2)
- 快速排序 (Quick Sort): 最佳 O(nlogn), 平均 O(nlogn), 最坏 O(n^2)
- 合并排序 (Merge Sort): 最佳 O(nlogn), 平均 O(nlogn), 最坏 O(nlogn)
- Tim 排序 (Tim Sort): 最佳 O(n), 平均 O(nlogn), 最坏 O(nlogn)

## 进程, 线程, 协程

### 进程

指计算机中已运⾏的程序。进程为曾经是分时系统的基本运作单位。在面向进程设计的系统中，进程是程序的基本执⾏实体;在⾯向线程设计的系统中，进程本身不是基本运⾏单位，⽽是线程的容器。 程序本身只是指令、数据及其组织形式的描述，进程才是程序的真正运⾏实例。

### 线程

操作系统能够进行运算调度的最小单位。它被包含在进程之中, 是进程的实际运行单位。

#### Java 线程编程模型

- 小于 1.5: Thread, Runnable
- 1.5: Executor, Callable, Future
- 1.7: ForkJoin
- 1.8: CompletionStage, CompletableFuture
- 1.9: Flow (Publisher, Subscriber, Subscription, Processor)

#### Java 线程池

- 小于 1.5: 自定义 Thread Pool
- 1.5+: ExecutorService, ThreadPoolExecutor, ScheduledThreadPoolExecutor
- 1.7: ForkJoinPool

---

- 同步: 最常见的编程手段, 指任务发起方和执行方在同一线程中完成
- 异步: 常见的提升吞吐手段, 指任务发起方和执行方在不同线程中完成
- 非阻塞: 一种编程模型, 由通知状态被动的回调执⾏，同步或异步执⾏均可

#### 线程状态

- API - java.lang.Thread.State (Since 1.5)
  - NEW: 线程已创建, 尚未启动
  - RUNNABLE: 表示线程处于可运行状态, 不代表一定运行
  - BLOCKED: 被 Monitor 锁阻塞, 表示当前线程在同步锁的场景运作
  - WAITTING: 线程处于等待状态, 由 Object#wait(), Thread#join()或 LockSupport#park()引起
  - TIMED_WAITTING: 线程处于规定时间内的等待状态
  - TERMINATED: 线程执行结束
- 使用场景
  - 线程堆栈
    - 工具 - jstack
    - JMX - java.lang.management.ThreadMXBean#dumpAllThreads(boolean, boolean)
    - API - java.lang.Thread#dumpStack()

---

> 为什么弃用 stop, suspend 和 resume 方法?

stop 会 unlock Monitor, 导致了 lock 块中数据不再安全

> 如何中止一个线程

- private volatile boolean stop(开关)
- 抛出异常

> 如何理解线程“中止”方法

interrupt()并不能中止线程, 但是可以传递 interrupt 状态 (在子线程 start()之前调用没有效果)

### Java 并发理论基础

- 线程安全实现手段
  - Reentrancy (重进入)
  - Thread-local storage (线程本地存储)
  - Immutable objects (不可变对象)
  - Mutual exclusion (互斥)
  - Atomic Operations (原子操作)
- 同步引入的问题
  - 死锁 (Dead Lock)
  - 饥饿 (Starvation)
  - 优先级倒转 (Priority Inversion)
  - 繁忙等待 (Busy Waiting)
- 同步实现
  - 信号量 (Semaphores)
  - 屏障 (Barriers)
  - 互斥 (Mutex)
  - 条件变量 (Condition Variables)
  - 自旋锁 (SpinLock)
  - 读写锁 (Reader-Writer Lock)

### Java synchronized

- 锁定对象: 对象(Object) 和类 (Class)
- 修饰范围: 方法(Method), 代码块(Block)
- 特点: 重新进入(Reentrant)
- 方法 flags: ACC_SYNCHRONIZED
- 字节码: monitorenter 和 monitorexit
- 锁实现: Thin Lock, Inflated, HeavyWeight

### Java volatile

- 底层: 内存屏障 (Memory Barrier)
- 语义: 可见性

### Java CAS (Compare And Swap)

- 底层: 原子信号指令 (Atomic Semaphore Instructions)
- 语义: 原子性

### Java 并发锁

- 重进入锁 - ReentrantLock
  - 与 synchronized 的类似点
    - 互斥 (Mutal Exclusion)
    - 重进入 (Reentrancy)
    - 隐形 Monitor 机制
  - 与 synchronized 的不同点
    - 获得顺序 (公平锁和非公平)
    - 限制锁定 (tryLock)
    - 条件对象支持 (Condition Support)
    - 运维方法
- 重进入读写锁 - ReentrantReadWriteLock
- 邮票锁 - StampedLock
  - 写 (Writing)
  - 读 (Reading)
  - 优化读 (Optimistic Reading)

公平锁 -> 保证执行顺序  
t1, t2, t3 -> t1.unlock() -> 若 t3 获得执行权 -> LockSupport.park(t3) -> t2.lock() -> t2.unlock() -> LockSupport.unpark(t3) -> t3.lock()

---

```java
class X {
  private final ReentrantLock lock = new ReentrantLock();

  public void m() {
    lock.lock();
    try {
      System.out.println("xxx");
    } finally {
      lock.unlock();
    }
  }
}
```

### Java 原子操作

- `java.util.concurrent.atomic.Atomic*`
- `java.util.concurrent.atomic.*Adder`

### Java 并发限制

- CountDownLatch -> latch.countdown() -> latch.await()
- CyclicBarrier -> cb.await() -> cb.reset()
- Semaphore -> s.acquire() -> s.release()

### Java-线程池

- ThreadPoolExecutor
- ScheduledThreadPoolExecutor
- Runnable VS Callable
- Future

### 面试题

### Java 并发集合框架

- CopyOnWrite\*: (CopyOnWriteArrayList, CopyOnWriteArraySet)
  - 并发特征:
    - 读: 无锁(volatile), O(1)
    - 写: 同步(synchronized), 复制(较慢, 内存消耗)
- ConcurrentSkipList\*: (ConcurrentSkipListMap, ConcurrentSkipListSet)
  - 并发特征: 无锁
  - 数据结构: 有序(ConcurrentNavigableMap), 跳表 (SkipList)变种
  - 时间复杂度: 平均 log(n) - containsKey, get, put 和 remove 方法
  - 特征: 空间换时间
- 跳表(SkipList)
  - 时间复杂度: 搜索,插入 O(log(n))
- ConcurrentSkipListMap
  - size()方法非 O(c)操作
  - 批量操作无法保证原子性: putAll, equals, toArray, containsValue, clear
  - Iterator, Spliterators: 弱一致性, 非 fast-fail
  - 非 null 约束: key 和 value 都不能为 null
- ConcurrentHashMap
  - 并发特征
    - 1.5-1.6: 读锁(部分), 写锁
    - 1.7, 1.8: 读无锁, 写锁
  - 数据机构
    - 1.8 之前: 桶(bucket)
    - 1.8: 桶(bucket), 平衡树(红黑树)
- BlockingQueue (ArrayBlockingQueue, LinkedBlockingQueue, SynchronousQueue)
  - 并发特征: 读锁, 写锁
  - 子接口/实现: LinkedBlockingDeque, LinkedTransferQueue

### Fork/Join 框架

- 编程模型
  - ExecutorService 扩展 - ForkJoinPool
  - Future 扩展 - ForkJoinTask, RecursiveTask, RecursiveAction

### CompletableFuture

- Future 的限制
  - 阻塞式结果返回
  - 无法链式多个 Future
  - 无法合并多个 Future 结果
  - 缺少异常处理

### Flow 框架

- Reactive Streams
  - Publisher
  - Subscriber
  - Subscription
  - Processor

## POSIX Thread

### 1. 简介

Portable Operating System Interface for UNIX

- 线程基础
  - 同进程中的线程
    - 进程指令
    - 打开的文件描述
    - 信号和信号处理
    - 当前工作目录
  - 线程特有的
    - 线程 ID
    - 寄存器集合, 栈指针
    - 局部变量栈和返回地址
    - 信号掩码
    - 优先级
    - 返回值

### 2. 基础 API

- 创建线程
  - pthread_create
    - Java API - new Thread(Runnable)
  - pthread*attr*\*
    - 状态: 是否可 join, 默认值 PTHREAD_CREATE_JOINABLE
    - 调度策略: 是否实时, 默认值
    - 范围: 内核(PTHREAD_SCOPE_SYSTEM), 用户(PTHREAD_SCOPE_PROCESS)
    - 栈: 地址和大小
  - pthread_join
    - Java API - Thread.join()
- 中止线程
  - 等待函数执行完成
  - void pthread_exit(void\* value)
  - int pthread_cancel(pthread_t thread)

### 3. 同步 API

- 互斥(Mutex)
- 条件变量(Condition Variable)
- 屏障(Barrier)

## Java 内存模型

### 共享变量(Shared Variables)

Memory that can be shared between threads is called shared memory or heap memory.  
All instance fields, _static_ fields, and array elements are stored in heap memory.

```java
private static final jdk.internal.misc.Unsafe U = jdk.internal.misc.Unsafe.getUnsafe();
// 计算对象中变量的绝对地址(偏移量)
private static final long VALUE = U.objectFieldOffset(AtomicInteger.class, "value");

private volatile int value;

public final int getAndSet(int newValue) {
    return U.getAndSetInt(this, VALUE, newValue);
}
```

Local variables, formal method parameters and exception handler parameters are never shared between threads and are unaffected by the memory model.

### 动作(Actions)

- Read(normal, or non-volatile). Reading a variable
- Write(normal, or non-volatile). Writing a variable
- Synchronization actions:
  - Volatile read.
  - Volatile write.
  - Lock. Locking a monitor.
  - Unlock. Unlocking a monitor.
  - The (synthetic) first and last action of a thread.
  - Actions that start a thread or detect that a thread has terminated.(start() | isAlive())

### 顺序(Order)

#### Programs and Program Order

- Sequential consistency is a very strong guarantee
- If a program has no data races, then all executions of the program will appear to be sequentially consistent (无数据竞争时)

#### Synchrinization Order

the synchronization order of the synchronization actions in `t` is consistent with the program order of `t`.

- An unlock action on monitor m _synchronizes-with_ all subsequent lock actions on m(where "sebsequent" is defined according to the synchronization order).
- A write to a volatile variable `v` _synchronizes-with_ all subsequent reads of v by any thread(where "sebsequent" is defined according to the synchronization order).
- An action that starts a thread _synchronizes-with_ the first action in the thread it starts.
- The write of the default value (zero, false or null) to each variable _synchronizes-with_ the first action in every thread.
- The final action in a thread T1 _synchronizes-with_ any action in another thread T2 that detects that T1 has terminated.
- If thread T1 interrupts thread T2, the interrupt by T1 _synchronizes-with_ any point where any other thread(including T2) determines that T2 has been interrupted(by having an `IntterruptedException` thrown or by invoking `Thread.interrupted` or `Thread.isInterrupted`)

The source of a _synchronizes-with_ edge is called a release, and the destination is called an acquire.

#### Happens-before Order

Two actions can be ordered by a happens-before relationship. If one action happens-before another, then the first is visible to and ordered before the second.

### 执行(Executions)

## AQS

### AbstractQueuedSynchronizer 原理

### 条件变量(Condition)原理

## Java Thread

### Java Thread 实现原理

#### 基本操作

##### 创建 - `new Thread()`

Thread 对象 new 实际普通的 Java 对象创建，不涉及 JVM 或者 OS 线程创建和启动

JNI 类型：`jobject`

##### 启动 - `Thread#start()`

`Thread#start()` 引导线程启动，不一定马上启动（非阻塞），终究会 `Thread#run()` 方法（回调）

`Thread#start()` 方法定义：

```java
public synchronized void start() { // 线程安全
    ...
    group.add(this); // 将当前线程添加到所在 ThreadGroup
    ...
    try {
        start0(); // Native 方法
    }
    ...
}

private native void start0(); // JNI Java 调用 JVM 方法
```

Java 类名称: `java.lang.Thread`

```java
public
class Thread implements Runnable {
    /* Make sure registerNatives is the first thing <clinit> does. */
    private static native void registerNatives();
    static {
        registerNatives();
    }
    ...
}
```

JNI 方法名称（C 函数）：

```c++
JNIEXPORT void JNICALL
Java_java_lang_Thread_registerNatives(JNIEnv *env, jclass cls)
{
    (*env)->RegisterNatives(env, cls, methods, ARRAY_LENGTH(methods));
}
```

java.lang.Thread native 方法的映射表：

```c++
static JNINativeMethod methods[] = {
    {"start0",           "()V",        (void *)&JVM_StartThread},
    {"stop0",            "(" OBJ ")V", (void *)&JVM_StopThread},
    {"isAlive",          "()Z",        (void *)&JVM_IsThreadAlive}
    ...
};
```

方法签名：`java.lang.Thread#start0()`

Native 方法：`JVM_StartThread`

```c++
/*
 * java.lang.Thread
 */
JNIEXPORT void JNICALL
JVM_StartThread(JNIEnv *env, jobject thread);
```

> 方法声明位置：`/jdk/src/share/javavm/export/jvm.h`

实现入口：

```c++
JVM_ENTRY(void, JVM_StartThread(JNIEnv* env, jobject jthread))
...
{
    jlong size = java_lang_Thread::stackSize(JNIHandles::resolve_non_null(jthread));
    ...
      size_t sz = size > 0 ? (size_t) size : 0;
      native_thread = new JavaThread(&thread_entry, sz);
    ...
}
```

`native_thread` 创建了 `JavaThread` 类型对象，第一个构造参数 `&thread_entry`，即函数指针（引用函数 `thread_entry`）：

```c++
static void thread_entry(JavaThread* thread, TRAPS) {
  HandleMark hm(THREAD);
  Handle obj(THREAD, thread->threadObj()); // 1
  JavaValue result(T_VOID);
  JavaCalls::call_virtual(&result,         // 2
                          obj,             // 3
                          KlassHandle(THREAD, SystemDictionary::Thread_klass()), // 4
                          vmSymbols::run_method_name(), // 5
                          vmSymbols::void_method_signature(), // 6
                          THREAD);
}
```

1. `thread->threadObject()` 返回了 Java 代码创建 `java.lang.Thread` 对象
2. 调用 JVM C++ 对象虚拟方法（多态）
3. obj `java.lang.Thread` 对象包装
4. java.lang.Thread 类对象
5. `java.lang.Thread#run()` 方法名称
6. `void` 方法签名

综上所述，`thread_entry` 方法执行目的是要执行 `java.lang.Thread` 对象的`run()` 方法

分析 `JavaThread::JavaThread` 构造器：

```c++
JavaThread::JavaThread(ThreadFunction entry_point, size_t stack_sz)
  ...
{
  ...
  set_entry_point(entry_point); // 设置 thread_entry 函数到当前对象
  ...
  os::create_thread(this, thr_type, stack_sz);
  ...
}
```

第一个构造参数 `entry_point` 是 `ThreadFunction`， `entry_point` 实际指向函数 -> `thread_entry`，`thread_entry` 函数会被 `JavaThread` 对象作为成员变量保存，接下来执行 `os::create_thread(this, thr_type, stack_sz);` 方法，以 Linux 实现为例：

```c++
bool os::create_thread(Thread* thread, ThreadType thr_type, size_t stack_size) {
  assert(thread->osthread() == NULL, "caller responsible");

  // Allocate the OSThread object
  OSThread* osthread = new OSThread(NULL, NULL);
  if (osthread == NULL) {
    return false;
  }

  // set the correct thread state
  osthread->set_thread_type(thr_type);

  // Initial state is ALLOCATED but not INITIALIZED
  osthread->set_state(ALLOCATED);

  thread->set_osthread(osthread);

  // init thread attributes
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   ...
    pthread_t tid;
    int ret = pthread_create(&tid, &attr, (void* (*)(void*)) java_start, thread);

    pthread_attr_destroy(&attr);

  ...
  return true;
}
```

`java.lang.Thread#start0()` -> JVM JNI -> POSIX Thread `pthread_create`

`java_start` 函数就是实际 JVM 执行操作：

```c++
static void *java_start(Thread *thread) {
  ...
  // call one more level start routine
  thread->run();

  return 0;
}
```

参数 `thread` 实际为 `JavaThread` 对象，`thread->run()` 调用实际是 `JavaThread::run()` 方法的执行：

```c++
void JavaThread::run() {
  ...
  thread_main_inner();
  // Note, thread is no longer valid at this point!
}

void JavaThread::thread_main_inner() {
  ...
  if (!this->has_pending_exception() &&
      !java_lang_Thread::is_stillborn(this->threadObj())) {
    {
      ResourceMark rm(this);
      this->set_native_thread_name(this->get_thread_name());
    }
    HandleMark hm(this);
    this->entry_point()(this, this);
  }

  DTRACE_THREAD_PROBE(stop, this);

  this->exit(false);
  delete this;
}
```

`this->entry_point` 是 `JavaThread` 成员变量，指向 `thread_entry` 函数：

```c++
Thread.start0() -> JVM_StartThread : JVM Native 调用
JVM_StartThread -> new JavaThread() : 创建一个 JavaThread() 对象
new JavaThread() -> os_create_thread() : 创建线程（OS 相关的）
os_create_thread() -> pthread_create() : 创建 POSIX Thread
pthread_create() -> java_start() : 线程启动并回调 java_start()
java_start() -> JavaThread_run() : 调用 JavaThread run() 方法
JavaThread_run() -> thread_main_inner() : 内部调用
thread_main_inner() -> entry_point() : entry_point 指向 thread_entry()
thread_entry() -> java.lang.Thread.run() : 回调 Java Thread run() 方法
```

当 `java.lang.Thread#start()` 方法调用完成，JVM 所创建的`JavaThread`对象就移除： `delete this`

##### 完成 - `Thread#join()`

`java.lang.Thread#join()` 底层使用 `java.lang.Object#wait(long)` 方法实现：

```java
   public final synchronized void join(long millis)
    throws InterruptedException {
    ...
    if (millis == 0) {
        while (isAlive()) {
            wait(0);
        }
    }
    ...
    }
```

`java.lang.Object#wait(long)` 方法：

```java
public class Object {
    private static native void registerNatives();
    static {
        registerNatives();
    }
    ...
    public final native void wait(long timeoutMillis) throws InterruptedException;
    ...
}
```

Object Native 实现：jdk/src/share/native/java/lang/Object.c

```c++
static JNINativeMethod methods[] = {
    {"hashCode",    "()I",                    (void *)&JVM_IHashCode},
    {"wait",        "(J)V",                   (void *)&JVM_MonitorWait},
    {"notify",      "()V",                    (void *)&JVM_MonitorNotify},
    {"notifyAll",   "()V",                    (void *)&JVM_MonitorNotifyAll},
    {"clone",       "()Ljava/lang/Object;",   (void *)&JVM_Clone},
};
```

`wait(long)` 方法映射 `JVM_MonitorWait`:

```c++
JVM_ENTRY(void, JVM_MonitorWait(JNIEnv* env, jobject handle, jlong ms))
  JVMWrapper("JVM_MonitorWait");
  Handle obj(THREAD, JNIHandles::resolve_non_null(handle));
  JavaThreadInObjectWaitState jtiows(thread, ms != 0);
  if (JvmtiExport::should_post_monitor_wait()) {
    JvmtiExport::post_monitor_wait((JavaThread *)THREAD, (oop)obj(), ms);
  }
  ObjectSynchronizer::wait(obj, ms, CHECK); // 1
JVM_END
```

1. ObjectSynchronizer::wait(obj, ms, CHECK):

```c++

```

wait() 方法执行前提，当前线程获得锁（synchronized）

T1-> 获得锁（synchronized ） -> wait() -> 释放锁

T2（等待获得锁）

T3

`Object#wait()`

Object -> Object Monitor -> WaitSet

```java
Object monitor  = new Object();
synchronized(monitor) {
    if(true){
        monitor.wait(); // monitor WaitSet {T1 -> T2 -> T3}
    }
}

synchronized(monitor) {
    if(true){
        monitor.notify(); // T4 monitor WaitSet pop() T1
    }
}
```

WaitSet 属于 ObjectMonitor 成员：

```c++
class ObjectMonitor {
    ...
 protected:
  ObjectWaiter * volatile _WaitSet; // LL of threads wait()ing on the monitor
}
```

\_WaitSet 是 `ObjectWaiter` 类型对象，属于双向队列节点：

```c++
class ObjectWaiter : public StackObj {
 public:
  ...
  ObjectWaiter * volatile _next; // 下一个节点（Waiter）
  ObjectWaiter * volatile _prev; // 上一个节点（Waiter）
  Thread*       _thread;
  jlong         _notifier_tid;
  ParkEvent *   _event;
  volatile int  _notified ;
  volatile TStates TState ;
  Sorted        _Sorted ;           // List placement disposition
  bool          _active ;           // Contention monitoring is enabled
 public:
  ObjectWaiter(Thread* thread);

  void wait_reenter_begin(ObjectMonitor *mon);
  void wait_reenter_end(ObjectMonitor *mon);
};
```

- JVM `ObjectWaiter` 类似于 Java AQS `AbstractQueuedSynchronizer.Node`
- `ObjectWaiter` 属于 CLH （双向）队列节点（自旋锁）

```c++
void Thread::SpinAcquire (volatile int * adr, const char * LockName) {
  ...
  for (;;) {
     while (*adr != 0) {
        ++ctr ;
        if ((ctr & 0xFFF) == 0 || !os::is_MP()) {
           if (Yields > 5) {
             // Consider using a simple NakedSleep() instead.
             // Then SpinAcquire could be called by non-JVM threads
             Thread::current()->_ParkEvent->park(1) ;
           } else {
             os::NakedYield() ;
             ++Yields ;
           }
        } else {
           SpinPause() ;
        }
     }
     if (Atomic::cmpxchg (1, adr, 0) == 0) return ;
  }
  ...
}
```

`AbstractQueuedSynchronizer.Node` 属于 CLH 变种（双向）队列节点（阻塞）：

```java
    private final boolean parkAndCheckInterrupt() {
        LockSupport.park(this);
        return Thread.interrupted();
    }
```

- JVM `ParkEvent->park` 以及 JDK `LockSupport.park(this)` 底层同为 `pthread_cond_wait` 函数。
- JVM `ParkEvent->unpark` 以及 JDK `LockSupport.unpark(this)` 底层同为 `pthread_cond_singal` 函数。

WaitSet

N1(T1)

自旋锁（Spin Lock）

CLH 队列

- 原生 - JVM ParkEvent（自旋）
- 变种 - JDK AQS Node（阻塞）
  - LockSupport#park()

当前 Thread

Part Event （CLH 队列）

`jdk.internal.misc.Unsafe#park`:

```java
public final class Unsafe {

    private static native void registerNatives();
    static {
        registerNatives();
    }

    public native void park(boolean isAbsolute, long time)
}

```

`hotspot/src/share/vm/prims/unsafe.cpp`:

```c++
UNSAFE_ENTRY(void, Unsafe_Park(JNIEnv *env, jobject unsafe, jboolean isAbsolute, jlong time))
  UnsafeWrapper("Unsafe_Park");
  EventThreadPark event;
...
  JavaThreadParkedState jtps(thread, time != 0);
  thread->parker()->park(isAbsolute != 0, time);
...
UNSAFE_END
```

实际执行的代码：`thread->parker()->park`， 当前线程关联的 `Parker` 对象（由 parker() 方法返回）

结论：

- `java.lang.Object#wait(long)`
  - `JVM_MonitorWait`
    - `ObjectSynchronizer::wait`
      - `Self->_ParkEvent->park ()`
        - `os::PlatformEvent::park()`
          - OS `pthread_cond_wait`
- `Unsafe#park(boolean,long)`
  - JVM Native
    - JVM `Parker::park`
      - OS `pthread_cond_wait`
- `Unsafe#unpark()`
  - JVM Native
    - JVM `Parker::unpark`
      - OS `pthread_cond_singal`

T1 park -> 释放锁

T2 获得锁 -> park

T3 获得锁

T main -> T1.unpark

- 中止 - `Thread#interrupt()`
- 休眠 - `Thread#sleep(long)`
- 让出 - `Thread#yield()`
- 停止 - ~~Thread#stop()~~
