# java.util.concurrent

## 并发工具类 - 分类

1. 并发安全: 互斥同步, 非互斥同步, 无同步方案
2. 管理线程, 提高效率
3. 线程协作

## 线程池

### 线程池介绍

- 反复创建线程开销大
- 过多的线程会占用太多线程

---
优点:

- 加快响应速度
- 合理利用cpu和内存
- 方便管理

### 创建和停止线程池

#### 线程池构造函数的参数

- corePoolSIze 核心线程数
- maxPoolSize 最大线程数
- keepAliveTime 保持存活时间
- unit 时间单位
- workQueue 工作队列
- threadFactory 线程工厂
- handler 拒绝策略

是否新增线程的判断顺序:

1. corePoolSize
2. workQueue
3. maxPoolSize

---

- ArrayBlockingQueue 有界队列
- LinkedBlockingQueue 无界队列
- SynchronousQueue 交换队列(size=0)

---

```java
/**
     * Creates a new {@code ThreadPoolExecutor} with the given initial
     * parameters.
     *
     * @param corePoolSize the number of threads to keep in the pool, even
     *        if they are idle, unless {@code allowCoreThreadTimeOut} is set
     * @param maximumPoolSize the maximum number of threads to allow in the
     *        pool
     * @param keepAliveTime when the number of threads is greater than
     *        the core, this is the maximum time that excess idle threads
     *        will wait for new tasks before terminating.
     * @param unit the time unit for the {@code keepAliveTime} argument
     * @param workQueue the queue to use for holding tasks before they are
     *        executed.  This queue will hold only the {@code Runnable}
     *        tasks submitted by the {@code execute} method.
     * @param threadFactory the factory to use when the executor
     *        creates a new thread
     * @param handler the handler to use when execution is blocked
     *        because the thread bounds and queue capacities are reached
     * @throws IllegalArgumentException if one of the following holds:<br>
     *         {@code corePoolSize < 0}<br>
     *         {@code keepAliveTime < 0}<br>
     *         {@code maximumPoolSize <= 0}<br>
     *         {@code maximumPoolSize < corePoolSize}
     * @throws NullPointerException if {@code workQueue}
     *         or {@code threadFactory} or {@code handler} is null
     */
    public ThreadPoolExecutor(int corePoolSize,
                              int maximumPoolSize,
                              long keepAliveTime,
                              TimeUnit unit,
                              BlockingQueue<Runnable> workQueue,
                              ThreadFactory threadFactory,
                              RejectedExecutionHandler handler)
```

### 常见线程池的特点和用法

```java
    public static ExecutorService newFixedThreadPool(int nThreads, ThreadFactory threadFactory) {
        return new ThreadPoolExecutor(nThreads, nThreads, 0L, TimeUnit.MILLISECONDS,new LinkedBlockingQueue<Runnable>(), threadFactory);
    }
   public static ExecutorService newSingleThreadExecutor() {
        return new FinalizableDelegatedExecutorService
            (new ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>()));
    }
    public static ExecutorService newSingleThreadExecutor(ThreadFactory threadFactory) {
        return new FinalizableDelegatedExecutorService
            (new ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>(), threadFactory));
    }
    public static ExecutorService newCachedThreadPool() {
        return new ThreadPoolExecutor(0, Integer.MAX_VALUE, 60L, TimeUnit.SECONDS, new SynchronousQueue<Runnable>());
    }
    public static ExecutorService newCachedThreadPool(ThreadFactory threadFactory) {
        return new ThreadPoolExecutor(0, Integer.MAX_VALUE, 60L, TimeUnit.SECONDS, new SynchronousQueue<Runnable>(), threadFactory);
    }
    public static ScheduledExecutorService newSingleThreadScheduledExecutor() {
        return new DelegatedScheduledExecutorService (new ScheduledThreadPoolExecutor(1));
    }
    public static ScheduledExecutorService newSingleThreadScheduledExecutor(ThreadFactory threadFactory) {
        return new DelegatedScheduledExecutorService   (new ScheduledThreadPoolExecutor(1, threadFactory));
    }
    public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize) {
        return new ScheduledThreadPoolExecutor(corePoolSize);
    }
```

> 线程池的线程数量设定为多少比较合适?

- CPU密集型(加密, 计算hash): 最佳线程数为CPU核心数的1-2倍左右
- 耗时I/O型(读写数据库, 文件, 网络读写等): 最佳线程数一般会大于cpu核心数很多倍, 以JVM线程监控显示繁忙情况为依据,保证线程空闲可以衔接上 -> 线程数 = CPU核心数 * (1 + 平均等待时间 / 平均工作时间)

---

- fixedThreadPool: 固定线程数
- cachedThreadPool: 可缓存, 回收
- scheduledThreadPool: 定时执行
- singleThreadPool: 唯一线程
- workStealingPool: fork/join

> 停止线程池的正确方法

- `java.util.concurrent.ThreadPoolExecutor#isShutdown()`
- `java.util.concurrent.ThreadPoolExecutor#isTerminated()`
- `java.util.concurrent.ThreadPoolExecutor#awaitTermination()`
- `java.util.concurrent.ThreadPoolExecutor#shutdown()`
  - 通知线程池关闭(需要执行完提交的所有任务)
- `java.util.concurrent.ThreadPoolExecutor#shutdownNow()`

```java
    public List<Runnable> shutdownNow() {
        List<Runnable> tasks;
        final ReentrantLock mainLock = this.mainLock;
        mainLock.lock();
        try {
            checkShutdownAccess();
            advanceRunState(STOP);
            interruptWorkers();
            tasks = drainQueue();
        } finally {
            mainLock.unlock();
        }
        tryTerminate();
        return tasks;
    }
```

> 四种拒绝策略

- AbortPolicy: 直接拒绝(抛异常)
- DiscardPolicy: 直接丢掉
- DiscardOldestPolicy: 丢弃最老的线程
- CallerRunsPolicy: 调用者执行当前提交线程

### 组成部分

- 线程池管理器
- 工作线程
- 任务队列
- 任务接口(Task)

### 线程池状态

- RUNNING: 接受新任务并处理排队任务
- SHUTDOWN: 不接受新任务, 但处理排队任务
- STOP: 不接受新任务, 不处理排队任务, 并中断正在运行的任务
- TIDYING: 所有任务都已终止, workerCount为零时, 线程会切换到TIDYING状态, 并将运行terminate()方法
- TERMINATED: terminate()运行完成

### 注意事项

- 注意任务堆积
- 避免线程数过度增加
- 排查线程泄露
