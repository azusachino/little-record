# Learn Mini APP

## 小程序背景

- 更好的体验
- 规范与管理

## WXML相关语法

- 标签名 + 属性名

### 数据绑定

```jsx
<view>
<text>{{message}}</text>
</view>

<view>
<block vx-for="">
</block>
<view>
```

- id
- class
- style
- hidden
- data-*
- bind*/catch*

```jsx
<view style="width:500rpx;height:30px;background-color:{{colorValue}}">
Hello world
</view>

Page({
    data: {
        colorValue: 'red'
    }
})
```

## WXS <==> JS

```jsx
<wxs module="m1">
  module.exports = {
      message: 'hello, world'
  }
</wxs>

<view> {{message}} </view>

<wxs src="./m2.wxs" module="m2"></wxs>
<view> {{m2.message}}</view>
```

## MINA框架

- 冷启动 -> 初次打开
- 热启动 -> 小程序仍处于活跃状态

## 小程序生命周期

1. onLaunch
2. onShow
3. onHide
4. onError

## 路由

1. 初始化
2. 打开新页面
3. 页面重定向
4. 页面返回
5. Tab切换
6. 重加载

## 事件

## 组件

- 媒体组件
- 地图
- 开放能力
- 画布
- 视图容器
- 基础内容
- 表单组件
- 导航

---

- view
- scroll-view
- swipper
- movable-area & movable-view
- cover-view

### 基础内容组件

- icon
- text
- rich-text
- progress => percent, color, sroke-width, active, activeColor, backgroundColor, show-info, active-modes

### 表单组件

- button
- checkbox
- form
- input
- label
- picker
- picker-view
- radio
- slider
- switch
- textarea

## API相关功能

参考设计文档
