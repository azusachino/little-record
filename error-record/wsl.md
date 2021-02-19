# WSL相关问题

## 1. wsl内部的$PATH和windows的环境变量产生冲突

在wsl中查看path环境变量，默认包含win10的系统变量，编译lede固件时部分命令执行会出现问题， 经过搜索使用以下命令无效

`echo "[interop]\nenabled=false\nappendWindowsPath=false" | sudo tee /etc/wsl.conf`

修改注册表可解决：

HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Lxss{GUID}\Flags Flags从更改7为5。

管理员运行powershell重启wsl

- net stop LxssManager
- net start LxssManager

## 2. wsl的默认用户不是root

terminal执行 `ubuntu20.exe config --default-user root`

## 3. wsl内部文件权限问题

修改Linux环境中用户目录下的wsl.conf文件

```properties
[automount]
enabled = true
root = /mnt/
options = "metadata,dmask=022,fmask=133"
mountFsTab = false
```

## 4. vmmem占用内存过大问题

Windows环境下, 在当前用户的个人目录下建立.wslconfig文件

```properties
[wsl2]
memory=2GB
swap=0
localhostForwarding=true
```
