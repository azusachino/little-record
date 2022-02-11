# levelDB

LEVEL_DB 是一个键值数据库

## 基本属性

### String & Slice

```c++
// Slice与std::string相互转换
class LEVELDB_EXPORT Slice {
public:
    Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}

    std::string ToString() const {
        return std::string(data_, size_);
    }
};
```

### 错误处理 Status

```c++
enum Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5
};
```

### Key 比较函数接口 Comparator

纯虚函数只有函数的名字而不具备函数的功能，不能被调用。  
纯虚函数的作用是在基类中为其派生类保留一个函数的名字，以便派生类根据需要对他进行定义。如果在基类中没有保留函数名字，则无法实现多态性。  
如果在一个类中声明了纯虚函数，在其派生类中没有对其函数进行定义，则该虚函数在派生类中仍然为纯虚函数。

```c++
class Comparator {
public:
    virtual ~Comparator();
    virtual int Compare(const Slice &a, const Slice &b) const = 0;
    virtual const char* Name() const = 0;
    virtual void FindShortestSeparator(std::string *start, const Slice &limit) const = 0;
    virtual void FindShortSuccessor(std::string *key) const = 0;
};
```
