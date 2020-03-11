# BASIC QUESTIONS

## HashMap的源码，实现原理，JDK8中对HashMap做了怎样的优化

>

## HaspMap扩容是怎样扩容的，为什么都是2的N次幂的大小

>

## HashMap，HashTable，ConcurrentHashMap的区别

>

## 极高并发下HashTable和ConcurrentHashMap哪个性能更好，为什么，如何实现的

>

## HashMap在高并发下如果没有处理线程安全会有怎样的安全隐患，具体表现是什么

>

## java中四种修饰符的限制范围

>

## Object类中的方法

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

## 接口和抽象类的区别，注意JDK8的接口可以有实现

>

## 动态代理的两种方式，以及区别

>

## Java序列化的方式

>

## 传值和传引用的区别，Java是怎么样的，有没有传值引用

>

## 一个ArrayList在循环过程中删除，会不会出问题，为什么

>

## @transactional注解在什么情况下会失效，为什么

>
