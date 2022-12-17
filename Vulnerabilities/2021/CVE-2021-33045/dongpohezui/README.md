# cve-2021-33045

通过修改浏览器发往```/RPC2_Login```的数据包登录摄像头的网页。

这里采用mitmproxy脚本实现相关功能，也可以通过Fiddler、burpsuite实现类似功能。

用法如下：

```
mitmweb  --showhost --set block_global=false -s mitm.py
```

然后，浏览器将mitmproxy设置为http代理。

参考链接：
 - Dahua Authentication Bypass ≈ Packet Storm
https://packetstormsecurity.com/files/164423/Dahua-Authentication-Bypass.html

 - mcw0/DahuaConsole
https://github.com/mcw0/DahuaConsole


