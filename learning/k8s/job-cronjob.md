# 任务调用 Job

## Job

### Job 基本使用

Job 主要是用来任务调用，可以一个或多个 Pod，并确保指定数量的 Pod 可以成功执行到进程正常结束。

```yml
apiVersion: batch/v1
kind: Job
metadata:
  name: pi
spec:
  template:
    spec:
      containers:
        - name: pi
          image: perl
          command: ["perl", "-Mbignum=bpi", "-wle", "print bpi(2000)"]
      restartPolicy: Never # 只运行一次
  backoffLimit: 4
```

创建对象后，Pod 模板中，被自动加上了一个 controller-uid=< 一个随机字符串 > 这样的 Label。而这个 Job 对象本身，则被自动加上了这个 Label 对应的 Selector，从而 保证了 Job 与它所管理的 Pod 之间的匹配关系。这个 uid 避免了不同 Job 对象的 Pod 不会重合。

```bash
$ kubectl create -f job.yaml

$ kubectl describe jobs/pi

Name:           pi
Namespace:      default
Selector:       controller-uid=cf78ebe4-07f9-4234-b8f9-2fe92df352ea
Labels:         controller-uid=cf78ebe4-07f9-4234-b8f9-2fe92df352ea
                job-name=pi
Annotations:    Parallelism:  1
Completions:    1
...
Pods Statuses:  0 Running / 1 Succeeded / 0 Failed
Pod Template:
  Labels:  controller-uid=cf78ebe4-07f9-4234-b8f9-2fe92df352ea
           job-name=pi
  Containers:
   pi:
    Image:      resouer/ubuntu-bc
    ...
Events:
  Type    Reason            Age   From            Message
  ----    ------            ----  ----            -------
  Normal  SuccessfulCreate  29m   job-controller  Created pod: pi-g9fs4
  Normal  Completed         27m   job-controller  Job completed
```

### 重要参数

- spec.backoffLimit 重试次数
- restartPolicy 重启策略
- spec.activeDeadlineSeconds 最长运行时间

### Job 的并行任务

- spec.parallelism 表示一个 Job 在任意时间最多可以启动多少个 Pod 同时运行；
- spec.completions 表示 Job 的最小完成数。

```yml
apiVersion: batch/v1
kind: Job
metadata:
  name: pi
spec:
  parallelism: 2 # 最多只有两个Pod同时运行
  completions: 4
  template:
    spec:
      containers:
        - name: pi
          image: perl
          command: ["perl", "-Mbignum=bpi", "-wle", "print bpi(2000)"]
      restartPolicy: Never
  backoffLimit: 4
```

## CronJob

```yml
apiVersion: batch/v1beta1
kind: CronJob
metadata:
  name: hello
spec:
  schedule: "*/1 * * * *" # 定时器
  jobTemplate:
    spec:
      template:
        spec:
          containers:
            - name: hello
              image: busybox
              args:
                - /bin/sh
                - -c
                - date; echo Hello from the Kubernetes cluster
          restartPolicy: OnFailure
```

如果设置的间隔时间太短，那么可能会导致任务还没执行完成又创建了新的 Pod。此时需要配置**spec.concurrencyPolicy**：

- Allow，这也是默认情况，这意味着这些 Job 可以同时存在；
- Forbid，这意味着不会创建新的 Pod，该创建周期被跳过；
- Replace，这意味着新产生的 Job 会替换旧的、没有执行完的 Job。
