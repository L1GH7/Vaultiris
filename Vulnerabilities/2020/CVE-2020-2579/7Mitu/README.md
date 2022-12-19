# CVE-2020-25790
Typesetter CMS文件上传漏洞环境

## 拉取方式
``` shell
docker pull mtwf/typesetter_cve-2020-25790:5.1
docker run -it -d -p 80:80 mtwf/typesetter_cve-2020-25790:5.1 /bin/bash -c "service apache2 start;/bin/bash"
```
typesetter账号密码为`admin:admin`
## 漏洞复现过程

![复现过程](https://github.com/7Mitu/CVE-2020-25790/raw/main/23333.gif)
- https://github.com/Typesetter/Typesetter/issues/674
