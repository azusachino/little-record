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
module java.sql {
  exports java.sql;
  exports javax.sql;

  requires transitive java.logging;

  uses java.sql.Driver;
}
```

### 2. Java 模块化核心概念

### 3. 模块化迁移

### 4. 模块化反射

## Collections
