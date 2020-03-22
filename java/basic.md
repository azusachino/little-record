# BASIC QUESTIONS

> HashMap的源码，实现原理，JDK8中对HashMap做了怎样的优化

a
> HaspMap扩容是怎样扩容的，为什么都是2的N次幂的大小

为了保证hash散列平稳
> HashMap，HashTable，ConcurrentHashMap的区别

a
> 极高并发下HashTable和ConcurrentHashMap哪个性能更好，为什么，如何实现的

a
> HashMap在高并发下如果没有处理线程安全会有怎样的安全隐患，具体表现是什么

a
> java中四种修饰符的限制范围

- private 私有
- 无修饰符 本类 + 本包
- protected 本类 + 子类
- public 都可以访问

> Object类中的方法

``` java
private static native void registerNatives();
public final native Class<?> getClass();
public native int hashCode();
public boolean equals(Object obj) {
    return (this == obj);
}
protected native Object clone() throws CloneNotSupportedException;
public String toString() {
    return getClass().getName() + "@" + Integer.toHexString(hashCode());
}
public final native void notify();
public final native void notifyAll();
public final native void wait(long timeout) throws InterruptedException;
public final void wait(long timeout, int nanos) throws InterruptedException {
     if (timeout < 0) {
        throw new IllegalArgumentException("timeout value is negative");
    }

    if (nanos < 0 || nanos > 999999) {
          throw new IllegalArgumentException(
                              "nanosecond timeout value out of range");
    }

    if (nanos > 0) {
        timeout++;
    }

    wait(timeout);
}
public final void wait() throws InterruptedException {
    wait(0);
}
protected void finalize() throws Throwable { }
```

> 接口和抽象类的区别，注意JDK8的接口可以有实现

a
> 动态代理的两种方式，以及区别

a
> Java序列化的方式

a
> 传值和传引用的区别，Java是怎么样的，有没有传值引用

a
> 一个ArrayList在循环过程中删除，会不会出问题，为什么

会出现ConcureentModificationException, 需要用iterator来实现遍历删除
> @Transactional注解在什么情况下会失效，为什么

1. @Transactional 应用在非 public 修饰的方法上
2. @Transactional 注解属性 propagation 设置错误
3. @Transactional 注解属性 rollbackFor 设置错误
4. 同一个类中方法调用，导致@Transactional失效
5. 异常被你的 catch“吃了”导致@Transactional失效
6. 数据库引擎不支持事务
