# Docker

Docker 容器本质上是宿主机上的进程。Docker 通过 namespace 实现了资源隔离，通过 cgroups 实现了资源限制，通过写时复制机制（copy-on-write）实现了高效的文件操作。

## commands

![.](https://res.weread.qq.com/wrepub/epub_26211797_7)

## Kernel

### Namespace 资源隔离

`CLONE_*`标志位

- UTS 主机和域名 CLONE_NEWUTS
- IPC 信号量、消息队列和共享内存 CLONE_NEWIPC
- PID 进程编号 CLONE_NEWPID
- Network 网络设备、网络栈、端口等 CLONE_NEWNET
- Mount 挂载点(文件系统) CLONE_NEWNS
- User 用户和用户组 CLONE_NEWUSER

**namespace API 操作的 4 种方式**:

namespace 的 API 包括 clone()、setns()以及 unshare()，还有/proc 下的部分文件。

- 通过`clone()`在创建新进程的同时创建 namespace

clone()实际上是 Linux 系统调用 fork()的一种更通用的实现方式，它可以通过 flags 来控制使用多少功能。

```c
int clone(int(*child_func)(void*), void *child_stack, int flags, void *args);
// child_func传入子进程运行的程序主函数
// child_stack传入子进程使用的栈空间
// flags表示使用哪些CLONE_*标志位
// args 传入用户参数
```

- 查看`/proc/[pid]/ns`文件

```sh
ls -l /proc/$$/ns  # $$是shell种表示当前运行的进程Id
```

- 通过`setns()`加入一个已经存在的 namespace

在 Docker 种，使用`docker exec`命令在已经运行着的容器中执行一个新的命令，就需要用到该方法。通过`setns()`系统调用，进程从原来的 namespace 加入某个已经存在的 namespace()。

为了把新加入的 namespace 利用起来，需要使用`execvp()`系列函数，该函数执行用户命令，最常用的就是调用`/bin/bash`并接受参数，运行起一个 shell。

```c
int setns(int fd, int nstype);
// fd表示要加入namespace的文件描述符，它是一个指向/proc/[pid]/ns目录的文件描述符
// nstype让调用者可以检查fd指向的namespace类型是否符合实际要求。0表示不检查

fd = open(argv[1], 0_RDONLY); // 获取namespace文件描述符
setns(fd, 0);
execvp(argv[2], &argv[2]); // 执行程序

./setns-test /proc/[12345]/ns /bin/bash
```

- 通过`unshare()`在原先进程上进行 namespace 隔离
- `fork()`系统调用

当程序调用 fork()函数时，系统会创建新的进程，为其分配资源，例如存储数据和代码的空间，然后把原来进程的所有值都复制到新进程中，只有少量数值与原来的进程值不同，相当于复制了本身。

fork()的神奇之处在于它仅仅被调用一次，却能够返回两次（父进程与子进程各返回一次），通过返回值的不同就可以区分父进程与子进程。它可能有以下 3 种不同的返回值：

- 在父进程中，fork()返回新创建子进程的进程 ID；
- 在子进程中，fork()返回 0；
- 如果出现错误，fork()返回一个负值。

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork!");
    } else if (fpid == 0) {
        printf("I am a child. Process id is %d\n", getpid());
    } else {
        printf("I am parent. Process Id is %d\n", getpid());
    }
    return 0;

}
```

```sh
gcc -Wall fork_example.c && ./a.out
## output
I am parent. Process Id is 4168163
I am a child. Process id is 4168164
```

**UTS namespace**:

```c
#define _GNU_SOURCE
#include "sys/types.h"
#include "sys/wait.h"
#include "stdio.h"
#include "sched.h"
#include "signal.h"
#include "unistd.h"

#define STACK_SIZE (1024*1024)
static char child_stack[STACK_SIZE];

char* const child_args[] = {"/bin/bash", NULL};

int child_main(void *args) {
    printf("in child process!\n");
    sethostname("NewNamespace", 12); // set new hostname in child process
    execv(child_args[0], child_args);
    return 1;
}

int main() {
    printf("program starts! \n");
    int child_pid = clone(child_main, child_stack+STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL); // refer new signal CLONE_NEWUTS
    waitpid(child_pid, NULL, 0);
    printf("program exited! \n");
    return 0;
}
```

```sh
gcc -Wall uts.c -o uts.o && ./uts.o
## output
program starts!
in child process!
[root@NewNamespace samples]# exit
exit
program exited!
```

**IPC namespace**:

进程间通信（Inter-Process Communication, IPC）涉及的 IPC 资源包括常见的信号量、消息队列和共享内存。申请 IPC 资源就申请了一个全局唯一的 32 位 ID，所以 IPC namespace 中实际上包含了系统 IPC 标识符以及实现 POSIX 消息队列的文件系统。在同一个 IPC namespace 下的进程彼此可见，不同 IPC namespace 下的进程则互相不可见。

```sh
# 创建一个mq
ipcmk -Q
# 查看已开启的mq
ipcs -q
# 在子进程种查看mq
gcc -Wall ipc.c -o ipc.o && ./ipc.o
ipcs -q
```

**PID namespace**:

PID namespace 隔离非常实用，它对进程 PID 重新标号，即两个不同 namespace 下的进程可以有相同的 PID。每个 PID namespace 都有自己的计数程序。内核为所有的 PID namespace 维护了一个树状结构，最顶层的是系统初始时创建的，被称为 root namespace。它创建的新 PID namespace 被称为 child namespace（树的子节点），而原先的 PID namespace 就是新创建的 PID namespace 的 parentnamespace（树的父节点）。通过这种方式，不同的 PID namespaces 会形成一个层级体系。所属的父节点可以看到子节点中的进程，并可以通过信号等方式对子节点中的进程产生影响。反过来，子节点却不能看到父节点 PID namespace 中的任何内容，由此产生如下结论。

- 每个 PID namespace 中的第一个进程“PID 1”，都会像传统 Linux 中的 init 进程一样拥有特权，起特殊作用。
- 一个 namespace 中的进程，不可能通过 kill 或 ptrace 影响父节点或者兄弟节点中的进程，因为其他节点的 PID 在这个 namespace 中没有任何意义。
- 如果你在新的 PID namespace 中重新挂载/proc 文件系统，会发现其下只显示同属一个 PID namespace 中的其他进程。
- 在 root namespace 中可以看到所有的进程，并且递归包含所有子节点中的进程。

- PID namespace 中的 init 进程

在传统的 Unix 系统中，PID 为 1 的进程是 init，地位非常特殊。它作为所有进程的父进程，维护一张进程表，不断检查进程的状态，一旦有某个子进程因为父进程错误成为了“孤儿”进程，init 就会负责收养这个子进程并最终回收资源，结束进程。

- 信号与 init 进程

内核还为 PID namespace 中的 init 进程赋予了其他特权——信号屏蔽。如果 init 中没有编写处理某个信号的代码逻辑，那么与 init 在同一个 PID namespace 下的进程（即使有超级权限）发送给它的该信号都会被屏蔽。这个功能的主要作用是防止 init 进程被误杀。

- 挂载 proc 文件系统

如果在新的 PID namespace 中使用 ps 命令查看，看到的还是所有的进程，因为与 PID 直接相关的/proc 文件系统（procfs）没有挂载到一个与原/proc 不同的位置。如果只想看到 PID namespace 本身应该看到的进程，需要重新挂载/proc。

- unshare() & setns()

unshare()允许用户在原有进程中建立命名空间进行隔离。但创建了 PIDnamespace 后，原先 unshare()调用者进程并不进入新的 PID namespace，接下来创建的子进程才会进入新的 namespace，这个子进程也就随之成为新 namespace 中的 init 进程。类似地，调用 setns()创建新 PID namespace 时，调用者进程也不进入新的 PIDnamespace，而是随后创建的子进程进入。

**Mount namespace**:

可以通过`/proc/[pid]/mounts` 查看到所有挂载在当前 namespace 中的文件系统，还可以通过`/proc/[pid]/mountstats` 看到 mount namespace 中文件设备的统计信息，包括挂载文件的名字、文件系统类型、挂载位置等。

挂载传播定义了挂载对象（mount object）之间的关系，这样的关系包括共享关系和从属关系，系统用这些关系决定任何挂载对象中的挂载事件如何传播到其他挂载对象。

- 共享关系
- 从属关系

传播事件的挂载对象称为共享挂载；接收传播事件的挂载对象称为从属挂载；同时兼有前述两者特征的挂载对象称为共享/从属挂载；既不传播也不接收传播事件的挂载对象称为私有挂载；另一种特殊的挂载对象称为不可绑定的挂载，它们与私有挂载相似，但是不允许执行绑定挂载，即创建 mount namespace 时这块文件对象不可被复制。

**Network namespace**:

network namespace 主要提供了关于网络资源的隔离，包括网络设备、IPv4 和 IPv6 协议栈、IP 路由表、防火墙、`/proc/net` 目录、`/sys/class/net` 目录、套接字（socket）等。一个物理的网络设备最多存在于一个 network namespace 中，可以通过创建 veth pair（虚拟网络设备对：有两端，类似管道，如果数据从一端传入另一端也能接收到，反之亦然）在不同的 network namespace 间创建通道，以达到通信目的。

**User namespace**:

user namespace 主要隔离了安全相关的标识符（identifier）和属性（attribute），包括用户 ID、用户组 ID、root 目录、key（指密钥）以及特殊权限。

### cgroups 资源隔离

cgroups 不仅可以限制被 namespace 隔离起来的资源，还可以为资源设置权重、计算使用量、操控任务（进程或线程）启停等。

**官方定义**:

cgroups 是 Linux 内核提供的一种机制，这种机制可以根据需求把一系列系统任务及其子任务整合（或分隔）到按资源划分等级的不同组内，从而为系统资源管理提供一个统一的框架。

**特点**:

- cgroups 的 API 以一个伪文件系统的方式实现，用户态的程序可以通过文件操作实现 cgroups 的组织管理。
- cgroups 的组织管理操作单元可以细粒度到线程级别，另外用户可以创建和销毁 cgroup，从而实现资源再分配和管理。
- 所有资源管理的功能都以子系统的方式实现，接口统一。
- 子任务创建之初与其父任务处于同一个 cgroups 的控制组。

**作用**:

- 资源限制：cgroups 可以对任务使用的资源总额进行限制。如设定应用运行时使用内存的上限，一旦超过这个配额就发出 OOM（Out of Memory）提示。
- 优先级分配：通过分配的 CPU 时间片数量及磁盘 IO 带宽大小，实际上就相当于控制了任务运行的优先级。
- cgroups 可以统计系统的资源使用量，如 CPU 使用时长、内存用量等，这个功能非常适用于计费。
- 任务控制：cgroups 可以对任务执行挂起、恢复等操作。

**术语表**:

- task（任务）：在 cgroups 的术语中，任务表示系统的一个进程或线程。
- cgroup（控制组）:cgroups 中的资源控制都以 cgroup 为单位实现。cgroup 表示按某种资源控制标准划分而成的任务组，包含一个或多个子系统。一个任务可以加入某个 cgroup，也可以从某个 cgroup 迁移到另外一个 cgroup。
- subsystem（子系统）:cgroups 中的子系统就是一个资源调度控制器。比如 CPU 子系统可以控制 CPU 时间分配，内存子系统可以限制 cgroup 内存使用量。
- hierarchy（层级）：层级由一系列 cgroup 以一个树状结构排列而成，每个层级通过绑定对应的子系统进行资源控制。层级中的 cgroup 节点可以包含零或多个子节点，子节点继承父节点挂载的子系统。整个操作系统可以有多个层级。

**组织结构与基本规则**:

系统中的多个 cgroup 构成的层级并非单根结构，可以允许存在多个。

- 规则 1：同一层级可以附加一个或多个子系统
- 规则 2：一个子系统可以附加到多个层级，当且仅当目标层级只有唯一一个子系统时
- 规则 3：系统每次新建一个层级时，该系统上的所有任务默认加入这个新建层级的初始化 cgroup，这个 cgroup 也被称为 root cgroup
- 规则 4：任务在 fork/clone 自身时创建的子任务默认与原任务在同一个 cgroup 中，但是子任务允许被移动到不同的 cgroup 中

**子系统简介**:

- blkio：可以为块设备设定输入/输出限制，比如物理驱动设备（包括磁盘、固态硬盘、USB 等）。
- cpu：使用调度程序控制任务对 CPU 的使用。
- cpuacct：自动生成 cgroup 中任务对 CPU 资源使用情况的报告。
- cpuset：可以为 cgroup 中的任务分配独立的 CPU（此处针对多处理器系统）和内存。
- devices：可以开启或关闭 cgroup 中任务对设备的访问。
- freezer：可以挂起或恢复 cgroup 中的任务。
- memory：可以设定 cgroup 中任务对内存使用量的限定，并且自动生成这些任务对内存资源使用情况的报告。
- perf_event：使用后使 cgroup 中的任务可以进行统一的性能测试。
- net_cls:Docker 没有直接使用它，它通过使用等级识别符（classid）标记网络数据包，从而允许 Linux 流量控制程序（Traffic Controller, TC）识别从具体 cgroup 中生成的数据包。

```sh
# 查看cup子系统下的文件信息
ls /sys/fs/cgroup/cpu
# output
cgroup.clone_children  cpuacct.stat       cpuacct.usage_percpu       cpuacct.usage_sys   cpu.cfs_quota_us   cpu.shares  init.scope         system.slice  YunJing
cgroup.procs           cpuacct.usage      cpuacct.usage_percpu_sys   cpuacct.usage_user  cpu.rt_period_us   cpu.stat    notify_on_release  tasks
cgroup.sane_behavior   cpuacct.usage_all  cpuacct.usage_percpu_user  cpu.cfs_period_us   cpu.rt_runtime_us  docker      release_agent      user.slice
# 在/sys/fs/cgroup/cpu下创建控制组，会自动生成一系列文件
sudo mkdir iris
ls iris
#output
cgroup.clone_children  cpuacct.usage         cpuacct.usage_percpu_sys   cpuacct.usage_user  cpu.rt_period_us   cpu.stat
cgroup.procs           cpuacct.usage_all     cpuacct.usage_percpu_user  cpu.cfs_period_us   cpu.rt_runtime_us  notify_on_release
cpuacct.stat           cpuacct.usage_percpu  cpuacct.usage_sys          cpu.cfs_quota_us    cpu.shares         tasks

# 限制PID xxx的CPU使用配额
echo 814085 >> /sys/fs/cgroup/cpu/iris/tasks
echo 20000 > /sys/fs/cgroup/cpu/iris/cpu.cfs_quota_us # 将cpu限制为最高使用20%
```

**cgroups 实现方式和工作原理简介**:

cgroups 的实现本质上是给任务挂上钩子，当任务运行的过程中涉及某种资源时，就会触发钩子上所附带的子系统进行检测，根据资源类别的不同，使用对应的技术进行资源限制和优先级分配。

- cgroups 如何判断资源超限及超出限额之后的措施

memory 子系统，会在描述内存状态的“mm_struct”结构体中记录它所属的 cgroup，当进程需要申请更多内存时，就会触发 cgroup 用量检测，用量超过 cgroup 规定的限额，则拒绝用户的内存申请，否则就给予相应内存并在 cgroup 的统计信息中记录。实际实现要比以上描述复杂得多，不仅需考虑内存的分配与回收，还需考虑不同类型的内存如 cache（缓存）和 swap（交换区内存拓展）等。

进程所需的内存超过它所属的 cgroup 最大限额以后，如果设置了 OOM Control（内存超限控制），那么进程就会收到 OOM 信号并结束；否则进程就会被挂起，进入睡眠状态，直到 cgroup 中其他进程释放了足够的内存资源为止。Docker 中默认是开启 OOM Control 的。

- cgroup 与任务之间的关联关系

cgroup 与任务之间是多对多的关系，所以它们并不直接关联，而是通过一个中间结构把双向的关联信息记录起来。每个任务结构体 task_struct 中都包含了一个指针，可以查询到对应 cgroup 的情况，同时也可以查询到各个子系统的状态，这些子系统状态中也包含了找到任务的指针，不同类型的子系统按需定义本身的控制信息结构体，最终在自定义的结构体中把子系统状态指针包含进去，然后内核通过 container_of（这个宏可以通过一个结构体的成员找到结构体自身）等宏定义来获取对应的结构体，关联到任务，以此达到资源限制的目的。
