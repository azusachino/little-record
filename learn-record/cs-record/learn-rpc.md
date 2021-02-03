# RPC

RPC是解决分布式系统通信的一大利器.

- 序列化
- 编解码
- 网络传输

---

- 连接管理
- 健康检测
- 负载均衡
- 优雅启停
- 异常重启
- 业务分组
- 熔断限流

## 定义

RPC的全称是Remote Procedure Call, 即远程过程调用

- 屏蔽远程调用和本地调用的区别
- 隐藏底层网络通信的复杂性

## 序列化

- 头部数据用来声明序列化协议, 序列化版本, 用于高低版本向后兼容
- 对象数据主要包括类名, 签名, 属性名, 属性类型及属性值
- 存在对象引用, 继承的情况下, 递归遍历"写对象"逻辑

### JDK, JSON, Hessian

### Protobuf

```java
/**
  * // Idl 文件格式
  * syntax = "proto3";
  * option java_package = "com.test";
  * option java_outer_classname = "StudentProtobuf";
  *
  * message StudentMsg{
     int32 no = 1;
     string name = 2; 
  }
  */

StudentProtobuf.StudentMsg.Builder builder = StudentProtobuf.StudentMsg.newBuilder().setNo(103);
builder.setName("protobuf");
// to byte
StudentProtobuf.StudentMsg msg = builder.build();
byte[] data = msg.toByteAdday();

// parse from byte
StudentProtobuf.StudentMsg deStudent = StudentProtobuf.StudentMsg.parseFrom(data);

System.out.println(deStudent);
```
