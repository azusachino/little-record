# I/O

## IO 读写的基础原理

用户程序进行 IO 的读写，依赖于底层的 IO 读写，基本上会用到底层的 read&write 两大系统调用。

具体来说，调用操作系统的 read，是把数据从内核缓冲区复制到进程缓冲区；而 write 系统调用，是把数据从进程缓冲区复制到内核缓冲区。

![.](https://res.weread.qq.com/wrepub/epub_26174369_3)

## 四种主要 IO 模型

- Blocking IO
- Non-Blocking IO
- IO Multiplexing (Reactor)
- Asynchronous IO

![.](https://res.weread.qq.com/wrepub/epub_26174369_4)
![.](https://res.weread.qq.com/wrepub/epub_26174369_5)
![.](https://res.weread.qq.com/wrepub/epub_26174369_6)
![.](https://res.weread.qq.com/wrepub/epub_26174369_7)

Linux 默认最大句柄数 1024。 `ulimit -n 100000`

`/etc/security/limits.conf`

```config
soft nofile 1000000
hard nofile 1000000
```

soft nofile 表示软性极限，hard nofile 表示硬性极限。

## Java NIO
