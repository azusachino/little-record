# Golang经验

## $GOPATH/go.mod exists but should not

开启模块支持后，并不能与$GOPATH共存,所以把项目从$GOPATH中移出即可

解决办法：

打开设置，将当前路径移出Project GOPATH
