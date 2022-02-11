# Docker Distinguish

## docker &

- docker-cli: the command-line utility that you interact with using `docker ...` commands
- docker-daemon: a api server act as a bridge from docker-cli to containerd through grpc
- containerd: a daemon process that manages and runs containers. It pushes and pulls images, manages storage and networking, and supervises the running of containers. (OCI Image spec, Distribution spec)
- runc: the low-level container runtime (the thing that actually creates and runs containers). It includes `libcontainer`, a native go-based implementation for creating containers. (OCI Runtime spec)

## shim

In tech terms, a shim is a component in a software system, which acts as a bridge between different APIs, or as a compatibility layer. A shim is sometimes added when you want to use a third-party component, but you need a little bit of glue code to make it work.

## containerd

containerd is a high-level container runtime that came from Docker, and implements the CRI spec. It pulls images from registries, manages them and then hands over to a lower-level runtime, which actually creates and runs the container processes.

## CRI-O

CRI-O is another high-level container runtime which implements the Container Runtime Interface (CRI). It’s an alternative to containerd. It pulls container images from registries, manages them on disk, and launches a lower-level runtime to run container processes.
