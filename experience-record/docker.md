# Docker

## 删除none的image

```sh
docker rmi $(docker images | grep "none" | awk '{print $3}')
```
