# Gitbook 相关问题

## 1. gitbook install failed

国内博客上总结的办法:

1. 降低 nodejs 的版本 10.22.0（代价太高，没有尝试）
2. 将错误的那一行注释掉（不起作用）

解决方法:

```sh
# 在gitbook-cli的文件夹中，补充安装一个依赖
cd /usr/local/lib/node_modules/gitbook-cli/node_modules/npm/node_modules/
npm install graceful-fs@4.2.0 --save
```

## 2. gitbook serve error

gitbook 默认依赖了 cpr@1 导致某一行代码执行失败

解决方案:

安装较新版本的 cpr

```bash
cd ~/.gitbook/versions/3.2.3
npm install cpr@3
```
