## 复现过程
在IDEA中导入xstream组件的jar包，本次复现所使用的是CommonsCollections6链来进行利用，故导入commons-collections组件jar包,POC详见附件。
首先使用ysoserial启动一个恶意RMI服务端进行监听，并利用了CommonsCollections6链进行命令执行，如下所示：
```txt
java -cp .\ysoserial.jar ysoserial.exploit.JRMPListener 1099 CommonsCollections6  "calc"
```
![1.png](1.png)
同时根据该漏洞对Xstrean的反序列化流程进行了分析调试做了记录，详见`xstream反序列化流程分析.pdf`

