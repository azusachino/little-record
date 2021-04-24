# Git Related

## ssh -T git@github.com timeout

add `config` to ~/.ssh folder

```properties
Host github.com
User git
Hostname ssh.github.com
PreferredAuthentications publickey
IdentityFile ~/.ssh/id_rsa
Port 443
```

## windows 环境下换行符导致.sh 脚本无法执行

git windows 安装之后，可以配置下面两个参数：

- `git config --global core.autocrlf true`
  - true 表示检出是转换 CRLF, 提交时转换为 LF
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

## 一台主机配置多个 git 账户

1.通过命令生成两份密钥

`ssh-keygen -t rsa -C your@email.com`

2.将生成的 ssh 信息配置到账户信息中

3.在 `.ssh` 文件夹中新建 `config` 文件

```plain
Host   域名地址
    HostName  域名地址
    IdentityFile C:\Users\yourname\.ssh\id_rsa
    PreferredAuthentications publickey
    User name1

# 配置github
Host github.com
    HostName github.com
    IdentityFile C:\Users\yourname\.ssh\id_rsa_new
    PreferredAuthentications publickey
    User name2
```

4.通过 ssh -T git@github.com 检测是否匹配成功
