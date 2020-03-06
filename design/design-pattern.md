# DESIGN PATTERN

## 观察者模式

>

## 代理模式

>

## 单例模式，有五种写法

```java
public class Singleton {
    private static volatile Singleton instance = null;

    private Singleton() {}

    public static Singleton getInstance() {
        if (instance == null) {
            sychronized (this) {
                if (instance == null) {
                    instance = new Singleton();
                }
            }
        }
        return instance;
    }
}
```

## 可以考Spring中使用了哪些设计模式

>
