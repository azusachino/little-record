# Untaged Questions

> String a = "123"; String b = "123"; a==b的结果是什么

True (同为常量池 指针)
> HashMap里的hashcode方法和equal方法什么时候需要重写？如果不重写会有什么后果

严重的hash冲突 (key不是基础类型的时候)
> ArrayList和LinkedList底层实现有什么差别？它们各自适用于哪些场合？对此大家也可以

了解下相关底层代码。
> volatile关键字有什么作用? (线程内存和堆内存的差)

保证可见性和happens-before
> CompletableFuture, 怎么实现多线程并发控制

```java
public static List<String> findPrice4(String product){
    List<CompletableFuture<String>> priceFuture = shops.stream()
            .map(shop -> CompletableFuture.supplyAsync( // 异步获取价格
                    () -> shop.getPrice2(product), executor))
            .map(future -> future.thenApply(Quote::parse)) // 获取到价格后对价格解析
            .map(future -> future.thenCompose(quote -> CompletableFuture.supplyAsync( // 另一个异步任务构造异步应用报价
                                                        () -> Discount.applyDiscount(quote), executor)))
            .collect(toList());

    return priceFuture.stream()
            .map(CompletableFuture::join) //join 操作和get操作有相同的含义，等待所有异步操作的结果。
            .collect(toList());
}
```

> JVM里, new出来的对象是在哪个区？再深入一下, 问下如何查看和优化JVM虚拟机内存

Heap
> Java的静态代理和动态代理有什么差别？最好结合底层代码

啊
> JVM数据存储模型,新生代 年老代的构造

eden + 2survivor
> java GC算法, 什么时候会触发minor gc, 什么时候触发full GC

啊
> GC 可达性分析中哪些算是GC ROOT

啊
> 你熟悉的JVM调优参数,使用过哪些调优工具

jmap, jstat
> JAVA有什么锁类型

悲观锁,乐观锁
> 描述下线程池的处理流程

core -> queue -> maxSize -> handler
> 类加载机制,一个类加载到虚拟机中一共有几个步骤,这些步骤的顺序哪些是固定的,哪些不是固定的,实现原理是什么

双亲委派 -> 打破双亲委派
> hashmap是线程不安全的,concurrentHashMap是线程安全的,是怎么实现的

cas+lock
> volatile关键字解决了什么问题,实现原理是什么

高并发中对象的可见性, 内存屏障
> 并发容器有哪些,并发容器和同步容器的区别

ConcurrentHashMap, CopyOnWriteArrayList

锁的粒度更细, 性能更好
