# Spring Related

## Spring-Retry

```java
@EnableRetry
@Configuration
public class RetryConfig {

    public static final AtomicInteger ATOMIC_INTEGER = new AtomicInteger();

    public static void main(String[] args) throws InterruptedException {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(RetryConfig.class);

        applicationContext.refresh();

        RetryConfig rc = applicationContext.getBean(RetryConfig.class);
        rc.haha();

        applicationContext.wait(3000L);
        applicationContext.close();
    }

    @Retryable(value = RuntimeException.class)
    protected void haha() {
        System.out.println("haha " + ATOMIC_INTEGER.addAndGet(1));
        throw new RuntimeException("我错了");
    }

    @Recover
    protected void recover(RuntimeException e) {
        System.out.println("recovered, ex: " + e.getMessage());
    }
}
```

output:

```java
19:50:22.345 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry: count=0
haha 1
19:50:23.350 [main] DEBUG org.springframework.retry.support.RetryTemplate - Checking for rethrow: count=1
19:50:23.350 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry: count=1
haha 2
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Checking for rethrow: count=2
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry: count=2
haha 3
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Checking for rethrow: count=3
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry failed last attempt: count=3
recovered, ex: 我错了
Exception in thread "main" java.lang.IllegalMonitorStateException
    at java.lang.Object.wait(Native Method)
    at com.iflytek.scv.logging.config.RetryConfig.main(RetryConfig.java:32)
```
