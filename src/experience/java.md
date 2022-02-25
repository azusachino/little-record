# Experience center in java

## Be careful with Java 'keyword' (Spring default inject `Collection`)

When accidently set table name and entity name to `Collection`, encounter the exception `org.apache.ibatis.type.TypeException: The alias 'Collection' is already mapped to the value 'java.util.Collection'.`

```java
public void registerAlias(String alias, Class<?> value) {
        if (alias == null) {
            throw new TypeException("The parameter alias cannot be null");
        } else {
            String key = alias.toLowerCase(Locale.ENGLISH);
            if (this.typeAliases.containsKey(key) && this.typeAliases.get(key) != null && !((Class)this.typeAliases.get(key)).equals(value)) {
                throw new TypeException("The alias '" + alias + "' is already mapped to the value '" + ((Class)this.typeAliases.get(key)).getName() + "'.");
            } else {
                this.typeAliases.put(key, value);
            }
        }
    }
```

## Spring 事务

1. @Transactional 只能被应用到 public 方法上, 对于其它非 public 的方法,如果标记了@Transactional 也不会报错,但方法没有事务功能.
2. 用 spring 事务管理器,由 spring 来负责数据库的打开,提交,回滚.默认遇到运行期例外(throw new RuntimeException("注释");)会回滚，即遇到不受检查（unchecked）的例外时回滚；而遇到需要捕获的例外(throw new Exception("注释");)不会回滚,即遇到受检查的例外（就是非运行时抛出的异常，编译器会检查到的异常叫受检查例外或说受检查异常）时，需我们指定方式来让事务回滚要想所有异常都回滚,要加上 @Transactional( rollbackFor={Exception.class,其它异常}) .如果让 unchecked 例外不回滚： @Transactional(notRollbackFor=RunTimeException.class)
3. @Transactional 注解应该只被应用到 public 可见度的方法上。 如果你在 protected、private 或者 package-visible 的方法上使用 @Transactional 注解，它也不会报错， 但是这个被注解的方法将不会展示已配置的事务设置。
4. @Transactional 注解可以被应用于接口定义和接口方法、类定义和类的 public 方法上。然而，请注意仅仅 @Transactional 注解的出现不足于开启事务行为，它仅仅 是一种元数据，能够被可以识别 @Transactional 注解和上述的配置适当的具有事务行为的 beans 所使用。上面的例子中，其实正是 元素的出现 开启 了事务行为。
5. Spring 团队的建议是你在具体的类（或类的方法）上使用 @Transactional 注解，而不要使用在类所要实现的任何接口上。你当然可以在接口上使用 @Transactional 注解，但是这将只能当你设置了基于接口的代理时它才生效。因为注解是不能继承的，这就意味着如果你正在使用基于类的代理时，那么事务的设置将不能被基于类的代理所识别，而且对象也将不会被事务代理所包装（将被确认为严重的）。因此，请接受 Spring 团队的建议并且在具体的类上使用 @Transactional 注解。

### 事务传播级别

- @Transactional(propagation=Propagation.REQUIRED) ：如果有事务, 那么加入事务, 没有的话新建一个(默认情况下)
- @Transactional(propagation=Propagation.NOT_SUPPORTED) ：容器不为这个方法开启事务
- @Transactional(propagation=Propagation.REQUIRES_NEW) ：不管是否存在事务,都创建一个新的事务,原来的挂起,新的执行完毕,继续执行老的事务
- @Transactional(propagation=Propagation.MANDATORY) ：必须在一个已有的事务中执行,否则抛出异常
- @Transactional(propagation=Propagation.NEVER) ：必须在一个没有的事务中执行,否则抛出异常(与 Propagation.MANDATORY 相反)
- @Transactional(propagation=Propagation.SUPPORTS) ：如果其他 bean 调用这个方法,在其他 bean 中声明事务,那就用事务.如果其他 bean 没有声明事务,那就不用事务.

### 事务隔离级别

- @Transactional(isolation = Isolation.READ_UNCOMMITTED)：读取未提交数据(会出现脏读, 不可重复读) 基本不使用
- @Transactional(isolation = Isolation.READ_COMMITTED)：读取已提交数据(会出现不可重复读和幻读)
- @Transactional(isolation = Isolation.REPEATABLE_READ)：可重复读(会出现幻读)
- @Transactional(isolation = Isolation.SERIALIZABLE)：串行化

## Feign

通过 FeignClient 调用接口时, GET 接口需要给入参标注@SpringQueryMap

## maven 下载源码

`mvn dependency:sources`

## web flux cors

```java
@Configuration
public class WebFluxConfig extends WebFluxConfigurationSupport {

    private static final String ALLOWED_HEADERS = "Origin, X-Requested-With, Content-Type, Accept";
    private static final String ALLOWED_METHODS = "POST, GET, PATCH, DELETE, PUT";
    private static final String ALLOWED_ORIGIN = "*";
    private static final String MAX_AGE = "3600L";

    @Bean
    public WebFilter corsFilter() {
        return (ServerWebExchange ctx, WebFilterChain chain) -> {
            ServerHttpRequest request = ctx.getRequest();
            if (CorsUtils.isCorsRequest(request)) {
                ServerHttpResponse response = ctx.getResponse();
                HttpHeaders headers = response.getHeaders();
                headers.add("Access-Control-Allow-Origin", ALLOWED_ORIGIN);
                headers.add("Access-Control-Allow-Methods", ALLOWED_METHODS);
                headers.add("Access-Control-Max-Age", MAX_AGE);
                headers.add("Access-Control-Allow-Headers", ALLOWED_HEADERS);
                headers.add("Access-Control-Allow-Credentials", "true");
                if (request.getMethod() == HttpMethod.OPTIONS) {
                    response.setStatusCode(HttpStatus.OK);
                    return Mono.empty();
                }
            }
            return chain.filter(ctx);
        };
    }
}
```

## NIO 读文件

```java
private static void byteBuffer() throws IOException {
  RandomAccessFile randomAccessFile = new RandomAccessFile("/Users/zhengwei/lanqiao/ConditionOperator.java", "rw");
  FileChannel channel = randomAccessFile.getChannel();
  ByteBuffer buffer = ByteBuffer.allocate(10);
  int bytesRead = channel.read(buffer);
  while (bytesRead != -1) {
    // System.out.println("读取字节数："+bytesRead);
    //之前是写buffer，现在要读buffer
    buffer.flip();// 切换模式，写->读
    System.out.print(Charset.forName("utf-8").decode(buffer));  // 这样读取，如果10字节恰好分割了一个字符将出现乱码
    buffer.clear();// 清空,position位置为0，limit=capacity
    //  继续往buffer中写
    bytesRead = channel.read(buffer);
  }
  randomAccessFile.close();
}

private static void readLine() throws IOException {
  RandomAccessFile randomAccessFile = new RandomAccessFile("/Users/zhengwei/lanqiao/ConditionOperator.java", "rw");
  FileChannel channel = randomAccessFile.getChannel();
  ByteBuffer buffer = ByteBuffer.allocate(1024 * 1024);
  int bytesRead = channel.read(buffer);
  ByteBuffer stringBuffer = ByteBuffer.allocate(20);
  while (bytesRead != -1) {
    System.out.println("读取字节数：" + bytesRead);
    //之前是写buffer，现在要读buffer
    buffer.flip();// 切换模式，写->读
    while (buffer.hasRemaining()) {
      byte b = buffer.get();
      if (b == 10 || b == 13) { // 换行或回车
        stringBuffer.flip();
        // 这里就是一个行
        final String line = Charset.forName("utf-8").decode(stringBuffer).toString();
        System.out.println(line + "----------");// 解码已经读到的一行所对应的字节
        stringBuffer.clear();
      } else {
        if (stringBuffer.hasRemaining())
          stringBuffer.put(b);
        else { // 空间不够扩容
          stringBuffer = reAllocate(stringBuffer);
          stringBuffer.put(b);
        }
      }
    }
    buffer.clear();// 清空,position位置为0，limit=capacity
    //  继续往buffer中写
    bytesRead = channel.read(buffer);
  }
  randomAccessFile.close();
}

private static ByteBuffer reAllocate(ByteBuffer stringBuffer) {
  final int capacity = stringBuffer.capacity();
  byte[] newBuffer = new byte[capacity * 2];
  System.arraycopy(stringBuffer.array(), 0, newBuffer, 0, capacity);
  return (ByteBuffer) ByteBuffer.wrap(newBuffer).position(capacity);
}
```

## 按行读文件

```java
// 利用Java8的Stream
Stream<String> stream = Files.lines(Paths.get(fileName));

// BufferedReader
BufferedReader br = Files.newBufferedReader(Paths.get(fileName));
List<String> list = br.lines().collect(Collectors.toList());

// BufferedReader + Scanner
BufferedReader br = new BufferedReader(new FileReader(fileName));
Scanner scanner = new Scanner(new File(fileName));
```

## Spring Webflux 获取 Request

### SpringMVC 场景下

直接调用 RequestContextHolder.getRequestAttributes().getRequest()，即可获得当前请求的 req。（采用了 ThreadLocal）

```java
ServletRequestAttributes requestAttributes = (ServletRequestAttributes)RequestContextHolder.getRequestAttributes();
// get the request
HttpServletRequest request = requestAttributes.getRequest();
```

### SpringWebFlux 场景下

为了能够方便获得 Request 对象，我们就需要在开始的时候把它存在一个可以使用、并且是相同 scope 的容器里。

1. `WebFilter`
2. `reactor.util.context.Context`

```java
@Configuration
@ConditionalOnWebApplication(type = ConditionalOnWebApplication.Type.REACTIVE)
public class ReactiveRequestContextFilter implements WebFilter {
  @Override
  public Mono<Void> filter(ServerWebExchange exchange, WebFilterChain chain) {
    ServerHttpRequest request = exchange.getRequest();
    return chain.filter(exchange)
        .subscriberContext(ctx -> ctx.put(ReactiveRequestContextHolder.CONTEXT_KEY, request));
  }
}

public class ReactiveRequestContextHolder {
  public static final Class<ServerHttpRequest> CONTEXT_KEY = ServerHttpRequest.class;

  public static Mono<ServerHttpRequest> getRequest() {
    return Mono.subscriberContext()
        .map(ctx -> ctx.get(CONTEXT_KEY));
  }
}

@RestController
public class GetRequestController {

  @RequestMapping("/request")
  public Mono<String> getRequest() {
    return ReactiveRequestContextHolder.getRequest()
        .map(request -> request.getHeaders().getFirst("user"));
  }
}
```

## Spring Webflux static file

```java
final AsynchronousFileChannel fileChannel = AsynchronousFileChannel.open(targetPath, StandardOpenOption.WRITE);

AtomicLong fileSize = new AtomicLong(targetPath.toFile().length());

Flux<DataBuffer> fileDataStream = webClient
                .get()
                .uri(remoteXFerServiceTargetHost)
                .accept(MediaType.APPLICATION_OCTET_STREAM)
                .header("Range", String.format("bytes=%d-", fileSize.get()))
                .retrieve()
                .onStatus(HttpStatus::is4xxClientError, clientResponse -> Mono.error(new CustomException("4xx error")))
                .onStatus(HttpStatus::is5xxServerError, clientResponse -> Mono.error(new CustomException("5xx error")))
                .bodyToFlux(DataBuffer.class);

DataBufferUtils
        .write(fileData , fileChannel)
        .map(DataBufferUtils::release)
        .doOnError(throwable -> {
            try {
                fileChannel.force(true);
            } catch (IOException e) {
                e.printStackTrace();
            }
        })
        .retryWhen(Retry.any().fixedBackoff(Duration.ofSeconds(5)).retryMax(5))
        .doOnComplete(() -> {
            try {
                fileChannel.force(true);
            } catch (IOException e) {
                e.printStackTrace();
            }
        })
        .doOnError(e -> !(e instanceof ChannelException), e -> {
            try {
                Files.deleteIfExists(targetPath);
            } catch (IOException exc) {
                exc.printStackTrace();
            }
        })
        .doOnError(ChannelException.class, e -> {
            try {
                Files.deleteIfExists(targetPath);
            } catch (IOException exc) {
                exc.printStackTrace();
            }
        })
        .doOnTerminate(() -> {
            try {
                fileChannel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        })
    .blockLast();
```

## Spring WebFlux 文件相关开发

### Endpoint

```java
// 不同于SpringMVC，采用FilePart
@PostMapping(value = "/upload", consumes = MediaType.MULTIPART_FORM_DATA_VALUE)
public Mono<FileInfo> uploadFile(@RequestPart("file") FilePart filePart) {
    log.info("upload file:{}", filePart.filename());
    return fileService.uploadFile(filePart);
}

@GetMapping("download/{fileId}")
public Mono<Void> downloadFile(@PathVariable String fileId, ServerHttpResponse response) {
    Mono<FileInfo> fileInfoMono = fileService.getFileById(fileId);
    Mono<FileInfo> fallback = Mono.error(new FileNotFoundException("No file was found with fileId: " + fileId));
    return fileInfoMono
        .switchIfEmpty(fallback)
        .flatMap(fileInfo -> {
            var fileName = new String(fileInfo.getDfsFileName().getBytes(Charset.defaultCharset()), StandardCharsets.ISO_8859_1);

            ZeroCopyHttpOutputMessage zeroCopyResponse = (ZeroCopyHttpOutputMessage) response;
            response.getHeaders().set(HttpHeaders.CONTENT_DISPOSITION, "attachment; filename=" + fileName);
            response.getHeaders().setContentType(MediaType.IMAGE_PNG);

            var file = new File(fileInfo.getDfsBucket());
            return zeroCopyResponse.writeWith(file, 0, file.length());
        });
}

@DeleteMapping("{fileId}")
public Mono<Void> deleteFile(@PathVariable String fileId, ServerHttpResponse response) {
    return fileService.deleteById(fileId);
}
```

### Service

```java
@Override
public Mono<FileInfo> uploadFile(FilePart filePart) {
    return DataBufferUtils.join(filePart.content())
        .map(dataBuffer -> {
            ObjectWriteResponse writeResponse = dfsRepository.uploadObject(filePart.filename(), dataBuffer.asInputStream());
            FileInfo fileInfo = new FileInfo();
            fileInfo.setOriginFileName(filePart.filename());
            fileInfo.setDfsFileName(writeResponse.object());
            fileInfo.setDfsBucket(writeResponse.bucket());
            fileInfo.setCreatedAt(new Date());
            return fileInfo;
        })
        .flatMap(fileInfo -> fileInfoRepository.insert(fileInfo))
        .onErrorStop();
}

@Override
public Mono<FileInfo> getFileById(String fileId) {
    return fileInfoRepository.findById(fileId);
}

@Override
public Mono<Void> deleteById(String fileId) {
    Mono<FileInfo> fileInfoMono = this.getFileById(fileId);
    Mono<FileInfo> fallback = Mono.error(new FileNotFoundException("No file was found with fileId: " + fileId));
    return fileInfoMono
        .switchIfEmpty(fallback)
        .flatMap(fileInfo -> {
            dfsRepository.deleteObject(fileInfo.getDfsFileName());
            return fileInfoRepository.deleteById(fileId);
        }).then();
}
```

### Exception Handler

```java
@Slf4j
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler(FileNotFoundException.class)
    @ResponseStatus(code = HttpStatus.NOT_FOUND)
    public ErrorInfo handleFileNotFoundException(FileNotFoundException e) {
        log.error("FileNotFoundException occurred", e);
        return new ErrorInfo("not_found", e.getMessage());
    }

    @ExceptionHandler(DfsServerException.class)
    @ResponseStatus(code = HttpStatus.INTERNAL_SERVER_ERROR)
    public ErrorInfo handleDfsServerException(DfsServerException e) {
        log.error("DfsServerException occurred", e);
        return new ErrorInfo("server_error", e.getMessage());
    }
}
```

## AWS SDK chunkUpload

```java
        // 初始化一个分片上传，获取分块上传ID，桶名 + 对像名 + 分块上传ID 唯一确定一个分块上传
        InitiateMultipartUploadRequest initRequest = new InitiateMultipartUploadRequest(bucket, fileName);
        InitiateMultipartUploadResult initResult = this.amazonS3.initiateMultipartUpload(initRequest);
        String uploadId = initResult.getUploadId();

        List<PartETag> eTags = new ArrayList<>();

        // 分片上传
        try (FileInputStream is = new FileInputStream(file)) {

            long contentLength = FileUtil.size(file);
            long filePosition = 0, partSize = BUFFER_SIZE;
            for (int i = 0; filePosition < contentLength; i++) {
                // Last part can be less than 5 MB. Adjust part size.
                partSize = Math.min(partSize, (contentLength - filePosition));
                // Create request to upload a part.
                UploadPartRequest uploadRequest = new UploadPartRequest()
                        .withBucketName(bucket)
                        .withKey(fileName)
                        .withUploadId(uploadId)
                        .withPartNumber(i)
                        .withFileOffset(filePosition)
                        .withPartSize(partSize)
                        .withInputStream(is);
                //如果是加密的，需要加入这步
                if (filePosition + partSize == contentLength) {
                    uploadRequest.setLastPart(true);
                }
                // Upload part and add response to our list.
                eTags.add(this.amazonS3.uploadPart(uploadRequest).getPartETag());

                filePosition += partSize;
            }
        } catch (Exception ignored) {

        }
        // 校验上传结果
        int nextMarker = 0;
        while (true) {
            ListPartsRequest listPartsRequest = new ListPartsRequest(bucket, fileName, uploadId);
            listPartsRequest.setPartNumberMarker(nextMarker);

            PartListing partList = this.amazonS3.listParts(listPartsRequest);

            for (PartSummary ps : partList.getParts()) {
                nextMarker++;
                eTags.add(new PartETag(ps.getPartNumber(), ps.getETag()));
            }

            if (!partList.isTruncated()) {
                break;
            }
        }
        // 完成分片上传
        CompleteMultipartUploadRequest completeRequest = new CompleteMultipartUploadRequest(bucket, fileName, uploadId, eTags);
        this.amazonS3.completeMultipartUpload(completeRequest);
```

## 获取 ip

```java
public abstract class IpUtils {

    public static String getClientIpAddress(HttpServletRequest request) {
        // 获取请求主机IP地址,如果通过代理进来，则透过防火墙获取真实IP地址
        String headerName = "x-forwarded-for";
        String ip = request.getHeader(headerName);
        if (null != ip && ip.length() != 0 && !"unknown".equalsIgnoreCase(ip)) {
            // 多次反向代理后会有多个IP值，第一个IP才是真实IP,它们按照英文逗号','分割
            if (ip.contains(",")) {
                ip = ip.split(",")[0];
            }
        }
        if (checkIp(ip)) {
            headerName = "Proxy-Client-IP";
            ip = request.getHeader(headerName);
        }
        if (checkIp(ip)) {
            headerName = "WL-Proxy-Client-IP";
            ip = request.getHeader(headerName);
        }
        if (checkIp(ip)) {
            headerName = "HTTP_CLIENT_IP";
            ip = request.getHeader(headerName);
        }
        if (checkIp(ip)) {
            headerName = "HTTP_X_FORWARDED_FOR";
            ip = request.getHeader(headerName);
        }
        if (checkIp(ip)) {
            headerName = "X-Real-IP";
            ip = request.getHeader(headerName);
        }
        if (checkIp(ip)) {
            headerName = "remote addr";
            ip = request.getRemoteAddr();
            // 127.0.0.1 ipv4, 0:0:0:0:0:0:0:1 ipv6
            if ("127.0.0.1".equals(ip) || "0:0:0:0:0:0:0:1".equals(ip)) {
                //根据网卡取本机配置的IP
                InetAddress inet = null;
                try {
                    inet = InetAddress.getLocalHost();
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                }
                if (Objects.isNull(inet)) {
                    return null;
                }
                ip = inet.getHostAddress();
            }
        }
        return ip;
    }

    private static boolean checkIp(String ip) {
        return null == ip || ip.length() == 0 || "unknown".equalsIgnoreCase(ip);
    }
}
```

## Spring 异步配置

```java
@EnableAsync
@Configuration
public class AsyncConfig {

    @Bean
    @Qualifier("asyncExecutor")
    public Executor executor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(Runtime.getRuntime().availableProcessors());
        executor.setMaxPoolSize(Runtime.getRuntime().availableProcessors() << 1);
        executor.setQueueCapacity(Integer.MAX_VALUE);
        executor.setKeepAliveSeconds(60);
        executor.setThreadNamePrefix("export-");
        executor.setTaskDecorator(r -> {
            // 获取当前上下文
            RequestAttributes requestAttributes = RequestContextHolder.getRequestAttributes();
            return () ->{
                try {
                    // 传递到异步线程
                    RequestContextHolder.setRequestAttributes(requestAttributes);
                    r.run();
                } finally {
                    // 用完销毁
                    RequestContextHolder.resetRequestAttributes();
                }
            };
        });

        executor.setRejectedExecutionHandler(new ThreadPoolExecutor.AbortPolicy());
        return executor;
    }
}
```

## `logback.xml` 配置文件简单解析

1. appender 对应一种日志文件类型，包括路径，rollingPolicy
2. Logger 可以适配多种 appender
3. 默认会初始化一个 ROOT 的 Logger，是所有子 Logger 的父类，LoggerContext 传递信息
4. 可以通过实现 Encoder 调整日志的打印规则

```java
// 1. 首先会查询是否有已经存在的logger，否则创建新的logger，继承于ROOT
// 2. logger可以在logback-spring.xml中自定义
Logger logger = LoggerFactory.getLogger("TEST_LOGGER");
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <!--
       说明：
       1、日志级别及文件
           日志记录采用分级记录，级别与日志文件名相对应，不同级别的日志信息记录到不同的日志文件中
           例如：error级别记录到log_error_xxx.log或log_error.log（该文件为当前记录的日志文件），而log_error_xxx.log为归档日志，
           日志文件按日期记录，同一天内，若日志文件大小等于或大于2M，则按0、1、2...顺序分别命名
           例如log-level-2013-12-21.0.log
           其它级别的日志也是如此。
       2、文件路径
           若开发、测试用，在Eclipse中运行项目，则到Eclipse的安装路径查找logs文件夹，以相对路径../logs。
           若部署到Tomcat下，则在Tomcat下的logs文件中
       3、Appender
           FILEERROR对应error级别，文件名以log-error-xxx.log形式命名
           FILEWARN对应warn级别，文件名以log-warn-xxx.log形式命名
           FILEINFO对应info级别，文件名以log-info-xxx.log形式命名
           FILEDEBUG对应debug级别，文件名以log-debug-xxx.log形式命名
           stdout将日志信息输出到控制上，为方便开发测试使用
    -->
    <contextName>SpringBootDemo</contextName>
    <property name="LOG_PATH" value="D:\\JavaWebLogs" />
    <!--设置系统日志目录-->
    <property name="APPDIR" value="SpringBootDemo" />

    <!-- 日志记录器，日期滚动记录 -->
    <appender name="FILEERROR" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <!-- 正在记录的日志文件的路径及文件名 -->
        <file>${LOG_PATH}/${APPDIR}/log_error.log</file>
        <!-- 日志记录器的滚动策略，按日期，按大小记录 -->
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!-- 归档的日志文件的路径，例如今天是2013-12-21日志，当前写的日志文件路径为file节点指定，可以将此文件与file指定文件路径设置为不同路径，从而将当前日志文件或归档日志文件置不同的目录。
            而2013-12-21的日志文件在由fileNamePattern指定。%d{yyyy-MM-dd}指定日期格式，%i指定索引 -->
            <fileNamePattern>${LOG_PATH}/${APPDIR}/error/log-error-%d{yyyy-MM-dd}.%i.log</fileNamePattern>
            <!-- 除按日志记录之外，还配置了日志文件不能超过2M，若超过2M，日志文件会以索引0开始，
            命名日志文件，例如log-error-2013-12-21.0.log -->
            <timeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                <maxFileSize>2MB</maxFileSize>
            </timeBasedFileNamingAndTriggeringPolicy>
        </rollingPolicy>
        <!-- 追加方式记录日志 -->
        <append>true</append>
        <!-- 日志文件的格式 -->
        <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <pattern>===%d{yyyy-MM-dd HH:mm:ss.SSS} %-5level %logger Line:%-3L - %msg%n</pattern>
            <charset>utf-8</charset>
        </encoder>
        <!-- 此日志文件只记录info级别的 -->
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>error</level>
            <onMatch>ACCEPT</onMatch>
            <onMismatch>DENY</onMismatch>
        </filter>
    </appender>

    <!-- 日志记录器，日期滚动记录 -->
    <appender name="FILEWARN" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <!-- 正在记录的日志文件的路径及文件名 -->
        <file>${LOG_PATH}/${APPDIR}/log_warn.log</file>
        <!-- 日志记录器的滚动策略，按日期，按大小记录 -->
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!-- 归档的日志文件的路径，例如今天是2013-12-21日志，当前写的日志文件路径为file节点指定，可以将此文件与file指定文件路径设置为不同路径，从而将当前日志文件或归档日志文件置不同的目录。
            而2013-12-21的日志文件在由fileNamePattern指定。%d{yyyy-MM-dd}指定日期格式，%i指定索引 -->
            <fileNamePattern>${LOG_PATH}/${APPDIR}/warn/log-warn-%d{yyyy-MM-dd}.%i.log</fileNamePattern>
            <!-- 除按日志记录之外，还配置了日志文件不能超过2M，若超过2M，日志文件会以索引0开始，
            命名日志文件，例如log-error-2013-12-21.0.log -->
            <timeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                <maxFileSize>2MB</maxFileSize>
            </timeBasedFileNamingAndTriggeringPolicy>
        </rollingPolicy>
        <!-- 追加方式记录日志 -->
        <append>true</append>
        <!-- 日志文件的格式 -->
        <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <pattern>===%d{yyyy-MM-dd HH:mm:ss.SSS} %-5level %logger Line:%-3L - %msg%n</pattern>
            <charset>utf-8</charset>
        </encoder>
        <!-- 此日志文件只记录info级别的 -->
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>warn</level>
            <onMatch>ACCEPT</onMatch>
            <onMismatch>DENY</onMismatch>
        </filter>
    </appender>

    <!-- 日志记录器，日期滚动记录 -->
    <appender name="FILEINFO" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <!-- 正在记录的日志文件的路径及文件名 -->
        <file>${LOG_PATH}/${APPDIR}/log_info.log</file>
        <!-- 日志记录器的滚动策略，按日期，按大小记录 -->
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!-- 归档的日志文件的路径，例如今天是2013-12-21日志，当前写的日志文件路径为file节点指定，可以将此文件与file指定文件路径设置为不同路径，从而将当前日志文件或归档日志文件置不同的目录。
            而2013-12-21的日志文件在由fileNamePattern指定。%d{yyyy-MM-dd}指定日期格式，%i指定索引 -->
            <fileNamePattern>${LOG_PATH}/${APPDIR}/info/log-info-%d{yyyy-MM-dd}.%i.log</fileNamePattern>
            <!-- 除按日志记录之外，还配置了日志文件不能超过2M，若超过2M，日志文件会以索引0开始，
            命名日志文件，例如log-error-2013-12-21.0.log -->
            <timeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                <maxFileSize>2MB</maxFileSize>
            </timeBasedFileNamingAndTriggeringPolicy>
        </rollingPolicy>
        <!-- 追加方式记录日志 -->
        <append>true</append>
        <!-- 日志文件的格式 -->
        <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <pattern>===%d{yyyy-MM-dd HH:mm:ss.SSS} %-5level %logger Line:%-3L - %msg%n</pattern>
            <charset>utf-8</charset>
        </encoder>
        <!-- 此日志文件只记录info级别的 -->
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>info</level>
            <onMatch>ACCEPT</onMatch>
            <onMismatch>DENY</onMismatch>
        </filter>
    </appender>

    <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
        <!--encoder 默认配置为PatternLayoutEncoder-->
        <encoder>
            <pattern>===%d{yyyy-MM-dd HH:mm:ss.SSS} %-5level %logger Line:%-3L - %msg%n</pattern>
            <charset>utf-8</charset>
        </encoder>
        <!--此日志appender是为开发使用，只配置最底级别，控制台输出的日志级别是大于或等于此级别的日志信息-->
        <filter class="ch.qos.logback.classic.filter.ThresholdFilter">
            <level>debug</level>
        </filter>
    </appender>

    <logger name="org.springframework" level="WARN" />
    <logger name="org.hibernate" level="WARN" />

    <!-- 生产环境下，将此级别配置为适合的级别，以免日志文件太多或影响程序性能 -->
    <!--  root => <logger name="root"> -->
    <root level="INFO">
        <appender-ref ref="FILEERROR" />
        <appender-ref ref="FILEWARN" />
        <appender-ref ref="FILEINFO" />
        <!-- 生产环境将请stdout,testfile去掉 -->
        <appender-ref ref="STDOUT" />
    </root>

    <logger name="TEST_LOGGER" level="TRACE">
      <appender-ref ref="STDOUT">
    </logger>
</configuration>
```

## 配置多环境 logback

```properties
# along with profile
logging.config=classpath:logback-${spring.profiles.active}.xml

# define Property in profile config
LOG_HOME = /usr/local/app/logs
FILE_SUFFIX = %d{yyyyMMdd}
APP_NAME = test-logback
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration debug="false">

    <!--定义日志文件的存储地址 勿在 LogBack 的配置中使用相对路径-->
    <property name="appName" value="wumanshu"/>
    <property name="LOG_HOME" value="/home/wumanshu"/>
    <Property name="cdr-directory-send" value="/home/wumanshu/REPORT/send"/>
    <Property name="file-suffix" value="%d{yyyyMMdd}"/>
    <Property name="pv-type" value="5L"/>
    <!-- 配置本机的IP地址，去掉.，每段不足三位的，前端补零 -->
    <Property name="ipStr" value="192168007921"/>
    <!-- 应用端口号，不足五位的前端补零凑够五位 -->
    <Property name="portStr" value="08181"/>

    <!--格式化输出：%d表示日期，%thread表示线程名，%-5level：级别从左显示5个字符宽度%msg：日志消息，%n是换行符-->
    <property name="LOG_PATTERN" value="%-12(%d{yyyy-MM-dd HH:mm:ss.SSS}) |-%-5level [%thread] %c [%L] -| %msg%n"/>

    <!--控制台日志， 控制台输出 -->
    <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>DEBUG</level>
        </filter>
        <encoder>
            <pattern>${LOG_PATTERN}</pattern>
            <charset>utf-8</charset>
        </encoder>
    </appender>

    <!--文件日志， 按照每天生成日志文件 -->
    <appender name="ERROR" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!--日志文件输出的文件名-->
            <FileNamePattern>${LOG_HOME}/logs/error-${file-suffix}.log</FileNamePattern>
            <!--日志文件保留天数-->
            <MaxHistory>30</MaxHistory>
        </rollingPolicy>
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>ERROR</level>
        </filter>
        <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <pattern>${LOG_PATTERN}</pattern>
            <charset>utf-8</charset>
        </encoder>
        <!--日志文件最大的大小-->
        <triggeringPolicy class="ch.qos.logback.core.rolling.SizeBasedTriggeringPolicy">
            <MaxFileSize>10MB</MaxFileSize>
        </triggeringPolicy>
    </appender>

    <!--话单 当文件大小达到10M时，新建文件输出话单 -->
    <appender name="CDR" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
            <!--日志文件输出的文件名-->
            <FileNamePattern>${cdr-directory-send}/${appName}${ipStr}${portStr}${pv-type}%d{yyyyMMddHHmm}.txt
            </FileNamePattern>
            <!--日志文件保留天数-->
            <MaxHistory>30</MaxHistory>
        </rollingPolicy>
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
            <level>ERROR</level>
            <onMatch>ACCEPT</onMatch>
            <onMismatch>DENY</onMismatch>
        </filter>
        <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <!--格式化输出：%d表示日期，%thread表示线程名，%-5level：级别从左显示5个字符宽度%msg：日志消息，%n是换行符-->
            <pattern>%m%n</pattern>
        </encoder>
        <!--日志文件最大的大小-->
        <triggeringPolicy class="ch.qos.logback.core.rolling.SizeBasedTriggeringPolicy">
            <MaxFileSize>10MB</MaxFileSize>
        </triggeringPolicy>
    </appender>

    <root level="DEBUG">
        <appender-ref ref="STDOUT"/>
    </root>

    <!--error日志-->
    <logger name="com.wlf.translateprovider" level="INFO" additivity="false">
        <appender-ref ref="ERROR"/>
        <appender-ref ref="STDOUT"/>
    </logger>

    <!--话单-->
    <logger name="com.wlf.translateprovider.cdr" level="ERROR"
            additivity="false">
        <appender-ref ref="CDR"/>
    </logger>
</configuration>
```

## Spring Retry

```java
@EnableRetry
@Configuration
public class RetryConfig {

    public static final AtomicInteger ATOMIC_INTEGER = new AtomicInteger();

    public static void main(String[] args) throws InterruptedException {
        AnnotationConfigApplicationContext applicationContext = new AnnotationConfigApplicationContext();
        applicationContext.register(RetryConfig.class);

        applicationContext.refresh();

        RetryConfig rc = applicationContext.getBean(RetryConfig.class);
        rc.haha();

        applicationContext.wait(3000L);
        applicationContext.close();
    }

    @Retryable(value = RuntimeException.class)
    protected void haha() {
        System.out.println("haha " + ATOMIC_INTEGER.addAndGet(1));
        throw new RuntimeException("我错了");
    }

    @Recover
    protected void recover(RuntimeException e) {
        System.out.println("recovered, ex: " + e.getMessage());
    }
}
```

output:

```java
19:50:22.345 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry: count=0
haha 1
19:50:23.350 [main] DEBUG org.springframework.retry.support.RetryTemplate - Checking for rethrow: count=1
19:50:23.350 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry: count=1
haha 2
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Checking for rethrow: count=2
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry: count=2
haha 3
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Checking for rethrow: count=3
19:50:24.360 [main] DEBUG org.springframework.retry.support.RetryTemplate - Retry failed last attempt: count=3
recovered, ex: 我错了
Exception in thread "main" java.lang.IllegalMonitorStateException
    at java.lang.Object.wait(Native Method)
    at com.iflytek.scv.logging.config.RetryConfig.main(RetryConfig.java:32)
```

## WebFlux Fire and Forget Mechanism

**If your fire-and-forget is already async returning Mono/Flux**:

```java
public Flux<Data> search(SearchRequest request)
{
    return searchService.search(request)
                        .collectList()
                        .doOnNext(data -> doThisAsync(data).subscribe())  // add error logging here or inside doThisAsync
                        .flatMapMany(Flux::fromIterable);
}

public Mono<Void> doThisAsync(List<Data> data) {
    //do some async/non-blocking processing here like calling WebClient
}
```

**If your fire-and-forget does blocking I/O**:

```java
public Flux<Data> search(SearchRequest request)
{
    return searchService.search(request)
                        .collectList()
                        .doOnNext(data -> Mono.fromRunnable(() -> doThisAsync(data))
                                              .subscribeOn(Schedulers.elastic())  // delegate to proper thread to not block main flow
                                              .subscribe())  // add error logging here or inside doThisAsync
                        .flatMapMany(Flux::fromIterable);
}

public void doThisAsync(List<Data> data) {
    //do some blocking I/O on calling thread
}
```

## Spring Cloud with Spring Alibaba

2021.0.0 版本的 SpringCloud + Spring Alibaba 2.2.7.RELEASE 版本不能很好的协调好。【alibaba 的 2.2.7 版本默认不使用 nacos 的 HTTP 端口，导致配置上很古怪，且没有相应的说明文档】

### 缺少 LoadBalancer Bean

自己实现或引入相关依赖

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-gateway</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-loadbalancer</artifactId>
</dependency>
```

### 默认不读取 bootstap 文件

引入依赖

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-bootstrap</artifactId>
</dependency>
```
