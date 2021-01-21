# Go 编程模式

## [切片, 接口, 时间和性能](https://coolshell.cn/articles/21128.html)

### Slice

Slice 在 Go 语言中不是数组, 是一个结构体

```go
type slice struct{
    array unsafe.Pointer //指向存放数据的数组指针
    len   int            //长度有多大
    cap   int            //容量有多大
}
```

### 深度比较

使用反射`reflect.DeepEqual()`

```go
import (
    "fmt"
    "reflect"
)
func main() {
    v1 := data{}
    v2 := data{}
    fmt.Println("v1 == v2:",reflect.DeepEqual(v1,v2))
    //prints: v1 == v2: true
    m1 := map[string]string{"one": "a","two": "b"}
    m2 := map[string]string{"two": "b", "one": "a"}
    fmt.Println("m1 == m2:",reflect.DeepEqual(m1, m2))
    //prints: m1 == m2: true
    s1 := []int{1, 2, 3}
    s2 := []int{1, 2, 3}
    fmt.Println("s1 == s2:",reflect.DeepEqual(s1, s2))
    //prints: s1 == s2: true
}
```

### 接口编程

`io.Read#Read(p []byte) (n int, err error)`, `iouitl.ReadAll`

使用`Stringable`接口把"业务类型" `Country` 和 `City` 和"控制逻辑" `Print()` 给解耦了. 于是, 只要实现了`Stringable` 接口, 都可以传给 `PrintStr()` 来使用.

```go
type Country struct {
    Name string
}
type City struct {
    Name string
}
type Stringable interface {
    ToString() string
}
func (c Country) ToString() string {
    return "Country = " + c.Name
}
func (c City) ToString() string{
    return "City = " + c.Name
}
func PrintStr(p Stringable) {
    fmt.Println(p.ToString())
}
d1 := Country {"USA"}
d2 := City{"Los Angeles"}
PrintStr(d1)
PrintStr(d2)
```

### 接口完整性检查

Go 语言的编译器并没有严格检查一个对象是否实现了某接口所有的接口方法

```go
type Shape interface {
    Sides() int
    Area() int
}
type Square struct {
    len int
}
func (s* Square) Sides() int {
    return 4
}
func main() {
    s := Square{len: 5}
    fmt.Printf("%d\n",s.Sides())
}
```

声明一个 _ 变量（没人用），其会把一个 nil 的空指针，从 Square 转成 Shape，这样，如果没有实现完相关的接口方法，编译器就会报错：

> cannot use `(*Square)(nil) (type *Square)` as type Shape in assignment: *Square does not implement Shape (missing Area method)

```go
var _ Shape = (*Square)(nil)
```

### 时间

使用`time.Time`和`time.Duration`

- 在命令行上，flag 通过 time.ParseDuration 支持了 time.Duration
- Json 中的 encoding/json 中也可以把 time.Time 编码成 RFC 3339 的格式
- 数据库使用的 database/sql 也支持把 DATATIME 或 TIMESTAMP 类型转成 time.Time
- YAML 你可以使用 gopkg.in/yaml.v2 也支持 time.Time 、time.Duration 和 RFC 3339 格式

### 性能提示

- 如果需要把数字转字符串，使用 strconv.Itoa() 会比 fmt.Sprintf() 要快一倍左右
- 尽可能地避免把 String 转成[]Byte 。这个转换会导致性能下降。
- 如果在 for-loop 里对某个 slice 使用 append()请先把 slice 的容量很扩充到位，这样可以避免内存重新分享以及系统自动按 2 的 N 次方幂进行扩展但又用不到，从而浪费内存。
- 使用 StringBuffer 或是 StringBuild 来拼接字符串，会比使用 + 或 += 性能高三到四个数量级。
- 尽可能的使用并发的 go routine，然后使用 sync.WaitGroup 来同步分片操作
- 避免在热代码中进行内存分配，这样会导致 gc 很忙。尽可能的使用 sync.Pool 来重用对象。
- 使用 lock-free 的操作，避免使用 mutex，尽可能使用 sync/Atomic 包。 （关于无锁编程的相关话题，可参看《无锁队列实现》或《无锁 Hashmap 实现》）
- 使用 I/O 缓冲，I/O 是个非常非常慢的操作，使用 bufio.NewWrite() 和 bufio.NewReader() 可以带来更高的性能。
- 对于在 for-loop 里的固定的正则表达式，一定要使用 regexp.Compile() 编译正则表达式。性能会得升两个数量级。
- 如果需要更高性能的协议，考虑使用 protobuf 或 msgp 而不是 JSON，因为 JSON 的序列化和反序列化里使用了反射。
- 在使用 map 的时候，使用整型的 key 会比字符串的要快，因为整型比较比字符串比较要快。

## [错误处理](https://coolshell.cn/articles/21140.html)

### Java 的错误处理

`try-catch-finally`

- 函数接口在 input（参数）和 output（返回值）以及错误处理的语义是比较清楚的。
- 正常逻辑的代码可以与错误处理和资源清理的代码分开，提高了代码的可读性。
- 异常不能被忽略（如果要忽略也需要 catch 住，这是显式忽略）。
- 在面向对象的语言中（如 Java），异常是个对象，所以，可以实现多态式的 catch。
- 与状态返回码相比，异常捕捉有一个显著的好处是，函数可以嵌套调用，或是链式调用。

### Go 的错误处理

Go 语言的函数支持多返回值，所以，可以在返回接口把业务语义（业务返回值）和控制语义（出错返回值）区分开来。(result, err)

- 参数上基本上就是入参，而返回接口把结果和错误分离，这样使得函数的接口语义清晰；
- 而且，Go 语言中的错误参数如果要忽略，需要显式地忽略，用 \_ 这样的变量来忽略；
- 另外，因为返回的 error 是个接口（其中只有一个方法 Error()，返回一个 string ），所以你可以扩展自定义的错误处理。

```go
// 处理多种不同类型的Error
if err != nil {
  switch err.(type) {
    case *json.SyntaxError:
      ...
    case *ZeroDivisionError:
      ...
    case *NullPointerError:
      ...
    default:
      ...
  }
}
```

### 资源清理

- C 语言 – 使用的是 `goto fail;` 的方式到一个集中的地方进行清理
- C++语言- 一般来说使用 RAII 模式，通过面向对象的代理模式，把需要清理的资源交给一个代理类，然后在析构函数来解决。
- Java 语言 – 可以在 `finally` 语句块里进行清理。
- Go 语言 – 使用 `defer` 关键词进行清理。

```go
func Close(c io.Closer) {
  err := c.Close()
  if err != nil {
    log.Fatal(err)
  }
}
func main() {
  r, err := Open("a")
  if err != nil {
    log.Fatalf("error opening 'a'\n")
  }
  defer Close(r) // 使用defer关键字在函数退出时关闭文件。
  r, err = Open("b")
  if err != nil {
    log.Fatalf("error opening 'b'\n")
  }
  defer Close(r) // 使用defer关键字在函数退出时关闭文件。
}
```

### ErrorCheck 地狱

```go
// 太多的错误检查
func parse(r io.Reader) (*Point, error) {
    var p Point
    if err := binary.Read(r, binary.BigEndian, &p.Longitude); err != nil {
        return nil, err
    }
    if err := binary.Read(r, binary.BigEndian, &p.Latitude); err != nil {
        return nil, err
    }
    if err := binary.Read(r, binary.BigEndian, &p.Distance); err != nil {
        return nil, err
    }
    if err := binary.Read(r, binary.BigEndian, &p.ElevationGain); err != nil {
        return nil, err
    }
    if err := binary.Read(r, binary.BigEndian, &p.ElevationLoss); err != nil {
        return nil, err
    }
}

// 函数式编程
func parse(r io.Reader) (*Point, error) {
    var p Point
    var err error
    read := func(data interface{}) {
        if err != nil {
            return
        }
        err = binary.Read(r, binary.BigEndian, data)
    }
    read(&p.Longitude)
    read(&p.Latitude)
    read(&p.Distance)
    read(&p.ElevationGain)
    read(&p.ElevationLoss)
    if err != nil {
        return &p, err
    }
    return &p, nil
}
```

### 错误包装

```go
// 基础用法
if err != nil {
   return fmt.Errorf("something failed: %v", err)
}
// 封装
type authorizationError struct {
    operation string
    err error   // original error
}
func (e *authorizationError) Error() string {
    return fmt.Sprintf("authorization failed during %s: %v", e.operation, e.err)
}
```

## [Functional Options](https://coolshell.cn/articles/21146.html)

### 配置选项问题

在编程中，经常性的需要实例化一个对象

- 侦听的IP地址 Addr 和端口号 Port ，这两个配置选项是必填的。
- 协议 Protocol 、 Timeout 和MaxConns 字段，这几个字段是不能为空的，但是有默认值的，比如：协议是tcp, 超时30秒 和 最大链接数1024个。
- TLS是安全链接，需要配置相关的证书和私钥。这个是可以为空的。

```go
type Server struct {
    Addr     string
    Port     int
    Protocol string
    Timeout  time.Duration
    MaxConns int
    TLS      *tls.Config
}
// 由于Go语言不支持重载函数， 需要用不同的函数名称对应
func NewDefaultServer(addr string, port int) (*Server, error) {
  return &Server{addr, port, "tcp", 30 * time.Second, 100, nil}, nil
}
func NewTLSServer(addr string, port int, tls *tls.Config) (*Server, error) {
  return &Server{addr, port, "tcp", 30 * time.Second, 100, tls}, nil
}
func NewServerWithTimeout(addr string, port int, timeout time.Duration) (*Server, error) {
  return &Server{addr, port, "tcp", timeout, 100, nil}, nil
}
func NewTLSServerWithMaxConnAndTimeout(addr string, port int, maxconns int, timeout time.Duration, tls *tls.Config) (*Server, error) {
  return &Server{addr, port, "tcp", 30 * time.Second, maxconns, tls}, nil
}
```

### 配置对象方案

解决上述问题的最常见办法是使用一个配置对象

```go
type Config struct {
    Protocol string
    Timeout  time.Duration
    Maxconns int
    TLS      *tls.Config
}
type Server struct {
    Addr string
    Port int
    Conf *Config
}

func NewServer(addr string, port int, conf *Config) (*Server, error) {
    //...
}
//Using the default configuratrion
srv1, _ := NewServer("localhost", 9000, nil) 
conf := ServerConfig{Protocol:"tcp", Timeout: 60*time.Duration}
srv2, _ := NewServer("locahost", 9000, &conf)
```

### Builder模式

```go
//使用一个builder类来做包装
type ServerBuilder struct {
  Server
}
func (sb *ServerBuilder) Create(addr string, port int) *ServerBuilder {
  sb.Server.Addr = addr
  sb.Server.Port = port
  //其它代码设置其它成员的默认值
  return sb
}
func (sb *ServerBuilder) WithProtocol(protocol string) *ServerBuilder {
  sb.Server.Protocol = protocol 
  return sb
}
func (sb *ServerBuilder) WithMaxConn( maxconn int) *ServerBuilder {
  sb.Server.MaxConns = maxconn
  return sb
}
func (sb *ServerBuilder) WithTimeOut( timeout time.Duration) *ServerBuilder {
  sb.Server.Timeout = timeout
  return sb
}
func (sb *ServerBuilder) WithTLS( tls *tls.Config) *ServerBuilder {
  sb.Server.TLS = tls
  return sb
}
func (sb *ServerBuilder) Build() (Server) {
  return  sb.Server
}
```

### Functional Options

```go
// 首先定义一个函数类型
type Option func(*Server)

// 定义一组函数式Function
func Protocol(p string) Option {
    return func(s *Server) {
        s.Protocol = p
    }
}
func Timeout(timeout time.Duration) Option {
    return func(s *Server) {
        s.Timeout = timeout
    }
}
func MaxConns(maxconns int) Option {
    return func(s *Server) {
        s.MaxConns = maxconns
    }
}
func TLS(tls *tls.Config) Option {
    return func(s *Server) {
        s.TLS = tls
    }
}

// Server的构造函数
func NewServer(addr string, port int, options ...func(*Server)) (*Server, error) {
  srv := Server{
    Addr:     addr,
    Port:     port,
    Protocol: "tcp",
    Timeout:  30 * time.Second,
    MaxConns: 1000,
    TLS:      nil,
  }
  for _, option := range options {
    option(&srv)
  }
  //...
  return &srv, nil
}

// 使用方式
s1, _ := NewServer("localhost", 1024)
s2, _ := NewServer("localhost", 2048, Protocol("udp"))
s3, _ := NewServer("0.0.0.0", 8080, Timeout(300*time.Second), MaxConns(1000))
```

## [委托和反转控制](https://coolshell.cn/articles/21214.html)

反转控制IoC – Inversion of Control 是一种软件设计的方法，其主要的思想是把控制逻辑与业务逻辑分享，不要在业务逻辑里写控制逻辑，这样会让控制逻辑依赖于业务逻辑，而是反过来，让业务逻辑依赖控制逻辑。

### 嵌入和委托

#### 结构体嵌入

在Go语言中，我们可以很方便的把一个结构体给嵌到另一个结构体中。

```go
type Widget struct {
    X, Y int
}
type Label struct {
    Widget        // Embedding (delegation)
    Text   string // Aggregation
}

// 使用
label := Label{Widget{10, 10}, "State:"}
label.X = 11
label.Y = 12
```

#### 方法重写

```go
// 绘画
type Painter interface {
    Paint()
}
 
// 点击
type Clicker interface {
    Click()
}

type Button struct {
    Label // Embedding (delegation)
}
type ListBox struct {
    Widget          // Embedding (delegation)
    Texts  []string // Aggregation
    Index  int      // Aggregation
}

func (label Label) Paint() {
  fmt.Printf("%p:Label.Paint(%q)\n", &label, label.Text)
}
// 因为这个接口可以通过 Label 的嵌入带到新的结构体，
// 所以可以在 Button 中可以重载这个接口方法
func (button Button) Paint() { // Override
    fmt.Printf("Button.Paint(%s)\n", button.Text)
}
func (button Button) Click() {
    fmt.Printf("Button.Click(%s)\n", button.Text)
}
func (listBox ListBox) Paint() {
    fmt.Printf("ListBox.Paint(%q)\n", listBox.Texts)
}
func (listBox ListBox) Click() {
    fmt.Printf("ListBox.Click(%q)\n", listBox.Texts)
}
```

Button.Paint() 接口可以通过 Label 的嵌入带到新的结构体，如果 Button.Paint() 不实现的话，会调用 Label.Paint() ，所以，在 Button 中声明 Paint() 方法，相当于Override。

#### 嵌入结果多态

```go
button1 := Button{Label{Widget{10, 70}, "OK"}}
button2 := NewButton(50, 70, "Cancel")
listBox := ListBox{Widget{10, 40}, 
    []string{"AL", "AK", "AZ", "AR"}, 0}
for _, painter := range []Painter{label, listBox, button1, button2} {
    painter.Paint()
}
 
for _, widget := range []interface{}{label, listBox, button1, button2} {
  widget.(Painter).Paint()
  if clicker, ok := widget.(Clicker); ok {
    clicker.Click()
  }
  fmt.Println() // print a empty line 
}
```

### 反转控制

```go
type IntSet struct {
    data map[int]bool
}
func NewIntSet() IntSet {
    return IntSet{make(map[int]bool)}
}
func (set *IntSet) Add(x int) {
    set.data[x] = true
}
func (set *IntSet) Delete(x int) {
    delete(set.data, x)
}
func (set *IntSet) Contains(x int) bool {
    return set.data[x]
}
```

#### 实现Undo功能

```go
type UndoableIntSet struct { // Poor style
    IntSet    // Embedding (delegation)
    functions []func()
}
 
func NewUndoableIntSet() UndoableIntSet {
    return UndoableIntSet{NewIntSet(), nil}
}
 
func (set *UndoableIntSet) Add(x int) { // Override
    if !set.Contains(x) {
        set.data[x] = true
        set.functions = append(set.functions, func() { set.Delete(x) })
    } else {
        set.functions = append(set.functions, nil)
    }
}
func (set *UndoableIntSet) Delete(x int) { // Override
    if set.Contains(x) {
        delete(set.data, x)
        set.functions = append(set.functions, func() { set.Add(x) })
    } else {
        set.functions = append(set.functions, nil)
    }
}
func (set *UndoableIntSet) Undo() error {
    if len(set.functions) == 0 {
        return errors.New("No functions to undo")
    }
    index := len(set.functions) - 1
    if function := set.functions[index]; function != nil {
        function()
        set.functions[index] = nil // For garbage collection
    }
    set.functions = set.functions[:index]
    return nil
}
```

- 我们在 UndoableIntSet 中嵌入了IntSet ，然后Override了 它的 Add()和 Delete() 方法。
- Contains() 方法没有Override，所以，会被带到 UndoableInSet 中来了。
- 在Override的 Add()中，记录 Delete 操作
- 在Override的 Delete() 中，记录 Add 操作
- 在新加入 Undo() 中进行Undo操作。

#### 反转依赖

```go
// 声明函数接口
type Undo []func()

// Undo的控制逻辑
func (undo *Undo) Add(function func()) {
  *undo = append(*undo, function)
}
func (undo *Undo) Undo() error {
  functions := *undo
  if len(functions) == 0 {
    return errors.New("No functions to undo")
  }
  index := len(functions) - 1
  if function := functions[index]; function != nil {
    function()
    functions[index] = nil // For garbage collection
  }
  *undo = functions[:index]
  return nil
}

type IntSet struct {
    data map[int]bool
    undo Undo
}
 
func NewIntSet() IntSet {
    return IntSet{data: make(map[int]bool)}
}
func (set *IntSet) Undo() error {
    return set.undo.Undo()
}
 
func (set *IntSet) Contains(x int) bool {
    return set.data[x]
}
func (set *IntSet) Add(x int) {
    if !set.Contains(x) {
        set.data[x] = true
        set.undo.Add(func() { set.Delete(x) })
    } else {
        set.undo.Add(nil)
    }
}
 
func (set *IntSet) Delete(x int) {
    if set.Contains(x) {
        delete(set.data, x)
        set.undo.Add(func() { set.Add(x) })
    } else {
        set.undo.Add(nil)
    }
}
```

## [Map-Reduce](https://coolshell.cn/articles/21164.html)

### Map

```go
// 定义map方法
func MapStrToStr(arr []string, fn func(s string) string) []string {
    var newArray = []string{}
    for _, it := range arr {
        newArray = append(newArray, fn(it))
    }
    return newArray
}
func MapStrToInt(arr []string, fn func(s string) int) []int {
    var newArray = []int{}
    for _, it := range arr {
        newArray = append(newArray, fn(it))
    }
    return newArray
}
// 使用
var list = []string{"Hao", "Chen", "MegaEase"}
x := MapStrToStr(list, func(s string) string {
    return strings.ToUpper(s)
})
fmt.Printf("%v\n", x)
//["HAO", "CHEN", "MEGAEASE"]
y := MapStrToInt(list, func(s string) int {
    return len(s)
})
fmt.Printf("%v\n", y)
//[3, 4, 8]
```

### Reduce

```go
func Reduce(arr []string, fn func(s string) int) int {
    sum := 0
    for _, it := range arr {
        sum += fn(it)
    }
    return sum
}

var list = []string{"Hao", "Chen", "MegaEase"}
x := Reduce(list, func(s string) int {
    return len(s)
})
fmt.Printf("%v\n", x)
```

### Filter

```go
func Filter(arr []int, fn func(n int) bool) []int {
    var newArray = []int{}
    for _, it := range arr {
        if fn(it) {
            newArray = append(newArray, it)
        }
    }
    return newArray
}

var intset = []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
out := Filter(intset, func(n int) bool {
   return n%2 == 1
})
fmt.Printf("%v\n", out)
out = Filter(intset, func(n int) bool {
    return n > 5
})
fmt.Printf("%v\n", out)
```

### 泛型化

```go
// 通过 reflect.ValueOf() 来获得 interface{} 的值，其中一个是数据 vdata，另一个是函数 vfn，
// 然后通过 vfn.Call() 方法来调用函数，通过 []refelct.Value{vdata.Index(i)}来获得数据。
func Map(data interface{}, fn interface{}) []interface{} {
    vfn := reflect.ValueOf(fn)
    vdata := reflect.ValueOf(data)
    result := make([]interface{}, vdata.Len())
    for i := 0; i < vdata.Len(); i++ {
        result[i] = vfn.Call([]reflect.Value{vdata.Index(i)})[0].Interface()
    }
    return result
}
```

## [Go Generation](https://coolshell.cn/articles/21179.html)

### Go的类型检查

#### TypeAssert
