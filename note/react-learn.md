# Learn React

## 简单介绍

- FaceBook推出
- 2013年推出
- 函数式编程
- 健全的文档和社区
- React 16 (Fiber)

## 脚手架

- `npm install -g create-react-app`
- `npx create-react-app my-app`

## 工程目录简介

- node_modules
- package.json
  - name
  - version
  - private
  - scripts
  - dependencies
- public
  - index.html SPA的page
  - favicon.ico 网页图标
  - manifest.json
- src
  - index.js 主程序入口文件
  - App.js 主界面
  - registerServiceWorker (PWA)
- README.md 介绍当前项目的文本
- yarn.lock 自动生成的保存版本信息文件

## React中的组件

```tsx
class App extends React.Component {
    render() {
        return (
            <div>Hello,World</div>
        )
    }
}
```

## React衍生的思考

- 声明式开发
- 可以与其他框架并存
- 组件化
- 单向数据流
- 视图层框架
- 函数式编程
