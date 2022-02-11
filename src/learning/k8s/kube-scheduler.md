# 调度器

调度器的主要职责，就是为一个新创建出来的 Pod，寻找一个最合适的节点（Node）。kube-scheduler 是 Kubernetes 集群的默认调度器。

默认调度器会首先调用一组 Filter 过滤器，也就是使用相应的 Predicates 的调度算法来进行过滤。然后，再调用一组叫作 Priority 的调度算法，来给上一步得到的结果里的每个 Node 打分，然后根据打分来对 Node 进行排序，找出最优节点，如果多个节点都有最高的优先级分数，那么则循环分配，确保平均分配给 pod。

调度算法执行完成后，调度器就需要将 Pod 对象的 nodeName 字段的值，修改为上述 Node 的名字。

## Filter

### GeneralPredicates

### VolumeFilterRule

### 检查调度 Pod 是否满足 Node 本身的某些条件

### check affinity & non-affinity

![.](https://img.luozhiyun.com/20200905190537.png)

## 调度过程流程

![.](https://img.luozhiyun.com/20200905190824.png)
