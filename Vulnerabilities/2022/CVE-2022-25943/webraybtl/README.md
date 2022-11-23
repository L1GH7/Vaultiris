# CVE-2022-24934


## 漏洞概述

WPS Office安装时，会以管理员权限安装服务程序wpscloudsvr，该服务程序配置为手动启动，但错误配置了该服务程序所在文件夹的访问控制列表（ACL）属性，普通Users组成员仍然对该文件夹有读写权限。

## 影响范围

WPS Office版本小于11.2.0.10258均存在该漏洞。

（WPS版本号看最后一组，最后一组数字大即为新版本，11.1表示个人版，11.2表示企业版，11.6和11.8表示政企定制版）

## 利用场景

普通Users组成员在服务程序wpscloudsvr所在文件夹放置被劫持的DLL文件，以普通用户权限启动该服务，最终DLL被服务程序加载执行，实现本地提权。
（wpscloudsvr服务程序自身并不存在DLL劫持漏洞，可使用系统DLL劫持漏洞，测试时win7 6.1.7601 sp1 32位系统存在可被利用的系统DLL劫持漏洞，win10 10.0.18363.592 64位不存在可被利用的DLL劫持漏洞）

## 复现环境

操作系统：Win7 sp1。

WPS Office版本：WPS Office 11.2.0.10200。

复现工具：Process Explorer，Process Monitor，CFF Explorer，vs2008。

分析工具：Detect It Easy，OD，IDA，Openssl，数字签名复制工具sigthief.py。

## 漏洞详情&技术咨询

![图片](http://raqgyz5ky.hn-bkt.clouddn.com/111111.jpeg)
