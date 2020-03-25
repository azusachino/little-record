# NETTY

## 初识netty

- Netty是一个提供了易于使用的API的客户端/服务端
- 并发高 - NIO (None Blocking IO)
- 传输快 - 零拷贝

### Hello Netty 服务器

- 构建一对主从线程组
- 定义服务器启动类
- 为服务器设置Channel
- 设置处理从线程池的助手类初始化器
- 监听启动和关闭服务器

#### `HelloServer`

```java
public static void main(String[] args) {
        // Master EventGroup
        EventLoopGroup masterGroup = new NioEventLoopGroup();
        // Slave EventGroup
        EventLoopGroup slaveGroup = new NioEventLoopGroup();
        try {
            ServerBootstrap bootStrap = new ServerBootstrap();
            bootStrap.group(masterGroup, slaveGroup)
                    .channel(NioServerSocketChannel.class)
                    .childHandler(new HelloServerInitializer());

            ChannelFuture cf = bootStrap.bind(8088).sync();
            cf.channel().closeFuture().sync();
        } catch (Exception e) {
            log.error(e.getMessage(), e);
        } finally {
            masterGroup.shutdownGracefully();
            slaveGroup.shutdownGracefully();
        }
    }
```

#### `HelloServerInitializer`

```java
public class HelloServerInitializer extends ChannelInitializer<SocketChannel> {

    @Override
    protected void initChannel(SocketChannel sc) {
        // acquire pipeline from sc
        ChannelPipeline cp = sc.pipeline();

        cp.addLast(HttpServerCodec.class.getName(), new HttpServerCodec());

        cp.addLast(HelloChannelHandler.class.getName(), new HelloChannelHandler());

    }
}
```

#### `HelloChannelHandler`

```java
public class HelloChannelHandler extends SimpleChannelInboundHandler<HttpObject> {

    @Override
    protected void channelRead0(ChannelHandlerContext ctx, HttpObject ho) {
        Channel channel = ctx.channel();

        System.out.println(channel.remoteAddress());

        ByteBuf content = Unpooled.copiedBuffer("Hello Netty", CharsetUtil.UTF_8);

        // create a http response
        FullHttpResponse res = new DefaultFullHttpResponse(HttpVersion.HTTP_1_1, HttpResponseStatus.valueOf(200), content);

        res.headers().set(HttpHeaderNames.CONTENT_TYPE, "text/plain")
                .set(HttpHeaderNames.CONTENT_LENGTH, content.readableBytes());

        ctx.writeAndFlush(res);
    }
}
```

### Netty 生命周期

- registered
- unregistered
- active
- inactive
- read
- readComplete
- eventTriggered
- WritablitityChanged
- exceptionCaught
