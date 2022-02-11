# Readme

## Objects

- Api groups => apis/batch/v1, apis/extensions/v1beta1
- metadata
- tag, selector
- namespace (FQDN, full qualified domain name)
- interface
- spec (target status)
- pod
- service
- status

```go
// TypeMeta
type TypeMeta struct {
    Kind string
    APIVersion string
}

// ObjectMeta
type ObjectMeta struct {
    Name string
    GenerateName string
    Namespace string
    SelfLink string // should remove in v1.21
    UID types.UID
    ResourceVersion string
    Generation int64
    CreationTimestamp Time
    DeletionTimestamp *Time
    DeletionGracePeriodSeconds *int64
    Labels map[string]string
    Annotations map[string]string
    OwnerReferences []OwnReference
    Finalizers []string
    ClusterName string
    ManagedFields []ManagedFieldsEntry
}

type Object interface {
    GetNamespace() string
    SetNamespace(ns string)
    GetName() string
    SetName(n string)
    GetGenerateName() string
    SetGenerateName(gn string)
    GetUID() types.UID
    SetUID(uid types.UID)
}
```
