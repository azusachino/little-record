# Golang 经验

## $GOPATH/go.mod exists but should not

开启模块支持后，并不能与$GOPATH共存,所以把项目从$GOPATH 中移出即可

解决办法：

打开设置，将当前路径移出 Project GOPATH

## Go 内存泄漏

`time.After`

```go
// 在一定时间（根据所传入的 Duration）后主动返回 time.Time 类型的 channel 消息。
func After(d Duration) <-chan Time
```

常见案例

```go
func main() {
    ch := make(chan string)
    go func() {
        time.Sleep(time.Second * 3)
        ch <- "任务完成"
    }()

    select {
    case _ = <-ch:
    case <-time.After(time.Second * 1):
        fmt.Println("任务超时！！！")
    }
}
```

出现内存泄漏的代码

```go
func main() {
    ch := make(chan int, 10)
    go func() {
        in := 1
        for {
            in++
            ch <- in
        }
    }()

    for {
        select {
        case _ = <-ch:
            // do something...
            continue
        case <-time.After(3 * time.Minute):
            fmt.Printf("现在是：%d，还不睡觉吗？", time.Now().Unix())
        }
    }
}
```

在运行一段时间后，利用 top 命令查看资源，可以看到当前 go 程序的内存占用达到了 10+G，并且还在持续增长。

使用 **`go tool pprof`** 分析 go 工程中函数内存申请情况，如下图

![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/0d55a56a-817d-4405-9cee-3fa25f385666/Untitled.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/0d55a56a-817d-4405-9cee-3fa25f385666/Untitled.png)

可以发现是不断地调用 `time.After`,从而导致计时器 `time.NewTimer` 不断创建和内存申请。

原因在于 for+select，再加上 `time.After` 的组合会导致内存泄露。因为 for 在循环时，就会调用都 select 语句，因此在每次进行 select 时，都会重新初始化一个全新的计时器（Timer）。这个计时器，是在 3 分钟后才会被触发去执行某些事，但重点在于计时器激活后，却又发现和 select 之间没有引用关系了，因此很合理的也就被 GC 给清理掉了。被抛弃的 `time.After` 的定时任务还是在时间堆中等待触发，在定时任务未到期之前，是不会被 GC 清除的。

### 解决办法

```go
func main() {
    timer := time.NewTimer(3 * time.Minute)
    defer timer.Stop()

    ...
    for {
        select {
        ...
        case <-timer.C:
            fmt.Printf("现在是：%d，你好呀！", time.Now().Unix())
        }
    }
}
```

## Goroutine 内存泄漏

### 泄露原因

- Goroutine 内正在进行 channel/mutex 等读写操作，但由于逻辑问题，某些情况下会被一直阻塞。
- Goroutine 内的业务逻辑进入死循环，资源一直无法释放。
- Goroutine 内的业务逻辑进入长时间等待，有不断新增的 Goroutine 进入等待。

**channel 使用不当**:

发送不接收

```go
func main() {
    for i := 0; i < 4; i++ {
        queryAll()
        fmt.Printf("goroutines: %d\n", runtime.NumGoroutine())
    }
}

func queryAll() int {
    ch := make(chan int)
    for i := 0; i < 3; i++ {
        go func() { ch <- query() }()
     }
    return <-ch
}

func query() int {
    n := rand.Intn(100)
    time.Sleep(time.Duration(n) * time.Millisecond)
    return n
}
// goroutines: 3
// goroutines: 5
// goroutines: 7
// goroutines: 9
```

原因在于 channel 均已经发送（3 个），接收端并没有完全接收（只返回 1 个 ch），造成了 Goroutine 泄漏。

接收不发送

```go
func main() {
    defer func() {
        fmt.Println("goroutines: ", runtime.NumGoroutine())
    }()

    var ch chan struct{}
    go func() {
        ch <- struct{}{}
    }()

    time.Sleep(time.Second)
}
// goroutines:  2
```

与 “发送不接收” 两者是相对的，channel 接收了值，但是不发送的话，同样会造成阻塞。

nil channel

```go
func main() {
    defer func() {
        fmt.Println("goroutines: ", runtime.NumGoroutine())
    }()

    var ch chan int // => ch := make(chan int)
    go func() {
        <-ch
    }()
    // ch <- 0
    time.Sleep(time.Second)
}
// goroutines:  2
```

channel 如果忘记初始化，那么无论你是读，还是写操作，都会造成阻塞。

奇怪的慢等待

```go
func main() {
    for {
        go func() {
            _, err := http.Get("https://www.xxx.com/")
            if err != nil {
                fmt.Printf("http.Get err: %v\n", err)
            }
            // do something...
    }()

    time.Sleep(time.Second * 1)
    fmt.Println("goroutines: ", runtime.NumGoroutine())
 }
}
// goroutines:  5
// goroutines:  9
// goroutines:  13
// goroutines:  17
// goroutines:  21
// goroutines:  25
// ...
```

第三方接口，有时候会很慢，久久不返回响应结果。恰好，Go 语言中默认的 `http.Client` 是没有设置超时时间的。

因此就会导致一直阻塞，一直阻塞就一直爽，Goroutine 自然也就持续暴涨，不断泄露，最终占满资源，导致事故。

```go
// 设置超时时间、限流、熔断
httpClient := http.Client{
    Timeout: time.Second * 10,
}
```

互斥锁忘记解锁

```go
func main() {
    total := 0
    defer func() {
        time.Sleep(time.Second)
        fmt.Println("total: ", total)
        fmt.Println("goroutines: ", runtime.NumGoroutine())
 }()

    var mutex sync.Mutex
    for i := 0; i < 10; i++ {
        go func() {
            mutex.Lock()
            total += 1
        }()
    }
}
// total:  1
// goroutines:  10
```

第一个互斥锁 `sync.Mutex` 加锁了，但是他可能在处理业务逻辑，又或是忘记 `Unlock` 了。

因此导致后面的所有 `sync.Mutex` 想加锁，却因未释放又都阻塞住了。

```go
var mutex sync.Mutex
    for i := 0; i < 10; i++ {
        go func() {
            mutex.Lock()
            defer mutex.Unlock()
            total += 1
        }()
    }
```

同步锁使用不当

```go
func handle(v int) {
    var wg sync.WaitGroup
    wg.Add(5)
    for i := 0; i < v; i++ {
        fmt.Println(".....了")
        wg.Done()
    }
    wg.Wait()
}

func main() {
    defer func() {
        fmt.Println("goroutines: ", runtime.NumGoroutine())
    }()

    go handle(3)
    time.Sleep(time.Second)
}
```

由于 `wg.Add` 的数量与 `wg.Done` 数量并不匹配，因此在调用 `wg.Wait` 方法后一直阻塞等待。

```go
    var wg sync.WaitGroup
    for i := 0; i < v; i++ {
        wg.Add(1)
        defer wg.Done()
        fmt.Println("脑子进煎鱼了")
    }
    wg.Wait()
```

### 排查方法

调用 `runtime.NumGoroutine` 方法来获取 Goroutine 的运行数量，进行前后一比较，就能知道有没有泄露了。

## GMP

Go scheduler 的主要功能是针对在处理器上运行的 OS 线程分发可运行的 Goroutine，而我们一提到调度器，就离不开三个经常被提到的缩写，分别是：

- G：Goroutine，实际上我们每次调用 `go func` 就是生成了一个 G。
- P：Processor，处理器，一般 P 的数量就是处理器的核数，可以通过 `GOMAXPROCS` 进行修改。
- M：Machine，系统线程。

![https://cdn.jsdelivr.net/gh/timqi/Blog@gh-pages/i/2020-05-15-1.jpeg](https://cdn.jsdelivr.net/gh/timqi/Blog@gh-pages/i/2020-05-15-1.jpeg)

1. 当我们执行 `go func()` 时，实际上就是创建一个全新的 Goroutine，我们称它为 G。
2. 新创建的 G 会被放入 P 的本地队列（Local Queue）或全局队列（Global Queue）中，准备下一步的动作。需要注意的一点，这里的 P 指的是创建 G 的 P。
3. 唤醒或创建 M 以便执行 G。
4. 不断地进行事件循环
5. 寻找在可用状态下的 G 进行执行任务
6. 清除后，重新进入事件循环

### M 的限制

第一，要知道**在协程的执行中，真正干活的是 GPM 中的哪一个**？

那势必是 M（系统线程） 了，因为 G 是用户态上的东西，最终执行都是得映射，对应到 M 这一个系统线程上去运行。

那么 M 有没有限制呢？

答案是：有的。在 Go 语言中，**M 的默认数量限制是 10000**，如果超出则会报错。

若确切是需要那么多，还可以通过 `debug.SetMaxThreads` 方法进行设置。

### G 的限制

第二，那 G 呢，Goroutine 的创建数量是否有限制？

答案是：没有。但**理论上会受内存的影响。**

Goroutine 创建所需申请的 2-4k 是需要连续的内存块。

### P 的限制

第三，那 P 呢，P 的数量是否有限制，受什么影响？

答案是：有限制。**P 的数量受环境变量 `GOMAXPROCS` 的直接影响**。

## Go 的逃逸分析

- 堆（Heap）：一般来讲是人为手动进行管理，手动申请、分配、释放。一般所涉及的内存大小并不定，一般会存放较大的对象。另外其分配相对慢，涉及到的指令动作也相对多。
- 栈（Stack）：由编译器进行管理，自动申请、分配、释放。一般不会太大，我们常见的函数参数（不同平台允许存放的数量不同），局部变量等等都会存放在栈上。

```go
type User struct {
 ID     int64
 Name   string
 Avatar string
}

func GetUserInfo() *User {
 return &User{ID: 13746731, Name: "EDDYCJY", Avatar: "https://avatars0.githubusercontent.com/u/13746731"}
}

func main() {
 _ = GetUserInfo()
}
```

main 调用 `GetUserInfo` 后返回的 `&User{...}`。这个变量是分配到栈上了呢，还是分配到堆上了？

答: `GetUserInfo()` 返回的是指针对象，引用被返回到了方法之外了。因此编译器会把该对象分配到堆上，而不是栈上。

## **怎么确定是否逃逸**

第一，通过编译器命令，就可以看到详细的逃逸分析过程。而指令集 `-gcflags` 用于将标识参数传递给 Go 编译器，涉及如下：

- `m` 会打印出逃逸分析的优化策略，实际上最多总共可以用 4 个 `m`，但是信息量较大，一般用 1 个就可以了。
- `l` 会禁用函数内联，在这里禁用掉 inline 能更好的观察逃逸情况，减少干扰。

```bash
go build -gcflags '-m -l' main.go
```

第二，通过反编译命令查看

```bash
go tool compile -S main.go
```

注：可以通过 `go tool compile -help` 查看所有允许传递给编译器的标识参数。

---

- 静态分配到栈上，性能一定比动态分配到堆上好。
- 底层分配到堆，还是栈。实际上对你来说是透明的，不需要过度关心。
- 每个 Go 版本的逃逸分析都会有所不同（会改变，会优化）。
- 直接通过 `go build -gcflags '-m -l'` 就可以看到逃逸分析的过程和结果。
- 到处都用指针传递并不一定是最好的，要用对。

## Go Context

本质上 Go 语言是基于 context 来实现和搭建了各类 goroutine 控制的，并且与 select-case 联合，就可以实现进行上下文的截止时间、信号控制、信息传递等跨 goroutine 的操作，是 Go 语言协程的重中之重。

```go
func main() {
 parentCtx := context.Background()
 ctx, cancel := context.WithTimeout(parentCtx, 1*time.Millisecond)
 defer cancel()

 select {
 case <-time.After(1 * time.Second):
  fmt.Println("overslept")
 case <-ctx.Done():
  fmt.Println(ctx.Err())
 }
}
```

- WithCancel：基于父级 context，创建一个可以取消的新 context。
- WithDeadline：基于父级 context，创建一个具有截止时间（Deadline）的新 context。
- WithTimeout：基于父级 context，创建一个具有超时时间（Timeout）的新 context。
- Background：创建一个空的 context，一般常用于作为根的父级 context。
- TODO：创建一个空的 context，一般用于未确定时的声明使用。
- WithValue：基于某个 context 创建并存储对应的上下文信息。

在标准库 context 的设计上，一共提供了四类 context 类型来实现上述接口。分别是 emptyCtx、cancelCtx、timerCtx 以及 valueCtx。

## Go Chan

本质上 channel 在设计上就是环形队列。其包含发送方队列、接收方队列，加上互斥锁 mutex 等结构。

```go
// src/runtime/chan.go
type hchan struct {
 qcount   uint
 dataqsiz uint
 buf      unsafe.Pointer
 elemsize uint16
 closed   uint32
 elemtype *_type
 sendx    uint
 recvx    uint
 recvq    waitq
 sendq    waitq

 lock mutex
}
```

- qcount：队列中的元素总数量。
- dataqsiz：循环队列的长度。
- buf：指向长度为 dataqsiz 的底层数组，仅有当 channel 为缓冲型的才有意义。
- elemsize：能够接受和发送的元素大小。
- closed：是否关闭。
- elemtype：能够接受和发送的元素类型。
- sendx：已发送元素在循环队列中的索引位置。
- recvx：已接收元素在循环队列中的索引位置。
- recvq：接受者的 sudog 等待队列（缓冲区不足时阻塞等待的 goroutine）。
- sendq：发送者的 sudog 等待队列。

```go
type waitq struct {
 first *sudog
 last  *sudog
}
type sudog struct {
 g *g

 next *sudog
 prev *sudog
 elem unsafe.Pointer
 ...
}
```

- g：指向当前的 goroutine。
- next：指向下一个 g。
- prev：指向上一个 g。
- elem：数据元素，可能会指向堆栈。
