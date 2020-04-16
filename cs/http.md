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

TODO
> TIME_WAIT和CLOSE_WAIT的区别

TODO
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

TODO
> TCP/IP如何保证可靠性, 说说TCP头的结构

TODO
> 如何避免浏览器缓存

TODO
> 如何理解HTTP协议的无状态

TODO
> 简述Http请求GET和POST的区别及数据包格式

TODO
> HTTP有哪些method

get, post, put, patch, delete, option

> 简述HTTP请求的报文格式

TODO
> HTTP的长连接是什么意思

TODO
> HTTPS的加密方式是什么, 讲讲整个加密解密过程

TODO
> HTTP和HTTPS的三次握手有什么区别?

TODO
> 什么是分块传送?

TODO
> Session和Cookie的区别?

1，session 在服务器端，cookie 在客户端（浏览器）
2，session 默认被存在在服务器的一个文件里（不是内存）
3，session 的运行依赖 session id，而 session id 是存在 cookie 中的，也就是说，如果浏览器禁用了 cookie ，同时 session 也会失效（但是可以通过其它方式实现，比如在 url 中传递 session_id）
4，session 可以放在 文件、数据库、或内存中都可以。
5，用户验证这种场合一般会用 session
