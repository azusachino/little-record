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

@Transactional(timeout=30) //默认是 30 秒

```java
1、@Transactional 只能被应用到public方法上, 对于其它非public的方法,如果标记了@Transactional也不会报错,但方法没有事务功能.

2、用 spring 事务管理器,由spring来负责数据库的打开,提交,回滚.默认遇到运行期例外(throw new RuntimeException("注释");)会回滚，即遇到不受检查（unchecked）的例外时回滚；而遇到需要捕获的例外(throw new Exception("注释");)不会回滚,即遇到受检查的例外（就是非运行时抛出的异常，编译器会检查到的异常叫受检查例外或说受检查异常）时，需我们指定方式来让事务回滚要想所有异常都回滚,要加上 @Transactional( rollbackFor={Exception.class,其它异常}) .如果让unchecked例外不回滚： @Transactional(notRollbackFor=RunTimeException.class)

3、@Transactional 注解应该只被应用到 public 可见度的方法上。 如果你在 protected、private 或者 package-visible 的方法上使用 @Transactional 注解，它也不会报错， 但是这个被注解的方法将不会展示已配置的事务设置。

4、@Transactional 注解可以被应用于接口定义和接口方法、类定义和类的 public 方法上。然而，请注意仅仅 @Transactional 注解的出现不足于开启事务行为，它仅仅 是一种元数据，能够被可以识别 @Transactional 注解和上述的配置适当的具有事务行为的beans所使用。上面的例子中，其实正是 元素的出现 开启 了事务行为。

5、Spring团队的建议是你在具体的类（或类的方法）上使用 @Transactional 注解，而不要使用在类所要实现的任何接口上。你当然可以在接口上使用 @Transactional 注解，但是这将只能当你设置了基于接口的代理时它才生效。因为注解是不能继承的，这就意味着如果你正在使用基于类的代理时，那么事务的设置将不能被基于类的代理所识别，而且对象也将不会被事务代理所包装（将被确认为严重的）。因此，请接受Spring团队的建议并且在具体的类上使用 @Transactional 注解。
```

### 事务传播级别

```java
@Transactional(propagation=Propagation.REQUIRED) ：如果有事务, 那么加入事务, 没有的话新建一个(默认情况下)
@Transactional(propagation=Propagation.NOT_SUPPORTED) ：容器不为这个方法开启事务
@Transactional(propagation=Propagation.REQUIRES_NEW) ：不管是否存在事务,都创建一个新的事务,原来的挂起,新的执行完毕,继续执行老的事务
@Transactional(propagation=Propagation.MANDATORY) ：必须在一个已有的事务中执行,否则抛出异常
@Transactional(propagation=Propagation.NEVER) ：必须在一个没有的事务中执行,否则抛出异常(与Propagation.MANDATORY相反)
@Transactional(propagation=Propagation.SUPPORTS) ：如果其他bean调用这个方法,在其他bean中声明事务,那就用事务.如果其他bean没有声明事务,那就不用事务.
```

### 事务隔离级别

```java
@Transactional(isolation = Isolation.READ_UNCOMMITTED)：读取未提交数据(会出现脏读, 不可重复读) 基本不使用
@Transactional(isolation = Isolation.READ_COMMITTED)：读取已提交数据(会出现不可重复读和幻读)
@Transactional(isolation = Isolation.REPEATABLE_READ)：可重复读(会出现幻读)
@Transactional(isolation = Isolation.SERIALIZABLE)：串行化
```

## Feign

通过 FeignClient 调用接口时, GET 接口需要给入参标注@SpringQueryMap

## maven 源码

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
