# JAVA

## java面向过程编程

- 核心要素
  - 数据结构: 原生类型, 对象类型, 数组类型, 集合类型
  - 方法调用: 访问性, 返回类型, 方法参数, 异常等
  - 执行流程: 赋值, 逻辑, 迭代(循环), 递归等

## Java面向对象编程

(java9之前)

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
  - 方法设计: 名称,访问性, 参数, 返回类型, 异常
  - 泛型设计: 类级别, 方法级别
  - 异常设计: 层次性, 传播性

---

- 单元：一个类或者一组类（组件）
  - 类采用名词结构
    - 动词过去式+名词
      - ContextRefreshedEvent
    - 动词ing + 名词
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
        - unchecked类型：RuntimeException
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
- Lambda表达式
  - 基本特点
    - 流程编排清晰
    - 函数类型编程
    - 改善代码臃肿
    - 兼容接口升级
  - 实现手段
    - @FunctionInterface接口
    - Lambda语法
    - 方法引用
    - 接口default方法实现
  - 局限性
    - 单一抽象方法
    - Lambda调试困难
    - Stream API操作能力有限

## Java模块化基础

- Java 9 模块化
  - 动机
    - 强封装的实施与精确的模块依赖声明使得大型应用和框架更好的维护
    - 安全提升
    - 增快应用模块中类型检测的速度, 提升应用性能
    - 搜身JDK以及SE的体积, 有利于在小型计算设备使用和云端部署
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
  - 对人的要求就高了（对 Class 透明化）
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
  - *Utils
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

## Java枚举设计

- 枚举(enum) 实际是 final class
- 枚举(enum) 成员修饰符为 public static final
- `values()` 是 Java 编译器做的字节码提升

## Java范型设计

### 范型使用场景

- 编译时强类型检查
- 避免类型强转
- 实现通用算法

### 范型类型

- 调⽤用泛型类型
- 实例例化泛型
- Java 7 Diamond 语法
- 类型参数命名约定

### 类型参数命名约定

- E:表示集合元素(Element)
- V:表示数值(Value)
- K:表示键(Key)
- T:表示类型(Type)

## Java方法设计

- 方法命名设计
- 方法返回类型设计
- 方法参数类型设计
- 方法参数名称设计
- 方法参数量设计

## 函数式设计

### 理解@FunctionInterface

- 用于函数式接口类型声明的注解类型, 这些接口的实例被Lambda表达式, 方法引用或构造器应用创建.
- 函数式接口只能有一个抽象方法, 并排除接口默认方法以及声明中覆盖Object的公开方法的同级.
- 不能标注在注解, 类以及枚举上
- 只要满足函数式接口的要求, 注解不是必须的

---

- 提供类型- `Supplier<T>`
- 消费类型- `Consumer<T>`
- 转换类型- `Function<T, R>`
- 断定类型- `Predicate<T>`
- 隐藏类型- `Action`

### 函数式接口设计

- Supplier - T get()
- Consumer - void accept()
- Function - R apply(T t)
- Predicate - boolean test(T t)

### 函数式在框架中的运用

- Supplier - 数据来源, 代码替换接口
- Consumer - 执行callback
- Function - 类型转换, 业务处理
- Predicate - 过滤, 对象比较

### Stream API 设计

- 转换: Stream#map(Function)
- 过滤: Stream#filter(Predicate)
- 排序: Stream#sorted()/(Comparator)
- 串行: Stream(默认类型)
- 并行: Stream#parallel() / isParallel()
- Collect, 分组, 聚合, flatMap, reduce

## 模块化设计

### 1. Java 模块化基础回顾

- 强封装的实施与精确的模块依赖声明使得大型应用和框架更好的维护
- 安全提升
- 增快应用模块中类型检测的速度, 提升应用性能
- 瘦身JDK以及SE的体积, 有利于在小型计算设备使用和云端部署

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

- 模块路径可能是单个artifact,或者是多个artifacts的目录, 存在于宿主机器上
- 模块路径的差异性
  - 定位整个模块而非类型
  - 运行时和编译时, 同一目录下不允许出现同名模块

### 3. 模块化迁移

- 迁移分析
  - 需要明确应用实现依赖的JDK模块
  - 需要名曲二方或三方jar所依赖的JDK模块
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
  - Fosters software resue
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

> 为什么Collection没有继承Clonable和Serializable

实例类自行选择是否实现cloneable和序列化

### 5. Java集合遍历实现

高性能的最小化实现接口

Collections, Arrays, BitSet, EnumSet

- 接口类型
  - 单例集合接口 (Collections.singleton*)(List, Set, Map) -- 不变集合(Immutable)
  - 空集合接口 (Collections.empty*)
    - 枚举：Collections.emptyEnumeration()
    - 迭代器：emptyIterator()、emptyListIterator()
    - List：emptyList()
    - Set: emptySet()、emptySortedSet()、emptyNavigableSet()
    - Map：emptyMap()、emptySortedMap()、emptyNavigableMap()
  - 转换集合接口 (Collections.*, Arrays.*)
    - Enumeration: `Collections.enumeration(Collection)`
    - List: `Collections.list(Enumeration<T>)`, `Arrays.asList(T…)`
    - Set: `Collections.newSetFromMap(Map<E, Boolean>)`
    - Queue: `Collections.asLifoQueue(Deque<T>)`
    - HashCode: Arrays.hashCode(…)
    - String: Arrays.toString(…)
  - 列举集合接口 (*.of(...))

### 6. 集合包装实现

功能性添加, 比如同步以及其他实现: Wrapper模式原则, 入参集合类型与返回类型相同或其子类

- 同步包装接口 (Collections.synchronized*)
- 只读包装接口 (Collections.unmodifiable*)
- 类型安全包装接口 (Collections.checked*)

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

- 冒泡排序 (Bubble Sort): 最佳O(n), 平均O(n^2), 最坏O(n^2)
- 插入排序 (Insertion Sort): 最佳O(n), 平均O(n^2), 最坏O(n^2)
- 快速排序 (Quick Sort): 最佳O(nlogn), 平均O(nlogn), 最坏O(n^2)
- 合并排序 (Merge Sort): 最佳O(nlogn), 平均O(nlogn), 最坏O(nlogn)
- Tim 排序 (Tim Sort): 最佳O(n), 平均O(nlogn), 最坏O(nlogn)

## 进程, 线程, 协程

### 进程

指计算机中已运⾏的程序。进程为曾经是分时系统的基本运作单位。在面向进程设计的系统中，进程是程序的基本执⾏实体;在⾯向线程设计的系统中，进程本身不不是基本运⾏单位，⽽是线程的容器器. 程序本身只是指令、数据及其组织形式的描述，进程才是程序的真正运⾏实例。

### 线程

操作系统能够进行运算调度的最小单位. 它被包含在进程之中, 是进程的实际运行单位.

#### Java线程编程模型

- 小于1.5: Thread, Runnable
- 1.5: Executor, Callable, Future
- 1.7: ForkJoin
- 1.8: CompletionStage, CompletableFuture
- 1.9: Flow (Publisher, Subscriber, Subscription, Processor)

#### Java线程池

- 小于1.5: 自定义Thread Pool
- 1.5+: ExecutorService, ThreadPoolExecutor, ScheduledThreadPoolExecutor
- 1.7: ForkJoinPool

---

- 同步: 最常见的编程手段, 指任务发起方和执行方在同一线程中完成
- 异步: 常见的提升吞吐手段, 指任务发起方和执行方在不同线程中完成
- 非阻塞: 一种编程模型, 由通知状态被动的回调执⾏，同步或异步执⾏均可.

#### 线程状态

- API - java.lang.Thread.State (Since 1.5)
  - NEW: 线程已创建, 尚未启动
  - RUNNABLE: 表示线程处于可运行状态, 不代表一定运行
  - BLOCKED: 被Monitor锁阻塞, 表示当前线程在同步锁的场景运作
  - WAITTING: 线程处于等待状态, 由Object#wait(), Thread#join()或LockSupport#park()引起
  - TIMED_WAITTING: 线程处于规定时间内的等待状态
  - TERMINATED: 线程执行结束
- 使用场景
  - 线程堆栈
    - 工具 - jstack
    - JMX - java.lang.management.ThreadMXBean#dumpAllThreads(boolean, boolean)
    - API - java.lang.Thread#dumpStack()

---

> 为什么弃用stop, suspend 和 resume方法?

stop会unlock Monitor, 导致了lock块中数据不再安全
> 如何中止一个线程

- private volatile boolean stop
- 抛出异常

> 如何理解线程“中止”方法

interrupt()并不能中止线程, 但是可以传递interrupt状态 (在子线程start()之前调用没有效果)

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
- 特点: 重新进入(Reentant)
- 方法flags: ACC_SYNCHRONIZED
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
t1, t2, t3 -> t1.unlock() -> 若t3获得执行权 -> LockSupport.park(t3) -> t2.lock() -> t2.unlock() -> LockSupport.unpark(t3) -> t3.lock()

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
- Semaphore -> s.acquire()

### Java 线程池

- ThreadPoolExecutor
- ScheduledThreadPoolExecutor
- Runnable VS Callable
- Future

### 面试题
