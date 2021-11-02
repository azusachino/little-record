# PV & PVC & StorageClass

## clean order

1. 确认 deployment 是否已挂载该 pvc,如果挂载了的话需要删除该 pvc，然后重新部署该 deployment。
2. 重新部署的 deploymeny 为 running 状态后，此时可以删除该 pvc
3. pvc 删除后，可以删除 pv
