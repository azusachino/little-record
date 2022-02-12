# Git Related

`ssh-keygen -t rsa -C your@email.com`

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

## windows 环境下换行符导致 shell 脚本无法执行

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
autocrlf = false ---> true
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

## Seven rules of a great git commit message

1. Separate subject from body with a blank line
2. Limit the subject line to 50 characters
3. Capitalize the subject line
4. Do not end the subject line with a period
5. Use the imperative mood in the subject line
6. Wrap the body at 72 characters
7. Use the body to explain what and why vs. how

```sh
Summarize changes in around 50 characters or less

More detailed explanatory text, if necessary. Wrap it to about 72
characters or so. In some contexts, the first line is treated as the
subject of the commit and the rest of the text as the body. The
blank line separating the summary from the body is critical (unless
you omit the body entirely); various tools like `log`, `shortlog`
and `rebase` can get confused if you run the two together.

Explain the problem that this commit is solving. Focus on why you
are making this change as opposed to how (the code explains that).
Are there side effects or other unintuitive consequences of this
change? Here's the place to explain them.

Further paragraphs come after blank lines.

 - Bullet points are okay, too

 - Typically a hyphen or asterisk is used for the bullet, preceded
   by a single space, with blank lines in between, but conventions
   vary here

If you use an issue tracker, put references to them at the bottom,
like this:

Resolves: #123
See also: #456, #789
```

### set up git message sample

```sh
git config --global core.editor emacs

# with specific editor
git config --global core.editor "'C:/Program Files/Notepad++/notepad++.exe' -multiInst -notabbar -nosession -noPlugin"

git config --global commit.template ~/.ssh/.gitmessage.txt
```
