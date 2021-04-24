# Kubernetes

## PlaceHolder

[Hello Minikube](https://kubernetes.io/docs/tutorials/hello-minikube/)

Before action, by using `minikube dashboard` to check status in web browser.

### Footsteps to deploy a service

A Kubernetes Pod is a group of one or more Containers, tied together for the purposes of administration and networking. The Pod in this tutorial has only one Container. A Kubernetes Deployment checks on the health of your Pod and restarts the Pod's Container if it terminates. Deployments are the recommended way to manage the creation and scaling of Pods.

1. Use the `kubectl` create command to create a Deployment that manages a Pod. The Pod runs a Container based on the provided Docker image. `kubectl create deployment hello-node --image=[k8s.gcr.io/echoserver:1.4](http://k8s.gcr.io/echoserver:1.4)`
2. View the deployment: `kubectl get deployments`
3. View the pod: `kubectl get pods`
4. View cluster events: `kubectl get events`
5. View the `kubectl` configuration: `kubectl config view`

By default, the Pod is only accessible by its internal IP address within the Kubernetes cluster. To make the hello-node Container accessible from outside the Kubernetes virtual network, you have to expose the Pod as a Kubernetes Service.

1. Expose the Pod to the public internet using the `kubectl` expose command: `kubectl expose deployment hello-node --type=LoadBalancer --port=8080` (The `--type=LoadBalancer` flag indicates that you want to expose your Service outside of the cluster.)
2. View the Service created: `kubectl get services`
3. Start the service: `minikube service hello-node`

Clean up

```bash
kubectl delete service hello-node
kubectl delete deployment hello-node
```

---

Kubernetes coordinates a highly available cluster of computers that are connected to work as a single unit.

### Cluster Diagram

![https://d33wubrfki0l68.cloudfront.net/283cc20bb49089cb2ca54d51b4ac27720c1a7902/34424/docs/tutorials/kubernetes-basics/public/images/module_01_cluster.svg](https://d33wubrfki0l68.cloudfront.net/283cc20bb49089cb2ca54d51b4ac27720c1a7902/34424/docs/tutorials/kubernetes-basics/public/images/module_01_cluster.svg)

The control plane is responsible for managing the cluster.

A node is a VM or a physical computer that serves as a worker machine in a Kubernetes cluster. Each node has a Kubelet, which is an agent for managing the node and communicating with the Kubernetes control plane.

`Minikube` as a simple implementation of Kubernetes Cluster.

`kubectl`: kubectl create deployment

- searched for a suitable node where an instance of the application could be run
- scheduled the application to run on that Node
- configured the cluster to reschedule the instance on a new Node when needed

`Kubernetes Pods`: A Pod is a group of one or more application containers (such as Docker) and includes shared storage (volumes), IP address and information about how to run them.

- Shared storage, as Volumes
- Networking, as a unique cluster IP address
- Information about how to run each container, such as the container image version or specific ports to use

Containers should only be scheduled together in a single Pod if they are tightly coupled and need to share resources such as disk.

`Kubernetes Nodes`: A Node is a worker machine in Kubernetes and may be either a virtual or a physical machine, depending on the cluster. Each Node is managed by the Master. A Node can have multiple pods, and the Kubernetes master automatically handles scheduling the pods across the Nodes in the cluster. The Master's automatic scheduling takes into account the available resources on each Node.

Every Kubernetes Node runs at least:

- Kubelet, a process responsible for communication between the Kubernetes Master and the Node; it manages the Pods and the containers running on a machine.
- A container runtime (like Docker) responsible for pulling the container image from a registry, unpacking the container, and running the application.

![https://d33wubrfki0l68.cloudfront.net/5cb72d407cbe2755e581b6de757e0d81760d5b86/a9df9/docs/tutorials/kubernetes-basics/public/images/module_03_nodes.svg](https://d33wubrfki0l68.cloudfront.net/5cb72d407cbe2755e581b6de757e0d81760d5b86/a9df9/docs/tutorials/kubernetes-basics/public/images/module_03_nodes.svg)

Service type:

- ClusterIP (default) - Exposes the Service on an internal IP in the cluster. This type makes the Service only reachable from within the cluster.
- NodePort - Exposes the Service on the same port of each selected Node in the cluster using NAT. Makes a Service accessible from outside the cluster using `<NodeIP>:<NodePort>`. Superset of ClusterIP.
- LoadBalancer - Creates an external load balancer in the current cloud (if supported) and assigns a fixed, external IP to the Service. Superset of NodePort.
- ExternalName - Maps the Service to the contents of the `externalName` field (e.g. `foo.bar.example.com`), by returning a `CNAME` record with its value. No proxying of any kind is set up. This type requires v1.7 or higher of `kube-dns`, or CoreDNS version 0.0.8 or higher.

Scaling

![https://d33wubrfki0l68.cloudfront.net/30f75140a581110443397192d70a4cdb37df7bfc/b5f56/docs/tutorials/kubernetes-basics/public/images/module_05_scaling2.svg](https://d33wubrfki0l68.cloudfront.net/30f75140a581110443397192d70a4cdb37df7bfc/b5f56/docs/tutorials/kubernetes-basics/public/images/module_05_scaling2.svg)

### Scaling up

To list your deployments use the `get deployments` command: `kubectl get deployments`

The output should be similar to:

```plain
NAME                  READY   UP-TO-DATE   AVAILABLE   AGE
kubernetes-bootcamp   1/1     1            1           11m
```

We should have 1 Pod. If not, run the command again. This shows:

- *NAME* lists the names of the Deployments in the cluster.
- *READY* shows the ratio of CURRENT/DESIRED replicas
- *UP-TO-DATE* displays the number of replicas that have been updated to achieve the desired state.
- *AVAILABLE* displays how many replicas of the application are available to your users.
- *AGE* displays the amount of time that the application has been running.

To see the ReplicaSet created by the Deployment, run `kubectl get rs`

Notice that the name of the ReplicaSet is always formatted as `[DEPLOYMENT-NAME]-[RANDOM-STRING]`. The random string is randomly generated and uses the pod-template-hash as a seed.

Two important columns of this command are:

- *DESIRED* displays the desired number of replicas of the application, which you define when you create the Deployment. This is the desired state.
- *CURRENT* displays how many replicas are currently running.

Next, let’s scale the Deployment to 4 replicas. We’ll use the `kubectl scale` command, followed by the deployment type, name and desired number of instances:

`kubectl scale deployments/kubernetes-bootcamp --replicas=4`

To list your Deployments once again, use `get deployments`:

`kubectl get deployments`

The change was applied, and we have 4 instances of the application available. Next, let’s check if the number of Pods changed:

`kubectl get pods -o wide`

There are 4 Pods now, with different IP addresses. The change was registered in the Deployment events log. To check that, use the describe command:

`kubectl describe deployments/kubernetes-bootcamp`

### Update app

To list your deployments use the get deployments command: `kubectl get deployments`

To list the running Pods use the `get pods` command:

`kubectl get pods`

To view the current image version of the app, run a `describe` command against the Pods (look at the Image field):

`kubectl describe pods`

To update the image of the application to version 2, use the `set image` command, followed by the deployment name and the new image version:

`kubectl set image deployments/kubernetes-bootcamp kubernetes-bootcamp=jocatalin/kubernetes-bootcamp:v2`

The command notified the Deployment to use a different image for your app and initiated a rolling update. Check the status of the new Pods, and view the old one terminating with the `get pods` command:

`kubectl get pods`

## Configuration

1.define a configmap, then `kubectl apply -f redis-configmap.yml`

```yml
apiVersion: v1
kind: ConfigMap
metadata:
  name: example-redis-config
data:
  redis-config: |
    maxmemory 2mb
    maxmemory-policy allkeys-lru
```

- A volume named config is created by spec.volumes[1]
- The key and path under spec.volumes[1].items[0] exposes the redis-config key from the example-redis-config ConfigMap as a file named redis.conf on the config volume.
- The config volume is then mounted at /redis-master by spec.containers[0].volumeMounts[1].

  2.define pod

```yml
apiVersion: v1
kind: Pod
metadata:
  name: redis
spec:
  containers:
    - name: redis
      image: redis:5.0.4
      command:
        - redis-server
        - "/redis-master/redis.conf"
      env:
        - name: MASTER
          value: "true"
      ports:
        - containerPort: 6379
      resources:
        limits:
          cpu: "0.1"
      volumeMounts:
        - mountPath: /redis-master-data
          name: data
        - mountPath: /redis-master
          name: config
  volumes:
    - name: data
      emptyDir: {}
    - name: config # connect with volumeMounts, use configmap as redis.conf
      configMap:
        name: example-redis-config
        items:
          - key: redis-config
            path: redis.conf
```

## Stateless Application

1.Run a HelloWorld

```yml
apiVersion: apps/v1
kind: Deployment
metadata:
labels:
  app.kubernetes.io/name: load-balancer-example
name: hello-world
spec:
replicas: 5
selector:
  matchLabels:
    app.kubernetes.io/name: load-balancer-example
template:
  metadata:
    labels:
      app.kubernetes.io/name: load-balancer-example
  spec:
    containers:
      - image: gcr.io/google-samples/node-hello:1.0
        name: hello-world
        ports:
          - containerPort: 8080
```

`kubectl apply -f load-balancer-example.yml`

2.Create Service exposes the deployment

`kubectl expose deployment hello-world --type=LoadBalancer --name=my-service`

## StatefulSet

1.Create a StatefulSet

```yml
apiVersion: v1
kind: Service
metadata:
  name: nginx
  labels:
    app: nginx
spec:
  ports:
  - port: 80
    name: web
  clusterIP: None
  selector:
    app: nginx
---
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: web
spec:
  serviceName: "nginx"
  replicas: 2
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: k8s.gcr.io/nginx-slim:0.8
        ports:
        - containerPort: 80
          name: web
        volumeMounts:
        - name: www
          mountPath: /usr/share/nginx/html
  volumeClaimTemplates:
  - metadata:
      name: www
    spec:
      accessModes: [ "ReadWriteOnce" ]
      resources:
        requests:
          storage: 1Gi
```
