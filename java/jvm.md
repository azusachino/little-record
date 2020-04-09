# JVM & JMM

> JVM的内存结构

a
> JVM方法栈的工作过程，方法栈和本地方法栈有什么区别

a
> JVM的栈中引用如何和堆中的对象产生关联

a
> 逃逸分析技术

a
> GC的常见算法，CMS以及G1的垃圾回收过程，CMS的各个阶段哪两个是Stop the world的，CMS会不会产生碎片，G1的优势

a
> 标记清除和标记整理算法的理解以及优缺点

a
> eden survivor区的比例，为什么是这个比例，eden survivor的工作过程

a
> JVM如何判断一个对象是否该被GC，可以视为root的都有哪几种类型

a
> 强软弱虚引用的区别以及GC对他们执行怎样的操作

a
> Java是否可以GC直接内存

a
> Java类加载的过程

a
> 双亲委派模型的过程以及优势

a
> 常用的JVM调优参数

a
> dump文件的分析

a
> Java有没有主动触发GC的方式

System.gc()
> 说说对synchronized关键字的理解

synchronized关键字解决的是多个线程之间访问资源的同步性，synchronized关键字可以保证被它修饰的方法或者代码块在任意时刻只能有一个线程执行。

在 Java 早期版本中，synchronized属于重量级锁，效率低下，因为监视器锁（monitor）是依赖于底层的操作系统的 Mutex Lock 来实现的，Java 的线程是映射到操作系统的原生线程之上的。如果要挂起或者唤醒一个线程，都需要操作系统帮忙完成，而操作系统实现线程之间的切换时需要从用户态转换到内核态，这个状态之间的转换需要相对比较长的时间，时间成本相对较高，这也是为什么早期的 synchronized 效率低的原因。庆幸的是在 Java 6 之后 Java 官方对从 JVM 层面对synchronized 较大优化，所以现在的 synchronized 锁效率也优化得很不错了。JDK1.6对锁的实现引入了大量的优化，如自旋锁、适应性自旋锁、锁消除、锁粗化、偏向锁、轻量级锁等技术来减少锁操作的开销。

> 如何使用 synchronized 关键字

- 修饰实例方法: 作用于当前对象实例加锁，进入同步代码前要获得当前对象实例的锁
- 修饰静态方法: 也就是给当前类加锁，会作用于类的所有对象实例，因为静态成员不属于任何一个实例对象，是类成员 (static 表明这是该类的一个静态资源，不管new了多少个对象，只有一份) 所以如果一个线程A调用一个实- 例对象的非静态 synchronized 方法，而线程B需要调用这个实例对象所属类的静态 synchronized 方法，是允许的，不会发生互斥现象，因为访问静态 synchronized 方法占用的锁是当前类的锁，而访问非静态 synchronized 方法占用的锁是当前实例对象锁。
修饰代码块: 指定加锁对象，对给定对象加锁，进入同步代码库前要获得给定对象的锁。

> 什么情况下会发生栈内存溢出?

- StackOverFlowError
  - 栈溢出就是方法执行是创建的栈帧超过了栈的深度。那么最有可能的就是方法递归调用产生这种结果 (-Xss)
- OutOfMemoryError(java heap space)
  - heap space表示堆空间，堆中主要存储的是对象。如果不断的new对象则会导致堆中的空间溢出 (-Xmx)
- OutOfMemoryError(PermGen space)
  - JDK1.8移除永久代，所以上述代码在JDK1.7及之前会出现永久代溢出的现象

> Java的内存结构, eden和survivor的比例

Eden区和Survivor区的比例是8：1：1

- 堆：存放对象
- 栈：运行时存放栈帧
- 程序计数器
- 方法区：存放类和常量

---

1. -Xms设置堆的最小空间大小。
2. -Xmx设置堆的最大空间大小。
3. -XX:NewSize设置新生代最小空间大小。
4. -XX:MaxNewSize设置新生代最大空间大小。
5. -XX:PermSize设置永久代最小空间大小。
6. -XX:MaxPermSize设置永久代最大空间大小。
7. -Xss设置每个线程的堆栈大小。

> Java内存模型为什么要分成新生代, 老年代, 持久代. 新生代中为什么要分Eden和Survivor
