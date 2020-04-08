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

## PropTypes 与 DefaultProps

```javascript
Class.propTypes = {
    name: string,
    age: number
}

Class.DefaultProps = {
    name: 'az'
}
```

## Props, State, render()

当props和state改变时, render函数会重新执行

---
state -> 当前组件的render()  
props -> 子组件的render()

## 虚拟dom

虚拟DOM就是JS对象, 可以描述真实DOM  
`['div', {id: 'abc'}, ['span', {}, 'hello world']]`

JSX -> React.createElement() -> Virtual Dom -> Real Dom

```javascript
return (<div><span> Hello World </span> </div>)
React.createElement('div', {}, React.createElement('span', {}, 'Hello World'))
```

### React 优点

1. 提升性能
2. 可以跨端实现

### 虚拟Dom Diff算法 (查找虚拟dom之间的差异)

层级比较 (dom的Key值)

## React生命周期 (钩子函数)

- Initialization
  - setup props and state
- Mounting
  - componentWillMount()
  - render()
  - componentDidMount()
- Update
  - componentWillReceiveProps
  - shouldComponentUpdate
  - componentWillUpdate
  - render()
  - componentDidUpdate
- Unmounting
  - componentWillUnmount

## 组件

- 容器组件
- UI组件
- 无状态组件 (纯函数, 性能较高)

```javascript
const TodoListUI = (props) => {
  return (
    <div>{props.value} </div>
  )
}
```

## Redux

### Redux 异步请求

```javascript
componentDidMount() {
  axios.get('/list.json')
  .then((res) => {
    const data = res.data
    const action = initListAction(data)
    store.dispatch(action)
  })
  .catch((e) => {
    console.err(e)
  })
}
```

### Redux-thunk

```js
import { createStore, applyMiddleware } from 'redux';
import thunk from 'redux-thunk';
import rootReducer from './reducers/index';

// Note: this API requires redux@>=3.1.0
const store = createStore(rootReducer, applyMiddleware(thunk));
```

updated version

```js
import { createStore, applyMiddleware, compose } from 'redux'
import thunkMiddleware from 'redux-thunk'
import rootReducer from '../reducers'

const composeEnhancers = typeof window === 'object' &&
(window as any).__REDUX_DEVTOOLS_EXTENSION_COMPOSE__ ?
  (window as any).__REDUX_DEVTOOLS_EXTENSION_COMPOSE__({}) : compose

const middleWares = [thunkMiddleware]

if ('development' === process.env.NODE_ENV) {
  middleWares.push(require('redux-logger').createLogger())
}

const enhancer = composeEnhancers(
  applyMiddleware(...middleWares)
)

export default function configStore() {
  return createStore(rootReducer, enhancer)
}

```

redux-thunk 是对`dispatch()`方法的封装
