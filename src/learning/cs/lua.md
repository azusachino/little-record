# Lua学习

Lua大小写敏感，分号可选

## 语法

### 注释

行注释：`--`  
块注释：

```lua
--[[
    块注释
    块注释
]]--
```

### 变量

- Lua的数值只有double(64bits)
- 字符串可以用单引号，双引号，支持C类型的转义
- Lua的NULL值是nil
- 布尔类型只有nil和false是falsy，数字0和空字符串都是truthy
- Lua中的变量都是全局变量，变量前加local关键字的是局部变量

## 控制语句

Lua中没有++或+=操作  
Lua中不等于是`~=`

### while

```lua
sum = 0
num = 1
while num <= 100 do
    sum = sum + num
    num = num + 1
end
print("sum =",sum)
```

### if-else

```lua
if age == 40 and sex =="Male" then
    print("男人四十一枝花")
elseif age > 60 and sex ~="Female" then -- 注意不等于的表达方式
    print("old man without country!")
elseif age < 20 then
    io.write("too young, too naive!\n")
else
    local age = io.read()
    print("Your age is "..age)
end
```

### for

```lua
sum = 0
for i = 1, 100, 2 do -- 可以定义step
    sum = sum + i
end
```

### until

```lua
sum = 2
repeat
   sum = sum ^ 2 --幂操作
   print(sum)
until sum >1000
```

## 函数

函数关键字：`function`

### 递归

```lua
function fib(n)
  if n < 2 then return 1 end
  return fib(n - 2) + fib(n - 1)
end
```

### 闭包

```lua
function newCounter()
    local i = 0
    return function()     -- anonymous function
       i = i + 1
        return i
    end
end
c1 = newCounter()
print(c1())  --> 1
print(c1())  --> 2
```

### 返回值

Lua支持在一条语句上赋多个值：`name, age, bGay = "haoel", 37, false, "haoel@hotmail.com"`

```lua
function getUserInfo(id)
    print(id)
    return "haoel", 37, "haoel@hotmail.com", "https://coolshell.cn"
end
name, age, email, website, bGay = getUserInfo() -- bGay is nil
```

### 局部函数

```lua
function foo(x) return x^2 end
foo = function(x) return x^2 end
```

## Table

Lua中的KeyValue数据结构

```lua
obj = {name="Az", age=26, handsome=true}
obj.website='adawdad.com'
local age = obj.age
obj.handsome=false
obj.name=nil
```

Lua的下标不是从0开始的，是从1开始的。  

Lua中的变量，如果没有local关键字，全都是全局变量，Lua也是用Table来管理全局变量的，Lua把这些全局变量放在了一个叫“_G”的Table里。

## MetaTable&MetaMethod

MetaTable和MetaMethod是Lua中的重要的语法，MetaTable主要是用来做一些类似于C++重载操作符式的功能。

```lua
fraction_a = {numerator=2,denominator=3}
fraction_b = {numerator=4,denominator=7}
-- 直接执行 fraction_a+fraction_b 会报错，这时可以使用MetaTable
fraction_op={}
function fraction_op.__add(f1, f2)
    ret = {}
    ret.numerator = f1.numerator * f2.denominator + f2.numerator * f1.denominator
    ret.denominator = f1.denominator * f2.denominator
    return ret
end

-- setmetatable是库函数
setmetatable(fraction_a, fraction_op)
setmetatable(fraction_b, fraction_op)

fraction_s = fraction_a + fraction_b -- 实际调用fraction_op.__add()
```

### Lua内建的MetaMethod

```lua
__add(a, b)                     对应表达式 a + b
__sub(a, b)                     对应表达式 a - b
__mul(a, b)                     对应表达式 a * b
__div(a, b)                     对应表达式 a / b
__mod(a, b)                     对应表达式 a % b
__pow(a, b)                     对应表达式 a ^ b
__unm(a)                        对应表达式 -a
__concat(a, b)                  对应表达式 a .. b
__len(a)                        对应表达式 #a
__eq(a, b)                      对应表达式 a == b
__lt(a, b)                      对应表达式 a < b
__le(a, b)                      对应表达式 a <= b
__index(a, b)                   对应表达式 a.b
__newindex(a, b, c)             对应表达式 a.b = c
__call(a, ...)                  对应表达式 a(...)
```

## 面向对象

`__index`这个重载，这个东西主要是重载了find key的操作。这操作可以让Lua变得有点面向对象的感觉，让其有点像Javascript的prototype。

```lua
Window_Prototype = {x=0, y=0, width=100, height=100}
MyWin = {title="Hello"}
setmetatable(MyWin, {__index = Window_Prototype}) -- 此时，MyWin就可以访问x,y...了
```

```lua
Person={}
function Person:new(p)
    local obj = p
    if (obj == nil) then
        obj = {name="ChenHao", age=37, handsome=true}
    end
    self.__index = self
    return setmetatable(obj, self)
end
function Person:toString()
    return self.name .." : ".. self.age .." : ".. (self.handsome and "handsome" or "ugly")
end

-- 使用样例
me = Person:new()
print(me:toString())
kf = Person:new{name="King's fucking", age=70, handsome=false}
print(kf:toString())
```

## 模块

可以直接使用require(“model_name”)来载入别的lua文件，文件的后缀是.lua。

1. `require`函数，载入同样的lua文件时，只有第一次的时候会去执行，后面的相同的都不执行了。
2. 如果你要让每一次文件都会执行的话，你可以使用`dofile("hello")`函数
3. 如果你需要载入后不执行，等你需要的时候执行时，你可以使用 `loadfile()`函数

```lua
-- myMod.lua
local Model = {}
local function getName()
    return "AZ"
end

function Model.Greeting()
    print("Hello, My name is" ..getName())
end

return Model
```

```lua
local model = require("myMod")
model.Greeting()
```
