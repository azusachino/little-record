# Windows 相关经验

## update error 0x8024401c

```bat
net stop wuauserv
reg delete HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate
net start wuauserv
```

## 程序跑出桌面问题

在任务栏找到该程序，按住 shift 和鼠标右键

- 移动，此时不要移动或点击鼠标，用方向键移动该程序的窗口
- 最大化

## wsl 内部的 $PATH 和 windows 的环境变量产生冲突

在 wsl 中查看 path 环境变量，默认包含 win10 的系统变量，编译 lede 固件时部分命令执行会出现问题， 经过搜索使用以下命令无效

`echo "[interop]\nenabled=false\nappendWindowsPath=false" | sudo tee /etc/wsl.conf`

修改注册表可解决：

`HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Lxss{GUID}\Flags` Flags 从更改 7 为 5。

管理员运行 powershell 重启 wsl

- net stop LxssManager
- net start LxssManager

## wsl 的默认用户不是 root

terminal 执行 `ubuntu20.exe config --default-user root`

## wsl 内部文件权限问题

修改 Linux 环境中用户目录下的 `wsl.conf` 文件

```properties
[automount]
enabled = true
root = /mnt/
options = "metadata,dmask=022,fmask=133"
mountFsTab = false
```

## vmmem 占用内存过大问题

Windows 环境下，在当前用户的个人目录下建立 `.wslconfig` 文件

```properties
[wsl2]
memory=2GB
swap=0
localhostForwarding=true
```
