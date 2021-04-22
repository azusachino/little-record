# Vue Related

## 在 vue template 模板{{xxx}}中的数据进行格式化处理

1.定义格式化的方法

```js
export default {
  filters: {
    formatState(val: any) {
      return moment(val).format("YYYY-MM-DD");
    },
  },
};
```

2.在模板中使用

```html
<template slot-scope="scope">
  <div>{{scope.row.createTime | formatState()}}</div>
</template>
```
