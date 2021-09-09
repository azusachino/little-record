# [Security](https://kubernetes.io/docs/concepts/security/)

## Overview of Cloud Native Security

### 4C's Cloud Native security

Code => Container => Cluster => Cloud/Co-Lo/Corporate Datacenter

### Cloud

- Network access to API Server(Control Plane)
- Netwoek access to Nodes
- Kubernetes access to Cloud Provider API
- Access to ETCD
- ETCD encryption

### Cluster

- Securing the cluster components that are configurable
- Securing the applications which run in the cluster
  - RBAC Authorization
  - Authentication
  - Application Secrets Management (and encrypting them in etcd at rest)
  - Pod Security Policies
  - Quality of Service (and Cluster Resource Management)
  - Network policies
  - TLS for Kubernetnes ingress

### Container

- Container Vulnerability Scanning and OS Dependency Security
- Image Signing and Enforcement
- Disallow privileged users

### Code

- Access over TLS only
- Limiting port ranges of communication
- 3rd party Dependency Security
- Static Code Analysis
- Dynamic probing attacks

## Pod Security Standards

The Pod Security Standards define three different policies to broadly cover the security spectrum.

- Privileged: Unrestricted policy, providing the widest possible level of permissions. This policy allows for known privilege escalations.
- Baseline: Minimally restrictive policy which prevents known privilege escalations. Allows the default (minimally specified) Pod configuration.
- Restricted: Heavily restricted policy, following current Pod hardening best practices.
