# DESIGN PATTERN

> 观察者模式

TODO
> 代理模式

TODO

> 单例模式，有五种写法

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
