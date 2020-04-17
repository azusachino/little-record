# 网络知识考点

## OSI开放式互联网

- 应用层
- 表示层
- 会话层
- 传输层
- 网络层
- 数据链接层
- 物理层

## HTTP主要特点

- 支持客户/服务端模式
- 简单快速
- 灵活
- 无连接
- 无状态

### 请求/响应的步骤

1. 客户端连接到Web服务器
2. 发送HTTP请求
3. 服务器接受请求并返回HTTP响应
4. 释放TCP连接
5. 客户端浏览器解析HTML内容

> 在浏览器地址栏输入URL后, 按下回车之后的流程

1. DNS解析
2. TCP连接
3. 发送HTTP请求
4. 服务器处理请求并返回HTTP报文
5. 浏览器解析渲染页面
6. 连接结束

> 常见的Http状态码

- 1xx: 指示信息- 表示请求已接收, 继续处理
- 2xx: 成功 -- 表示请求已被成功接收, 理解, 接受
- 3xx: 重定向 -- 要完成请求必须进行更进一步的操作
- 4xx: 客户端错误 -- 请求有语法错误或请求无法实现
- 5xx: 服务端错误 -- 服务器未能实现合法的请求

> GET和POST请求的区别

- HTTP报文层面: GET将请求信息放在URL, POST放在报文体中
- 数据库层面: GET符合幂等性和安全性, POST不符合
- 其他层面: GET可以被缓存, 被存储, 而POST不行

> Cookie和Session

Cookie是由服务器发给客户端的特殊信息, 以文本的形式存放在客户端, 客户端再次请求的时候, 会把Cookie回发, 服务器接收到后, 会解析Cookie生成与客户端保存相同的内容  
Session是服务器端的机制, 在服务器上保存的信息, 解析客户端请求并操作session id, 按需保存状态信息

区别:

- Cookie数据存放在客户的浏览器上, Session数据放在服务器上
- Session相对于Cookie更安全
- 若考虑减轻服务器负担, 应当使用Cookie

> HTTP和HTTPS的区别

SSL: 安全套接层(Security Sockets Layer)

- 为网络通信提供安全及数据完整性的一种安全协议
- 是操作系统对外的API, SSL3.0后更名为TLS
- 采用身份验证和数据加密保证网络通信的安全和数据的完整性

加密方式:

- 对称加密: 加密和解密都是用同一个密钥
- 非对称加密: 加密使用的密钥和解密使用的密钥是不相同的
- 哈希算法: 将任意长度的信息转换为固定长度的值, 算法不可逆
- 数字签名: 证明某个消息或者文件时某人发出/认同的

区别:

- HTTPS需要到CA申请证书, HTTP不需要
- HTTPS密文传输, HTTP明文传输
- HTTPS默认使用443端口, HTTP使用80端口
- HTTPS = HTTP + 加密 + 认证 + 完整性保护

> SOCKET通信流程

SERVER: 创建socket -> 绑定socket和端口号 -> 监听该端口号 -> 接收来自client的连接请求 -> 从socket中读取字段 -> 关闭socket
CLIENT: 创建socket -> 连接指定计算机的端口 -> 向socket中写入信息 -> 关闭socket

> http1.0和http1.1有什么区别?

1. 缓存处理，在HTTP1.0中主要使用header里的If-Modified-Since,Expires来做为缓存判断的标准，HTTP1.1则引入了更多的缓存控制策略例如Entity tag，If-Unmodified-Since, If-Match, If-None-Match等更多可供选择的缓存头来控制缓存策略。
2. 带宽优化及网络连接的使用，HTTP1.0中，存在一些浪费带宽的现象，例如客户端只是需要某个对象的一部分，而服务器却将整个对象送过来了，并且不支持断点续传功能，HTTP1.1则在请求头引入了range头域，它允许只请求资源的某个部分，即返回码是206（Partial Content），这样就方便了开发者自由的选择以便于充分利用带宽和连接。
3. 错误通知的管理，在HTTP1.1中新增了24个错误状态响应码，如409（Conflict）表示请求的资源与资源的当前状态发生冲突；410（Gone）表示服务器上的某个资源被永久性的删除。
4. Host头处理，在HTTP1.0中认为每台服务器都绑定一个唯一的IP地址，因此，请求消息中的URL并没有传递主机名（hostname）。但随着虚拟主机技术的发展，在一台物理服务器上可以存在多个虚拟主机（Multi-homed Web Servers），并且它们共享一个IP地址。HTTP1.1的请求消息和响应消息都应支持Host头域，且请求消息中如果没有Host头域会报告一个错误（400 Bad Request）。

5. 长连接，HTTP 1.1支持长连接（PersistentConnection）和请求的流水线（Pipelining）处理，在一个TCP连接上可以传送多个HTTP请求和响应，减少了建立和关闭连接的消耗和延迟，在HTTP1.1中默认开启Connection： keep-alive，一定程度上弥补了HTTP1.0每次请求都要创建连接的缺点。

SPDY：HTTP1.x的优化

1.降低延迟，针对HTTP高延迟的问题，SPDY优雅的采取了多路复用（multiplexing）。多路复用通过多个请求stream共享一个tcp连接的方式，解决了HOL blocking的问题，降低了延迟同时提高了带宽的利用率。
2. 请求优先级（request prioritization）。多路复用带来一个新的问题是，在连接共享的基础之上有可能会导致关键请求被阻塞。SPDY允许给每个request设置优先级，这样重要的请求就会优先得到响应。比如浏览器加载首页，首页的html内容应该优先展示，之后才是各种静态资源文件，脚本文件等加载，这样可以保证用户能第一时间看到网页内容。
3. header压缩。前面提到HTTP1.x的header很多时候都是重复多余的。选择合适的压缩算法可以减小包的大小和数量。
4. 基于HTTPS的加密协议传输，大大提高了传输数据的可靠性。
5. 服务端推送（server push），采用了SPDY的网页，例如我的网页有一个sytle.css的请求，在客户端收到sytle.css数据的同时，服务端会将sytle.js的文件推送给客户端，当客户端再次尝试获取sytle.js时就可以直接从缓存中获取到，不用再发请求了。

> TIME_WAIT和CLOSE_WAIT的区别

ESTABLISHED 表示正在通信，TIME_WAIT 表示主动关闭，CLOSE_WAIT 表示被动关闭。

TIME_WAIT 是主动关闭连接时形成的，等待2MSL时间，约4分钟。主要是防止最后一个ACK丢失。 由于TIME_WAIT 的时间会非常长，因此server端应尽量减少主动关闭连接

CLOSE_WAIT 是被动关闭连接是形成的。根据TCP状态机，服务器端收到客户端发送的FIN，则按照TCP实现发送ACK，因此进入CLOSE_WAIT状态。但如果服务器端不执行close()，就不能由CLOSE_WAIT迁移到LAST_ACK，则系统中会存在很多CLOSE_WAIT状态的连接。此时，可能是系统忙于处理读、写操作，而未将已收到FIN的连接，进行close。此时，recv/read已收到FIN的连接socket，会返回0。

> 说说你知道的几种HTTP状态码, 比如200, 304, 400

```java
 // 1xx Informational

 /**
  * {@code 100 Continue}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.2.1">HTTP/1.1: Semantics and Content, section 6.2.1</a>
  */
 CONTINUE(100, "Continue"),
 /**
  * {@code 101 Switching Protocols}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.2.2">HTTP/1.1: Semantics and Content, section 6.2.2</a>
  */
 SWITCHING_PROTOCOLS(101, "Switching Protocols"),
 /**
  * {@code 102 Processing}.
  * @see <a href="https://tools.ietf.org/html/rfc2518#section-10.1">WebDAV</a>
  */
 PROCESSING(102, "Processing"),
 /**
  * {@code 103 Checkpoint}.
  * @see <a href="https://code.google.com/p/gears/wiki/ResumableHttpRequestsProposal">A proposal for supporting
  * resumable POST/PUT HTTP requests in HTTP/1.0</a>
  */
 CHECKPOINT(103, "Checkpoint"),

 // 2xx Success

 /**
  * {@code 200 OK}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.3.1">HTTP/1.1: Semantics and Content, section 6.3.1</a>
  */
 OK(200, "OK"),
 /**
  * {@code 201 Created}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.3.2">HTTP/1.1: Semantics and Content, section 6.3.2</a>
  */
 CREATED(201, "Created"),
 /**
  * {@code 202 Accepted}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.3.3">HTTP/1.1: Semantics and Content, section 6.3.3</a>
  */
 ACCEPTED(202, "Accepted"),
 /**
  * {@code 203 Non-Authoritative Information}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.3.4">HTTP/1.1: Semantics and Content, section 6.3.4</a>
  */
 NON_AUTHORITATIVE_INFORMATION(203, "Non-Authoritative Information"),
 /**
  * {@code 204 No Content}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.3.5">HTTP/1.1: Semantics and Content, section 6.3.5</a>
  */
 NO_CONTENT(204, "No Content"),
 /**
  * {@code 205 Reset Content}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.3.6">HTTP/1.1: Semantics and Content, section 6.3.6</a>
  */
 RESET_CONTENT(205, "Reset Content"),
 /**
  * {@code 206 Partial Content}.
  * @see <a href="https://tools.ietf.org/html/rfc7233#section-4.1">HTTP/1.1: Range Requests, section 4.1</a>
  */
 PARTIAL_CONTENT(206, "Partial Content"),
 /**
  * {@code 207 Multi-Status}.
  * @see <a href="https://tools.ietf.org/html/rfc4918#section-13">WebDAV</a>
  */
 MULTI_STATUS(207, "Multi-Status"),
 /**
  * {@code 208 Already Reported}.
  * @see <a href="https://tools.ietf.org/html/rfc5842#section-7.1">WebDAV Binding Extensions</a>
  */
 ALREADY_REPORTED(208, "Already Reported"),
 /**
  * {@code 226 IM Used}.
  * @see <a href="https://tools.ietf.org/html/rfc3229#section-10.4.1">Delta encoding in HTTP</a>
  */
 IM_USED(226, "IM Used"),

 // 3xx Redirection

 /**
  * {@code 300 Multiple Choices}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.4.1">HTTP/1.1: Semantics and Content, section 6.4.1</a>
  */
 MULTIPLE_CHOICES(300, "Multiple Choices"),
 /**
  * {@code 301 Moved Permanently}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.4.2">HTTP/1.1: Semantics and Content, section 6.4.2</a>
  */
 MOVED_PERMANENTLY(301, "Moved Permanently"),
 /**
  * {@code 302 Found}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.4.3">HTTP/1.1: Semantics and Content, section 6.4.3</a>
  */
 FOUND(302, "Found"),
 /**
  * {@code 303 See Other}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.4.4">HTTP/1.1: Semantics and Content, section 6.4.4</a>
  */
 SEE_OTHER(303, "See Other"),
 /**
  * {@code 304 Not Modified}.
  * @see <a href="https://tools.ietf.org/html/rfc7232#section-4.1">HTTP/1.1: Conditional Requests, section 4.1</a>
  */
 NOT_MODIFIED(304, "Not Modified"),
 /**
  * {@code 307 Temporary Redirect}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.4.7">HTTP/1.1: Semantics and Content, section 6.4.7</a>
  */
 TEMPORARY_REDIRECT(307, "Temporary Redirect"),
 /**
  * {@code 308 Permanent Redirect}.
  * @see <a href="https://tools.ietf.org/html/rfc7238">RFC 7238</a>
  */
 PERMANENT_REDIRECT(308, "Permanent Redirect"),

 // --- 4xx Client Error ---

 /**
  * {@code 400 Bad Request}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.1">HTTP/1.1: Semantics and Content, section 6.5.1</a>
  */
 BAD_REQUEST(400, "Bad Request"),
 /**
  * {@code 401 Unauthorized}.
  * @see <a href="https://tools.ietf.org/html/rfc7235#section-3.1">HTTP/1.1: Authentication, section 3.1</a>
  */
 UNAUTHORIZED(401, "Unauthorized"),
 /**
  * {@code 402 Payment Required}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.2">HTTP/1.1: Semantics and Content, section 6.5.2</a>
  */
 PAYMENT_REQUIRED(402, "Payment Required"),
 /**
  * {@code 403 Forbidden}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.3">HTTP/1.1: Semantics and Content, section 6.5.3</a>
  */
 FORBIDDEN(403, "Forbidden"),
 /**
  * {@code 404 Not Found}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.4">HTTP/1.1: Semantics and Content, section 6.5.4</a>
  */
 NOT_FOUND(404, "Not Found"),
 /**
  * {@code 405 Method Not Allowed}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.5">HTTP/1.1: Semantics and Content, section 6.5.5</a>
  */
 METHOD_NOT_ALLOWED(405, "Method Not Allowed"),
 /**
  * {@code 406 Not Acceptable}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.6">HTTP/1.1: Semantics and Content, section 6.5.6</a>
  */
 NOT_ACCEPTABLE(406, "Not Acceptable"),
 /**
  * {@code 407 Proxy Authentication Required}.
  * @see <a href="https://tools.ietf.org/html/rfc7235#section-3.2">HTTP/1.1: Authentication, section 3.2</a>
  */
 PROXY_AUTHENTICATION_REQUIRED(407, "Proxy Authentication Required"),
 /**
  * {@code 408 Request Timeout}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.7">HTTP/1.1: Semantics and Content, section 6.5.7</a>
  */
 REQUEST_TIMEOUT(408, "Request Timeout"),
 /**
  * {@code 409 Conflict}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.8">HTTP/1.1: Semantics and Content, section 6.5.8</a>
  */
 CONFLICT(409, "Conflict"),
 /**
  * {@code 410 Gone}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.9">
  *     HTTP/1.1: Semantics and Content, section 6.5.9</a>
  */
 GONE(410, "Gone"),
 /**
  * {@code 411 Length Required}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.10">
  *     HTTP/1.1: Semantics and Content, section 6.5.10</a>
  */
 LENGTH_REQUIRED(411, "Length Required"),
 /**
  * {@code 412 Precondition failed}.
  * @see <a href="https://tools.ietf.org/html/rfc7232#section-4.2">
  *     HTTP/1.1: Conditional Requests, section 4.2</a>
  */
 PRECONDITION_FAILED(412, "Precondition Failed"),
 /**
  * {@code 413 Payload Too Large}.
  * @since 4.1
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.11">
  *     HTTP/1.1: Semantics and Content, section 6.5.11</a>
  */
 PAYLOAD_TOO_LARGE(413, "Payload Too Large"),

 /**
  * {@code 414 URI Too Long}.
  * @since 4.1
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.12">
  *     HTTP/1.1: Semantics and Content, section 6.5.12</a>
  */
 URI_TOO_LONG(414, "URI Too Long"),

 /**
  * {@code 415 Unsupported Media Type}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.13">
  *     HTTP/1.1: Semantics and Content, section 6.5.13</a>
  */
 UNSUPPORTED_MEDIA_TYPE(415, "Unsupported Media Type"),
 /**
  * {@code 416 Requested Range Not Satisfiable}.
  * @see <a href="https://tools.ietf.org/html/rfc7233#section-4.4">HTTP/1.1: Range Requests, section 4.4</a>
  */
 REQUESTED_RANGE_NOT_SATISFIABLE(416, "Requested range not satisfiable"),
 /**
  * {@code 417 Expectation Failed}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.5.14">
  *     HTTP/1.1: Semantics and Content, section 6.5.14</a>
  */
 EXPECTATION_FAILED(417, "Expectation Failed"),
 /**
  * {@code 418 I'm a teapot}.
  * @see <a href="https://tools.ietf.org/html/rfc2324#section-2.3.2">HTCPCP/1.0</a>
  */
 I_AM_A_TEAPOT(418, "I'm a teapot"),
 /**
  * @deprecated See
  * <a href="https://tools.ietf.org/rfcdiff?difftype=--hwdiff&url2=draft-ietf-webdav-protocol-06.txt">
  *     WebDAV Draft Changes</a>
  */
 @Deprecated
 INSUFFICIENT_SPACE_ON_RESOURCE(419, "Insufficient Space On Resource"),
 /**
  * @deprecated See
  * <a href="https://tools.ietf.org/rfcdiff?difftype=--hwdiff&url2=draft-ietf-webdav-protocol-06.txt">
  *     WebDAV Draft Changes</a>
  */
 @Deprecated
 METHOD_FAILURE(420, "Method Failure"),
 /**
  * @deprecated
  * See <a href="https://tools.ietf.org/rfcdiff?difftype=--hwdiff&url2=draft-ietf-webdav-protocol-06.txt">
  *     WebDAV Draft Changes</a>
  */
 @Deprecated
 DESTINATION_LOCKED(421, "Destination Locked"),
 /**
  * {@code 422 Unprocessable Entity}.
  * @see <a href="https://tools.ietf.org/html/rfc4918#section-11.2">WebDAV</a>
  */
 UNPROCESSABLE_ENTITY(422, "Unprocessable Entity"),
 /**
  * {@code 423 Locked}.
  * @see <a href="https://tools.ietf.org/html/rfc4918#section-11.3">WebDAV</a>
  */
 LOCKED(423, "Locked"),
 /**
  * {@code 424 Failed Dependency}.
  * @see <a href="https://tools.ietf.org/html/rfc4918#section-11.4">WebDAV</a>
  */
 FAILED_DEPENDENCY(424, "Failed Dependency"),
 /**
  * {@code 425 Too Early}.
  * @since 5.2
  * @see <a href="https://tools.ietf.org/html/rfc8470">RFC 8470</a>
  */
 TOO_EARLY(425, "Too Early"),
 /**
  * {@code 426 Upgrade Required}.
  * @see <a href="https://tools.ietf.org/html/rfc2817#section-6">Upgrading to TLS Within HTTP/1.1</a>
  */
 UPGRADE_REQUIRED(426, "Upgrade Required"),
 /**
  * {@code 428 Precondition Required}.
  * @see <a href="https://tools.ietf.org/html/rfc6585#section-3">Additional HTTP Status Codes</a>
  */
 PRECONDITION_REQUIRED(428, "Precondition Required"),
 /**
  * {@code 429 Too Many Requests}.
  * @see <a href="https://tools.ietf.org/html/rfc6585#section-4">Additional HTTP Status Codes</a>
  */
 TOO_MANY_REQUESTS(429, "Too Many Requests"),
 /**
  * {@code 431 Request Header Fields Too Large}.
  * @see <a href="https://tools.ietf.org/html/rfc6585#section-5">Additional HTTP Status Codes</a>
  */
 REQUEST_HEADER_FIELDS_TOO_LARGE(431, "Request Header Fields Too Large"),
 /**
  * {@code 451 Unavailable For Legal Reasons}.
  * @see <a href="https://tools.ietf.org/html/draft-ietf-httpbis-legally-restricted-status-04">
  * An HTTP Status Code to Report Legal Obstacles</a>
  * @since 4.3
  */
 UNAVAILABLE_FOR_LEGAL_REASONS(451, "Unavailable For Legal Reasons"),

 // --- 5xx Server Error ---

 /**
  * {@code 500 Internal Server Error}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.6.1">HTTP/1.1: Semantics and Content, section 6.6.1</a>
  */
 INTERNAL_SERVER_ERROR(500, "Internal Server Error"),
 /**
  * {@code 501 Not Implemented}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.6.2">HTTP/1.1: Semantics and Content, section 6.6.2</a>
  */
 NOT_IMPLEMENTED(501, "Not Implemented"),
 /**
  * {@code 502 Bad Gateway}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.6.3">HTTP/1.1: Semantics and Content, section 6.6.3</a>
  */
 BAD_GATEWAY(502, "Bad Gateway"),
 /**
  * {@code 503 Service Unavailable}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.6.4">HTTP/1.1: Semantics and Content, section 6.6.4</a>
  */
 SERVICE_UNAVAILABLE(503, "Service Unavailable"),
 /**
  * {@code 504 Gateway Timeout}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.6.5">HTTP/1.1: Semantics and Content, section 6.6.5</a>
  */
 GATEWAY_TIMEOUT(504, "Gateway Timeout"),
 /**
  * {@code 505 HTTP Version Not Supported}.
  * @see <a href="https://tools.ietf.org/html/rfc7231#section-6.6.6">HTTP/1.1: Semantics and Content, section 6.6.6</a>
  */
 HTTP_VERSION_NOT_SUPPORTED(505, "HTTP Version not supported"),
 /**
  * {@code 506 Variant Also Negotiates}
  * @see <a href="https://tools.ietf.org/html/rfc2295#section-8.1">Transparent Content Negotiation</a>
  */
 VARIANT_ALSO_NEGOTIATES(506, "Variant Also Negotiates"),
 /**
  * {@code 507 Insufficient Storage}
  * @see <a href="https://tools.ietf.org/html/rfc4918#section-11.5">WebDAV</a>
  */
 INSUFFICIENT_STORAGE(507, "Insufficient Storage"),
 /**
  * {@code 508 Loop Detected}
  * @see <a href="https://tools.ietf.org/html/rfc5842#section-7.2">WebDAV Binding Extensions</a>
   */
 LOOP_DETECTED(508, "Loop Detected"),
 /**
  * {@code 509 Bandwidth Limit Exceeded}
   */
 BANDWIDTH_LIMIT_EXCEEDED(509, "Bandwidth Limit Exceeded"),
 /**
  * {@code 510 Not Extended}
  * @see <a href="https://tools.ietf.org/html/rfc2774#section-7">HTTP Extension Framework</a>
  */
 NOT_EXTENDED(510, "Not Extended"),
 /**
  * {@code 511 Network Authentication Required}.
  * @see <a href="https://tools.ietf.org/html/rfc6585#section-6">Additional HTTP Status Codes</a>
  */
 NETWORK_AUTHENTICATION_REQUIRED(511, "Network Authentication Required");
```

> 当你用浏览器打开一个链接时, 计算机做了哪些工作?

1. DNS解析过程
2. HTTP请求过程
3. 建立连接
4. 发送HTTP请求
5. 返回HTTP响应
6. 维持连接
7. 断开连接
8. 浏览器解析过程
9. 主流程
10. 渲染流程
11. 页面生命周期

> TCP/IP如何保证可靠性, 说说TCP头的结构

保证可靠性:

1、将数据截断为合理的长度;
2、超时重发;
3、对于收到的请求，给出确认响应;
4、 校验出包有错，丢弃报文段，不给出响应，TCP发送数据端，超时时会重发数据;
5、对失序数据进行重新排序，然后才交给应用层;
6、对于重复数据，能够丢弃重复数据;
7、TCP可以进行流量控制，防止较快主机致使较慢主机的缓冲区溢出;

TCP固定头部结构：

16位端口号：包括了源端口号和目的端口号。进行TCP通信时，客户端通常使用系统自动选择的临时端口号(一般都很大)，而服务器则使用知服务端口号或服务器管理员自定义的端口号。
32位序号：一次TCP通信过程中对一个传输方向上的字节流的每个字节的编号(从这个方向第一个报文段依次排列)。假设主机A和主机B进行TCP通信，A发送给B的第一个TCP报文段中的序号值是系统初始化的一个随机值ISN(初始序号值)。那么之后在A到B的方向上发送的TCP报文段中的序号值将会被系统设置为ISN加上该报文段所携带数据的第一个字节在整个数据字节流中的偏移。
32位确认号：用作对另一方发送来的TCP报文段做出相应。其值是收到对方的报文段的序号值加1。
4位头部长度：标识该TCP头部有多少个32bit字(4字节)。一共有4位，所以最大能表示TCP头部大小为60字节。
6位标志位包含如下几项：
（1）URG标志，表示紧急指针是否有效。
（2）ACK标志，表示确认号是否有效。一般称携带ACK标志的报文段是确认报文段。
（3）PSH标志，提示接收端应用程序立即从TCP接受缓冲区读走数据。
（4）RST标志，表示要求对方重新建立连接。称携带RST标志的TCP报文段为复位报文段。
（5）SYN标志，表示请求建立一个连接。称携带SYN标志的TCP报文段为同步报文段。
（6）FIN标志，表示通知对方本端将关闭连接。称携带FIN标志的TCP报文段为结束报文段。
16位窗口大小：是TCP流量控制的一个手段。这里说的窗口指的是接收通告窗口(RWND)。它告诉对方本端TCP接收缓冲区还能容纳多少字节的数据，以让对方控制发送数据的速度。
16位校验和：由发送端填充，接收端对TCP报文段执行CRC算法以效验TCP报文段在传输过程中是否损坏(包括TCP头部和数据部分)。这也是TCP可靠传输的一个重要保障。
16位紧急指针：是一个正的偏移量。它和序号字段的值相加表示最后一个紧急数据的下一字节的序号。即这个字段是紧急指针相对当前序号的偏移，为紧急偏移。以让接收端迅速接受到紧急数据。TCP的紧急指针是发送端向接收端发送紧急数据的方法。
> 如何避免浏览器缓存

无法被浏览器缓存的请求：HTTP信息头中包Cache-Control:no-cache，pragma:no-cache，或Cache-Control:max-age=0等告诉浏览器不用缓存的请求需要根据Cookie，认证信息等决定输入内容的动态请求是不能被缓存的经过HTTPS安全加密的请求
> 如何理解HTTP协议的无状态

无连接的含义是限制每次连接只处理一个请求。服务器处理完客户的请求，并收到客户的应答后，即断开连接。采用这种方式可以节省传输时间。

无状态是指协议对于事务处理没有记忆能力，服务器不知道客户端是什么状态。即我们给服务器发送 HTTP 请求之后，服务器根据请求，会给我们发送数据过来，但是发送完，不会记录任何信息。
> 简述Http请求GET和POST的区别及数据包格式

Get请求, 数据附加在URL之后, POST会把请求的数据放置在HTTP请求包

POST传输数据理论上没有限制, GET受到URL长度限制

POST相对比GET安全
> HTTP有哪些method

get, post, put, patch, delete, options, trace, head

> 简述HTTP请求的报文格式

- 请求行（request line): GET HTTP/1.1
- 消息头部（header): User-Agent: xxx
- 空行
- 请求正文: Request body

> HTTP的长连接是什么意思

在HTTP/1.0中默认使用短连接。也就是说，客户端和服务器每进行一次HTTP操作，就建立一次连接，任务结束就中断连接。

从HTTP/1.1起，默认使用长连接，用以保持连接特性。使用长连接的HTTP协议，会在响应头加入：Connection:keep-alive

长连接可以省去较多的TCP建立和关闭的操作，减少浪费，节约时间。对于频繁请求资源的客户来说，较适用长连接。不过这里存在一个问题，存活功能的探测周期太长，还有就是它只是探测TCP连接的存活，属于比较斯文的做法，遇到恶意的连接时，保活功能就不够使了。在长连接的应用场景下，client端一般不会主动关闭它们之间的连接，Client与server之间的连接如果一直不关闭的话，会存在一个问题，随着客户端连接越来越多，server早晚有扛不住的时候，这时候server端需要采取一些策略，如关闭一些长时间没有读写事件发生的连接，这样可 以避免一些恶意连接导致server端服务受损；如果条件再允许就可以以客户端机器为颗粒度，限制每个客户端的最大长连接数，这样可以完全避免某个蛋疼的客户端连累后端服务。

短连接对于服务器来说管理较为简单，存在的连接都是有用的连接，不需要额外的控制手段。但如果客户请求频繁，将在TCP的建立和关闭操作上浪费时间和带宽。

> HTTPS的加密方式是什么, 讲讲整个加密解密过程

1. 服务器把自己的公开密钥登录至数字证书认证机构。
2. 数字证书认证机构用自己的私有密钥向服务器的公开密码署数字签名并颁发公钥证书。
3. 客户端拿到服务器的公钥证书后，使用数字签名认证机构的公开密钥，向数字证书认证机构验证公钥证书上的数字签名，以确认服务器的公开密钥的真实性。
4. 使用服务器的公开密钥对报文加密后发送。
5. 服务器用私有密钥对报文解密。

> HTTP和HTTPS的三次握手有什么区别?

![tcp](../resources/tcpip/http_handshake.jpg)

![https](../resources/tcpip/https_handshake.png)

- 客户端发起HTTPS请求
- 服务端的配置
  - 采用HTTPS协议的服务器必须要有一套数字证书，可以是自己制作或者CA证书。区别就是自己颁发的证书需要客户端验证通过，才可以继续访问，而使用CA证书则不会弹出提示页面。这套证书其实就是一对公钥和私钥。公钥给别人加密使用，私钥给自己解密使用。
- 传送证书
  - 这个证书其实就是公钥，只是包含了很多信息，如证书的颁发机构，过期时间等。
- 客户端解析证书
  - 这部分工作是有客户端的TLS来完成的，首先会验证公钥是否有效，比如颁发机构，过期时间等，如果发现异常，则会弹出一个警告框，提示证书存在问题。如果证书没有问题，那么就生成一个随即值，然后用证书对该随机值进行加密。
- 传送加密信息
  - 这部分传送的是用证书加密后的随机值，目的就是让服务端得到这个随机值，以后客户端和服务端的通信就可以通过这个随机值来进行加密解密了。
- 服务段解密信息
  - 服务端用私钥解密后，得到了客户端传过来的随机值(私钥)，然后把内容通过该值进行对称加密。所谓对称加密就是，将信息和私钥通过某种算法混合在一起，这样除非知道私钥，不然无法获取内容，而正好客户端和服务端都知道这个私钥，所以只要加密算法够彪悍，私钥够复杂，数据就够安全。
- 传输加密后的信息
  - 这部分信息是服务段用私钥加密后的信息，可以在客户端被还原。
- 客户端解密信息
  - 客户端用之前生成的私钥解密服务段传过来的信息，于是获取了解密后的内容。

> 什么是分块传送? -> Range & Content-Range

断点续传：指的是在上传/下载时，将任务（一个文件或压缩包）人为的划分为几个部分，每一个部分采用一个线程进行上传/下载，如果碰到网络故障，可以从已经上传/下载的部分开始继续上传/下载未完成的部分，而没有必要从头开始上传/下载。可以节省时间，提高速度。
> Session和Cookie的区别?

1，session 在服务器端，cookie 在客户端（浏览器）
2，session 默认被存在在服务器的一个文件里（不是内存）
3，session 的运行依赖 session id，而 session id 是存在 cookie 中的，也就是说，如果浏览器禁用了 cookie ，同时 session 也会失效（但是可以通过其它方式实现，比如在 url 中传递 session_id）
4，session 可以放在 文件、数据库、或内存中都可以。
5，用户验证这种场合一般会用 session
