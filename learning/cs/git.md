# Learn GIT

## Git提交规范

### AngularJS的提交规范

![angular](https://s3.ax1x.com/2021/01/22/s5BPSO.png)

```properties
<type>[optional scope]: <description>
# 空行
[optional body]
# 空行
[optional footer]
```

### type

本次提交的类型

- feat 新功能，顾名思义就是新需求的实现。
- fix 修复，就是对bug的修复。
- docs 文档，主要用来描述文档的变更。
- style 主要是代码风格相关的提交，比如格式化等。
- refactor 重构代码，对已有功能的重构，但是区别于bugfix。
- test 测试相关的提交，不太常用。
- chore 构建过程或辅助工具的变动，不太常用，比如之前用Maven，后面换成了Gradle。

### scope(optional)

本次提交影响的范围(方便快速定位)

### subject

本次提交的简要概述

### body(optional)

详细描述本次提交涉及的条目，罗列代码功能

### foot(optional)

描述本次提交中的break change和issue

#### break change

指明本次提交是否产生了破坏性修改，类似版本升级、接口参数减少、接口删除、迁移等。如果产生了上述的影响强烈建议在提交信息中写明break change，有利于出问题时快速定位，回滚，复盘。

#### issue

如果发现项目有bug、或者有优化的建议、甚至新增一个任务，就可以利用issue给项目提交一个任务。
