# [Scheduling, Preemption and Eviction](https://kubernetes.io/docs/concepts/scheduling-eviction/)

In Kubernetes, scheduling refers to making sure that Pods are matched to Nodes so that the kubelet can run them. Preemption is the process of terminating Pods with lower Priority so that Pods with higher Priority can schedule on Nodes. Eviction is the process of terminating one or more Pods on Nodes.

## [Kubernetes Scheduler](https://kubernetes.io/docs/concepts/scheduling-eviction/kube-scheduler/)

Scheduling refers to making sure that Pods are matched to Nodes so that Kubelet can run them.

### `kube-scheduler`

kube-scheduler is the default scheduler for Kubernetes and runs as part of the control plane.

In a cluster, Nodes that meet the scheduling requirements for a Pod are called feasible nodes.

The scheduler finds feasible Nodes for a Pod and then runs a set of functions to score the feasible Nodes and picks a Node with the highest score among the feasible ones to run the Pod. The scheduler then notifies the API server about this decision in a process called binding.

Factors that need taken into account for scheduling decisions include individual and collective resource requirements, hardware / software / policy constraints, affinity and anti-affinity specifications, data locality, inter-workload interference, and so on.

**Node Selection in kube-scheduler**:

1. Filtering => find suitable
2. Scoring => find the most suitable by ranking score

**two ways to configure filtering and ranking behaviour**:

1. `Scheduling Policies` allow you to configure Predicates for filtering and Priorities for scoring.
2. `Scheduling Profiles` allow you to configure Plugins that implement different scheduling stages, including: QueueSort, Filter, Score, Bind, Reserve, Permit, and others. You can also configure the kube-scheduler to run different profiles.

## [Assigning Pods to Nodes](https://kubernetes.io/docs/concepts/scheduling-eviction/assign-pod-node/)

### nodeSelector

**Step1: Attach label to the node**:

```sh
kubectl get nodes
kubectl label nodes <node-name> <label-key>=<label-value>
kubectl get nodes --show-labels
kubectl describe node <node-name>
```

**Step2: Add a nodeSelector field to your pod configuration**:

```sh
kubectl apply -f pod-nginx.yml
# check pod in which node
kubectl get pods -o wide

NAME    READY   STATUS    RESTARTS   AGE   IP           NODE       NOMINATED NODE   READINESS GATES
nginx   1/1     Running   0          6s    172.17.0.3   minikube   <none>           <none>
```

```yml
apiVersion: v1
kind: Pod
metadata:
  name: nginx
  labels:
    env: test
spec:
  containers:
    - name: nginx
      image: nginx
  # adding node selector
  nodeSelector:
    disktype: ssd
```

### Node Isolation/Restriction

保证目标标签不会被 kubelet 修改

Adding labels to Node objects allows targeting pods to specific nodes or groups of nodes. This can be used to ensure specific pods only run on nodes with certain isolation, security, or regulatory properties. When using labels for this purpose, choosing label keys that cannot be modified by the kubelet process on the node is strongly recommended. This prevents a compromised node from using its kubelet credential to set those labels on its own Node object, and influencing the scheduler to schedule workloads to the compromised node.

The `NodeRestriction` admission plugin prevents kubelets from setting or modifying labels with a `node-restriction.kubernetes.io/` prefix. To make use of that label prefix for node isolation:

Ensure you are using the Node authorizer and have enabled the NodeRestriction admission plugin.
Add labels under the `node-restriction.kubernetes.io/` prefix to your Node objects, and use those labels in your node selectors. For example, `example.com.node-restriction.kubernetes.io/fips=true` or `example.com.node-restriction.kubernetes.io/pci-dss=true`.

### Affinity & Anti-Affinity

- `requiredDuringSchedulingIgnoredDuringExecution` -- hard
- `preferredDuringSchedulingIgnoredDuringExecution` -- soft

By using `ignoredDuringExecution`, if labels on a node change at runtime such that the affinity rules on a pod are no longer met, the pod continues to run the the pod. In the future, there will have `requiredDuringSchedulingRequiredDuringExecution` which will evict pods from nodes that cease to satisfy the pod's node affinity requirements.

```yml
apiVersion: v1
kind: Pod
metadata:
  name: with-node-affinity
spec:
  affinity:
    nodeAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
        nodeSelectorTerms:
          - matchExpressions:
              - key: kubernetes.io/e2e-az-name
                operator: In
                values:
                  - e2e-az1
                  - e2e-az2
      preferredDuringSchedulingIgnoredDuringExecution:
        - weight: 1
          preference:
            matchExpressions:
              - key: another-node-label-key
                operator: In # In, NotIn, Exists, DoesNotExist, Gt, Lt
                values:
                  - another-node-label-value
  containers:
    - name: with-node-affinity
      image: k8s.gcr.io/pause:2.0
```

If you specify both `nodeSelector` and `nodeAffinity`, both must be satisfied for the pod to be scheduled onto a candidate node.

If you specify multiple `nodeSelectorTerms` associated with nodeAffinity types, then the pod can be scheduled onto a node if one of the nodeSelectorTerms can be satisfied.

If you specify multiple `matchExpressions` associated with nodeSelectorTerms, then the pod can be scheduled onto a node only if all matchExpressions is satisfied.

The weight field in preferredDuringSchedulingIgnoredDuringExecution is in the range 1-100.

**KubeSchedulerConfiguration**:

```yml
apiVersion: kubescheduler.config.k8s.io/v1beta1
kind: KubeSchedulerConfiguration

profiles:
  - schedulerName: default-scheduler
  - schedulerName: foo-scheduler
    pluginConfig:
      - name: NodeAffinity
        args:
          addedAffinity:
            requiredDuringSchedulingIgnoredDuringExecution:
              nodeSelectorTerms:
                - matchExpressions:
                    - key: scheduler-profile
                      operator: In
                      values:
                        - foo
```

The addedAffinity is applied to all Pods that set .spec.schedulerName to foo-scheduler, in addition to the NodeAffinity specified in the PodSpec. That is, in order to match the Pod, Nodes need to satisfy addedAffinity and the Pod's .spec.NodeAffinity.

**PodAffinity**:

```yml
apiVersion: v1
kind: Pod
metadata:
  name: with-pod-affinity
spec:
  affinity:
    podAffinity:
      requiredDuringSchedulingIgnoredDuringExecution:
        - labelSelector:
            matchExpressions:
              - key: security
                operator: In
                values:
                  - S1
          topologyKey: topology.kubernetes.io/zone
    podAntiAffinity:
      preferredDuringSchedulingIgnoredDuringExecution:
        - weight: 100
          podAffinityTerm:
            labelSelector:
              matchExpressions:
                - key: security
                  operator: In
                  values:
                    - S2
            topologyKey: topology.kubernetes.io/zone
  containers:
    - name: with-pod-affinity
      image: k8s.gcr.io/pause:2.0
```

The pod affinity rule says that the pod can be scheduled onto a node only if that node is in the same zone as at least one already-running pod that has a label with key "security" and value "S1". (More precisely, the pod is eligible to run on node N if node N has a label with key topology.kubernetes.io/zone and some value V such that there is at least one node in the cluster with key topology.kubernetes.io/zone and value V that is running a pod that has a label with key "security" and value "S1".) The pod anti-affinity rule says that the pod should not be scheduled onto a node if that node is in the same zone as a pod with label having key "security" and value "S2".

**TopologyKey**:

1. For pod affinity, empty topologyKey is not allowed in both requiredDuringSchedulingIgnoredDuringExecution and preferredDuringSchedulingIgnoredDuringExecution.
2. For pod anti-affinity, empty topologyKey is also not allowed in both requiredDuringSchedulingIgnoredDuringExecution and preferredDuringSchedulingIgnoredDuringExecution.
3. For requiredDuringSchedulingIgnoredDuringExecution pod anti-affinity, the admission controller LimitPodHardAntiAffinityTopology was introduced to limit topologyKey to kubernetes.io/hostname. If you want to make it available for custom topologies, you may modify the admission controller, or disable it.
4. Except for the above cases, the topologyKey can be any legal label-key.

### NamespaceSelector [alpha feature]

### Practical User-Cases

**Always co-located in the same node**:

Here is the yaml snippet of a simple redis deployment with three replicas and selector label app=store. The deployment has PodAntiAffinity configured to ensure the scheduler does not co-locate replicas on a single node.

```yml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: redis-cache
spec:
  selector:
    matchLabels:
      app: store
  replicas: 3
  template:
    metadata:
      labels:
        app: store
    spec:
      affinity:
        podAntiAffinity:
          requiredDuringSchedulingIgnoredDuringExecution:
            - labelSelector:
                matchExpressions:
                  - key: app
                    operator: In
                    values:
                      - store
              topologyKey: "kubernetes.io/hostname"
      containers:
        - name: redis-server
          image: redis:3.2-alpine
```

This informs the scheduler that all its replicas are to be co-located with pods that have selector label app=store. This will also ensure that each web-server replica does not co-locate on a single node.

```yml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: web-server
spec:
  selector:
    matchLabels:
      app: web-store
  replicas: 3
  template:
    metadata:
      labels:
        app: web-store
    spec:
      affinity:
        podAntiAffinity:
          requiredDuringSchedulingIgnoredDuringExecution:
            - labelSelector:
                matchExpressions:
                  - key: app
                    operator: In
                    values:
                      - web-store
              topologyKey: "kubernetes.io/hostname"
        podAffinity:
          requiredDuringSchedulingIgnoredDuringExecution:
            - labelSelector:
                matchExpressions:
                  - key: app
                    operator: In
                    values:
                      - store
              topologyKey: "kubernetes.io/hostname"
      containers:
        - name: web-app
          image: nginx:1.16-alpine
```

### nodeName

```yml
apiVersion: v1
kind: Pod
metadata:
  name: nginx
spec:
  containers:
    - name: nginx
      image: nginx
  nodeName: minikube
```

- If the named node does not exist, the pod will not be run, and in some cases may be automatically deleted.
- If the named node does not have the resources to accommodate the pod, the pod will fail and its reason will indicate why, for example OutOfmemory or OutOfcpu.
- Node names in cloud environments are not always predictable or stable.

## [Pod Overhead](https://kubernetes.io/docs/concepts/scheduling-eviction/pod-overhead/)

Pod Overhead is a feature for accounting for the resources consumed by the Pod infrastructure on top of the container requests & limits.

```yml
---
kind: RuntimeClass
apiVersion: node.k8s.io/v1
metadata:
  name: kata-fc
handler: kata-fc
overhead:
  podFixed:
    memory: "120Mi"
    cpu: "250m"
```

```yml
apiVersion: v1
kind: Pod
metadata:
  name: test-pod
spec:
  runtimeClassName: kata-fc
  containers:
    - name: busybox-ctr
      image: busybox
      stdin: true
      tty: true
      resources:
        limits:
          cpu: 500m
          memory: 100Mi
    - name: nginx-ctr
      image: nginx
      resources:
        limits:
          cpu: 1500m
          memory: 100Mi
```

At admission time the RuntimeClass admission controller updates the workload's PodSpec to include the overhead as described in the RuntimeClass. If the PodSpec already has this field defined, the Pod will be rejected. In the given example, since only the RuntimeClass name is specified, the admission controller mutates the Pod to include an overhead.

```sh
kubectl get pod test-pod -o jsonpath='{.spec.overhead}'
# output
map[cpu:250m memory:120Mi]
```

When the kube-scheduler is deciding which node should run a new Pod, the scheduler considers that Pod's overhead as well as the sum of container requests for that Pod. For this example, the scheduler adds the requests and the overhead, then looks for a node that has 2.25 CPU and 320 MiB of memory available.

```sh
kubectl get pod test-pod -o jsonpath='{.spec.containers[*].resources.limits}'
# output
map[cpu: 500m memory:100Mi] map[cpu:1500m memory:100Mi]
# check what is observed by the node
kubectl describe node | grep test-pod -B2
# output
Namespace                   Name                CPU Requests  CPU Limits   Memory Requests  Memory Limits  AGE
---------                   ----                ------------  ----------   ---------------  -------------  ---
default                     test-pod            2250m (56%)   2250m (56%)  320Mi (1%)       320Mi (1%)     36m
```

## [Taints and Tolerations](https://kubernetes.io/docs/concepts/scheduling-eviction/taint-and-toleration/)

## Scheduling Framework

## Scheduler Performance Tuning

## Resource Bin Packing for Extended Resources

## Pod Priority and Preemption

## Node-pressure Eviction

## API-initiated Eviction
