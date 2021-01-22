# GIT相关问题

## windows环境下换行符导致.sh脚本无法执行

git windows安装之后，可以配置下面两个参数：

- `git config --global core.autocrlf true`
  - true 表示检出是转换CRLF, 提交时转换为 LF
  - input 表示检出是不转换，提交时转换为 LF
  - false 表示不做转换
- `git config --global core.safecrlf true`
  - true 表示不允许提交时包含不同换行符
  - warn 则只在有不同换行符时警告
  - false 则允许提价时有不同换行符存在

也可以通过修改～/.gitconfig

```properties
[core]
autocrlf = false ---> true
safecrlf = true
```
