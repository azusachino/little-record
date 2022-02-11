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

## 下载文件

```js
// 定义文件内容，类型必须为Blob 否则createObjectURL会报错
const content = new Blob(data, {
  type: "text/plain",
});

// 生成url对象
const urlObject = window.URL || window.webkitURL || window;
const url = urlObject.createObjectURL(content);
// 生成<a></a>DOM元素
const el = document.createElement("a");
// 链接赋值
el.href = url;
el.download = filename;
// 必须点击否则不会下载
el.click();
// 移除链接释放资源
urlObject.revokeObjectURL(url);
```
