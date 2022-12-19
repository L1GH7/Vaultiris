## CVE-2020-0014 Toast 复现
该漏洞可使恶意 App 通过构造一个可被点击的 Toast 视图来截获用户在屏幕上的操作，以达到搜集用户密码等敏感信息的目的。  
影响范围：Android 8.0版本，8.1版本，9版本，10版本  


![log](./log.png)

只验证了可行性，未验证漏洞实际可以造成的危害  
[release](./app/release/app-release.apk)  

## LINKS

[通过安卓最新 Toast 漏洞进行 Tapjacking](https://www.anquanke.com/post/id/201052)  
[Android 安全公告](https://source.android.com/security/bulletin/2020-02-01)  
[CVE-2020-0014](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-0014)  