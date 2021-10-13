# Reading StatefulSetController

## stateful_set_control

```go
// behaviours that ssc can perform
type StatefulSetControlInterface interface {
    UpdateStatefulSet(set *apps.StatefulSet, pods []*v1.Pod) (*apps.StatefulSetStatus, error)
    ListRevisions(set *apps.StatefulSet) ([]*apps.ControllerRevision, error)
    AdoptOrpahRevisions(set *apps.StatefulSet, revisions []*apps.ControllerRevision) error
}

// default ssc
type defaultStatefulSetControl struct {
    podControl StatefulPodControlInterface
    statusUpdater StatefulSetStatusUpdaterInterface
    controllerHistory history.Interface
    recorder record.EventRecorder
}
```
