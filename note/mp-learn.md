# Mybatis-plus

## 逻辑删除

### 逻辑删除实现

```java
@TableLogic
private boolean isDeleted;
```

```yml
mybatis-plus:
  global-config:
```
