# Windows相关经验

## update error 0x8024401c

```bat
net stop wuauserv
reg delete HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate
net start wuauserv
```
