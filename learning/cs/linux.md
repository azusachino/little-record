# Linux

## Linux 的体系结构

- 体系结构主要分为用户态( 用户上层活动)和内核态
- 内核: 本质是一段管理计算机硬件设备的程序
- 系统调用: 内核的访问接口, 是一种能再简化的操作
- 公共函数库: 系统调用的组合拳
- Shell: 命令解释器, 可编程

## Linux 的目录结构

- bin (binaries)存放二进制可执行文件
- sbin (super user binaries)存放二进制可执行文件，只有 root 才能访问
- etc (etcetera)存放系统配置文件
- usr (unix shared resources)用于存放共享的系统资源 home 存放用户文件的根目录
- root 超级用户目录
- dev (devices)用于存放设备文件
- lib (library)存放跟文件系统中的程序运行所需要的共享库及内核模块
- mnt (mount)系统管理员安装临时文件系统的安装点 boot 存放用于系统引导时使用的各种文件
- tmp (temporary)用于存放各种临时文件
- var (variable)用于存放运行时需要改变数据的文件

## AWK

输出第一列和第四列：`awk '{print $1, $4}' abc.txt`

- 单引号中被大括号包着的就是 awk 语句，只能被单引号包含
- 其中$1 .. $n 表示第几列。$0 表示整个行

格式化输出：`awk '{printf "%-8s %-8s %-8s %-18s %-22s %-15s\n",$1,$2,$3,$4,$5,$6}' netstat.txt`

### 过滤

```bash
awk '$3==0 && $6=="LISTEN"' abc.txt # ==,!=,>,<,<=,>=
awk '$3>0 {print $0}' abc.txt
awk '$3==0 && $6=="LISTEN" || NR==1 ' netstat.txt # 如果需要表头，需要引入内建变量NR
awk  'BEGIN{FS=":"} {print $1,$3,$6}' /etc/passwd # 指定分隔符
```

| 内建变量 | 具体含义                                                                        |
| -------- | ------------------------------------------------------------------------------- |
| $0       | 当前记录（这个变量中存放着整个行的内容）                                        |
| `$1~$n`  | 当前记录的第 n 个字段，字段间由 FS 分隔                                         |
| FS       | 输入字段分隔符 默认是空格或 Tab                                                 |
| NF       | 当前记录中的字段个数，就是有多少列                                              |
| NR       | 已经读出的记录数，就是行号，从 1 开始，如果有多个文件话，这个值也是不断累加中。 |
| FNR      | 当前记录数，与 NR 不同的是，这个值会是各个文件自己的行号                        |
| RS       | 输入的记录分隔符， 默认为换行符                                                 |
| OFS      | 输出字段分隔符， 默认也是空格                                                   |
| ORS      | 输出的记录分隔符，默认为换行符                                                  |
| FILENAME | 当前输入文件的名字                                                              |

### 字符串匹配

```bash
awk '$6 ~ /FIN/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt
awk '$6 ~ /WAIT/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt
awk '/LISTEN/' netstat.txt
awk '$6 ~ /FIN|TIME/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt
awk '$6 !~ /WAIT/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt # 模式取反
awk '!/WAIT/' abc.txt
```

拆分文件

```bash
awk 'NR!=1{print > $6}' netstat.txt # 按第六列分隔文件(NR!=1表示不处理表头)
awk 'NR!=1{print $4,$5 > $6}' netstat.txt # 可以把指定的列输出到文件
awk 'NR!=1{if($6 ~ /TIME|ESTABLISHED/) print > "1.txt";
else if($6 ~ /LISTEN/) print > "2.txt";
else print > "3.txt" }' netstat.txt
```

### 统计

```bash
ls -l  *.cpp *.c *.h | awk '{sum+=$5} END {print sum}' # 计算文件综合
awk 'NR!=1{a[$6]++;} END {for (i in a) print i ", " a[i];}' netstat.txt # 统计各个Connection状态
ps aux | awk 'NR!=1{a[$1]+=$6;} END { for(i in a) print i ", " a[i]"KB";}' # 统计每个用户的进程占了多少内存
```

### 脚本

- BEGIN{ 这里面放的是执行前的语句 }
- END {这里面放的是处理完所有的行后要执行的语句 }
- {这里面放的是处理每一行时要执行的语句}

```awk
#!/bin/awk -f
#运行前
BEGIN {
    math = 0
    english = 0
    computer = 0
    printf "NAME    NO.   MATH  ENGLISH  COMPUTER   TOTAL\n"
    printf "---------------------------------------------\n"
}
#运行中
{
    math+=$3
    english+=$4
    computer+=$5
    printf "%-6s %-6s %4d %8d %8d %8d\n", $1, $2, $3,$4,$5, $3+$4+$5
}
#运行后
END {
    printf "---------------------------------------------\n"
    printf "  TOTAL:%10d %8d %8d \n", math, english, computer
    printf "AVERAGE:%10.2f %8.2f %8.2f\n", math/NR, english/NR, computer/NR
}
```

### 环境变量

使用-v 参数和 ENVIRON，使用 ENVIRON 的环境变量需要 export

```bash
x=5
y=10
export y
echo $x $y
5 10
awk -v val=$x '{print $1, $2, $3, $4+val, $5+ENVIRON["y"]}' OFS="\t" score.txt
```

### 其他

```bash
#从file文件中找出长度大于80的行
awk 'length>80' file
#按连接数查看客户端IP
netstat -ntu | awk '{print $5}' | cut -d: -f1 | sort | uniq -c | sort -nr
#打印99乘法表
seq 9 | sed 'H;g' | awk -v RS='' '{for(i=1;i<=NF;i++)printf("%dx%d=%d%s", i, NR, i*NR, i==NR?"\n":"\t")}'
```

## SED(Stream Editor)

### 用 s 命令替换

```bash
sed "s/my/HaoChen's/g" pets.txt # 没有改变原文件，只是把处理后的内容输出了
sed "s/my/HaoChen's/g" pets.txt > hao_pets.txt # 重定向
sed -i "s/my/HaoChen's/g" # 直接修改原文件
sed 's/^/#/g' pets.txt # 在每一行最前面加 #
sed 's/$/ --- /g' pets.txt # 在每一行最后加 ---
sed "3s/my/your/g" pets.txt # 替换第三行
sed "3,6s/my/your/g" pets.txt # 替换第3-第6行

sed 's/s/S/1' my.txt # 只替换每一行的第一个s
sed 's/s/S/2' my.txt # 只替换每一行的第二个s
sed 's/s/S/3g' my.txt # 只替换每一行的第三个以后的s

sed '1,3s/my/your/g; 3,$s/This/That/g' my.txt # 多个匹配模式
sed -e '1,3s/my/your/g' -e '3,$s/This/That/g' my.txt # 等价于上一个命令

sed 's/my/[&]/g' my.txt # 使用&作为被匹配的变量

sed 's/This is my \([^,&]*\),.*is \(.*\)/\1:\2/g' my.txt # 圆括号匹配
```

- a 命令：append
- i 命令：insert
- c 命令：替换匹配行
- d 命令：删除匹配行
- p 命令：打印命令

```bash
sed "1 i This is my monkey, my monkey's name is wukong" my.txt # 在第一行前插入一行（insert）
sed "1 a This is my monkey, my monkey's name is wukong" my.txt # 在第一行后追加一行（append）

sed "/fish/a This is my monkey, my monkey's name is wukong" my.txt # 结合正则匹配
sed "/my/a ----" my.txt # 对每一行都进行append

sed "2 c This is my monkey, my monkey's name is wukong" my.txt # 替换第二行
sed "/fish/c This is my monkey, my monkey's name is wukong" my.txt # 结合正则匹配

sed '/fish/d' my.txt # 删除匹配行

sed '/fish/p' my.txt # 打印匹配行
```

## iptables

Iptables filters packets based on:

- **Tables:** Tables are files that join similar actions. A table consists of several **chains**.
- **Chains:** A chain is a string of **rules**. When a packet is received, iptables finds the appropriate table, then runs it through the chain of **rules** until it finds a match.
- **Rules:** A rule is a statement that tells the system what to do with a packet. Rules can block one type of packet, or forward another type of packet. The outcome, where a packet is sent, is called a **target**.
- **Targets:** A target is a decision of what to do with a packet. Typically, this is to accept it, drop it, or reject it (which sends an error back to the sender).

### **Tables and Chains**

Linux firewall iptables has four default tables. We will list all four along with the chains each table contains.

**1. Filter**

The **Filter** table is the most frequently used one. It acts as a bouncer, deciding who gets in and out of your network. It has the following default chains:

- **Input** – the rules in this chain control the packets received by the server.
- **Output** – this chain controls the packets for outbound traffic.
- **Forward** – this set of rules controls the packets that are routed through the server.

**2. Network Address Translation (NAT)**

This table contains NAT (Network Address Translation) rules for routing packets to networks that cannot be accessed directly. When the destination or source of the packet has to be altered, the NAT table is used. It includes the following chains:

- **Prerouting –** this chain assigns packets as soon as the server receives them.
- **Output –** works the same as the output chain we described in the **filter** table.
- **Postrouting –** the rules in this chain allow making changes to packets after they leave the output chain.

**3. Mangle**

The **Mangle** table adjusts the IP header properties of packets. The table has all the following chains we described above:

- **Prerouting**
- **Postrouting**
- **Output**
- **Input**
- **Forward**

**4. Raw**

The **Raw** table is used to exempt packets from connection tracking. The raw table has two of the chains we previously mentioned:

- **Prerouting**
- **Output**

![https://phoenixnap.com/kb/wp-content/uploads/2020/01/iptables-diagram.png](https://phoenixnap.com/kb/wp-content/uploads/2020/01/iptables-diagram.png)

**5. Security (Optional)**

Some versions of Linux also use a **Security** table to manage special access rules. This table includes **input, output,** and **forward** chains, much like the filter table.

### **Targets**

A target is what happens after a packet matches a rule criteria. **Non-terminating** targets keep matching the packets against rules in a chain even when the packet matches a rule.

With **terminating** targets, a packet is evaluated immediately and is not matched against another chain. The terminating targets in Linux iptables are:

- **Accept** – this rule accepts the packets to come through the iptables firewall.
- **Drop** – the dropped package is not matched against any further chain. When Linux iptables drop an incoming connection to your server, the person trying to connect does not receive an error. It appears as if they are trying to connect to a non-existing machine.
- **Return** – this rule sends the packet back to the originating chain so you can match it against other rules.
- **Reject** – the iptables firewall rejects a packet and sends an error to the connecting device.
