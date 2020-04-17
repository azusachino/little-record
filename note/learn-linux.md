# Linux

## Linux的体系结构

- 体系结构主要分为用户态( 用户上层活动)和内核态
- 内核: 本质是一段管理计算机硬件设备的程序
- 系统调用: 内核的访问接口, 是一种能再简化的操作
- 公共函数库: 系统调用的组合拳
- Shell: 命令解释器, 可编程

## Linux的目录结构

- bin (binaries)存放二进制可执行文件
- sbin (super user binaries)存放二进制可执行文件，只有root才能访问
- etc (etcetera)存放系统配置文件
- usr (unix shared resources)用于存放共享的系统资源home 存放用户文件的根目录
- root 超级用户目录
- dev (devices)用于存放设备文件
- lib (library)存放跟文件系统中的程序运行所需要的共享库及内核模块
- mnt (mount)系统管理员安装临时文件系统的安装点boot 存放用于系统引导时使用的各种文件
- tmp (temporary)用于存放各种临时文件
- var (variable)用于存放运行时需要改变数据的文件
