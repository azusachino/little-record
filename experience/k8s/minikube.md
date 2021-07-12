# Minikube experience

## 1 Core CPU installing

download the latest release: `curl -Lo minikube https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64`

start with force cpu 1 & aliyun proxy: `minikube start --driver=none --extra-config=kubeadm.ignore-preflight-errors=NumCPU --force --cpus 1 --image-repository=registry.cn-hangzhou.aliyuncs.com/google_containers`
