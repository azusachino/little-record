# PowerShell ShortCut

## 入门级别

1. 像文件系统那样操作 Windows Registry——cd hkcu:
2. 在文件里递回地搜索某个字符串——dir –r | select string "searchforthis"
3. 使用内存找到五个进程——ps | sort –p ws | select –last 5
4. 循环（停止，然后重启）一个服务，如 DHCP——Restart-Service DHCP
5. 在文件夹里列出所有条目——Get-ChildItem – Force
6. 递归一系列的目录或文件夹——Get-ChildItem –Force c:\directory –Recurse
7. 在目录里移除所有文件而不需要单个移除——Remove-Item C:\tobedeleted –Recurse
8. 重启当前计算机——(Get-WmiObject -Class Win32_OperatingSystem -ComputerName .).Win32Shutdown(2)
9. 获取计算机组成或模型信息——Get-WmiObject -Class Win32_ComputerSystem
10. 获取当前计算机的 BIOS 信息——Get-WmiObject -Class Win32_BIOS -ComputerName .
11. 列出所安装的修复程序（如 QFE 或 Windows Update 文件）——Get-WmiObject -Class Win32_QuickFixEngineering -ComputerName .
12. 获取当前登录计算机的用户的用户名—— Get-WmiObject -Class Win32_ComputerSystem -Property UserName -ComputerName .
13. 获取当前计算机所安装的应用的名字——Get-WmiObject -Class Win32_Product -ComputerName . | Format-Wide -Column 1
14. 获取分配给当前计算机的 IP 地址——Get-WmiObject -Class Win32_NetworkAdapterConfiguration -Filter IPEnabled=TRUE -ComputerName . | Format-Table -Property IPAddress
15. 获取当前机器详细的 IP 配置报道——Get-WmiObject -Class Win32*NetworkAdapterConfiguration -Filter IPEnabled=TRUE -ComputerName . | Select-Object -Property [a-z]* -ExcludeProperty IPX\_,WINS\*
16. 找到当前计算机上使用 DHCP 启用的网络卡——Get-WmiObject -Class Win32_NetworkAdapterConfiguration -Filter "DHCPEnabled=true" -ComputerName .
17. 在当前计算机上的所有网络适配器上启用 DHCP——Get-WmiObject -Class Win32*NetworkAdapterConfiguration -Filter IPEnabled=true -ComputerName . | ForEach-Object -Process {$*.EnableDHCP()}
18. 在远程计算机上安装 MSI 包——(Get-WMIObject -ComputerName TARGETMACHINE -List | Where-Object -FilterScript {$\_.Name -eq "Win32_Product"}).Install(\\MACHINEWHEREMSIRESIDES\path\package.msi)
19. 使用基于 MSI 的应用升级包升级所安装的应用——(Get-WmiObject -Class Win32_Product -ComputerName . -Filter "Name='name_of_app_to_be_upgraded'").Upgrade(\\MACHINEWHEREMSIRESIDES\path\upgrade_package.msi)
20. 从当前计算机移除 MSI 包——(Get-WmiObject -Class Win32_Product -Filter "Name='product_to_remove'" -ComputerName . ).Uninstall()机器管理
21. 一分钟后远程关闭另一台机器——Start-Sleep 60; Restart-Computer –Force –ComputerName TARGETMACHINE
22. 添加打印机——(New-Object -ComObject WScript.Network).AddWindowsPrinterConnection(\\printerserver\hplaser3)
23. 移除打印机——(New-Object -ComObject WScript.Network).RemovePrinterConnection("\\printerserver\hplaser3 ")
24. 进入 PowerShell 会话——invoke-command -computername machine1, machine2 -filepath c:\Script\script.ps1
