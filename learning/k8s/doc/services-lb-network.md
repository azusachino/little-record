# Services, Load Balancing, and Networking

Kubernetes networking addresses four concerns:

- Containers within a Pod use networking to communicate via loopback.
- Cluster networking provides communication between different Pods.
- The Service resource lets you expose an application running in Pods to be reachable from outside your cluster.
- You can also use Services to publish services only for consumption inside your cluster.

## Service

In Kubernetes, a Service is an abstraction which defines a logical set of Pods and a policy by which to access them (sometimes this pattern is called a micro-service). The set of Pods targeted by a Service is usually determined by a selector.

## Topology-aware traffic routing with topology keys

## DNS for Services and Pods

## Connecting Applications with Services

## Ingress

## Ingress Controllers

## EndpointSlices

## Service Internal Traffic Policy

## Topology Aware Hints

## Network Policies

## IPv4/IPv6 dual-stack
