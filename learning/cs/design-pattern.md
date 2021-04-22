# DESIGN PATTERN

## 观察者模式

Observer

## 代理模式

Proxy

## 单例模式 - 有五种写法

### 双检查锁版本

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
