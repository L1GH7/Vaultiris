# Capricornus

Capricornus（摩羯座）一款基于wxpython的GUI图形化检测工具，包含了基础的备忘录，base64加解密，批量漏洞和单项漏洞检测功能。

![image-20221016095616608](https://qwtd-image.oss-cn-hangzhou.aliyuncs.com/img/image-20221016095616608.png)

# 目前支持检测下列漏洞

```
'Atlassian Bitbucket Server远程命令执行CVE-2022-36804',
'Spring4shell远程代码执行CVE-2022-22965',
'Omnia MPX文件读取漏洞CVE-2022-36642',
'M7S任意文件读取',
'Metabase 任意文件读取CVE-2021-41277',
'Teleport堡垒机do-login任意用户登录漏洞',
'Hikvision身份认证绕过',
'孚盟云AjaxMethod.ashx SQL注入',
'Alibaba Nacos 未授权访问漏洞',
'安恒明御WEB应用防火墙report.php任意用户登录',
'大华城市安防监控系统平台管理任意文件下载漏洞',
'迪普VPN任意文件读取漏洞',
'Weblogic CVE-2020-14882',
'泛微E-Cology HrmCareerApplyPerView.jsp SQL注入',
'VMware vCenter vid 任意文件读取漏洞',
'Spring未授权访问漏洞',
'GLPI htmLawedTest.php远程命令执行漏洞CVE-2022-35914'
```

# 安装

```
python3 pip install -r requirements.txt
python3 Capricornus1.2.py
```

![image-20221016095955541](https://qwtd-image.oss-cn-hangzhou.aliyuncs.com/img/image-20221016095955541.png)

# 二次开发

可以修改Capricornus.py文件进行二开，通过增加vul_list列表框、漏洞class类，以及调用模块进行增加漏洞

![image-20221016100216266](https://qwtd-image.oss-cn-hangzhou.aliyuncs.com/img/image-20221016100216266.png)

![image-20221016100113891](https://qwtd-image.oss-cn-hangzhou.aliyuncs.com/img/image-20221016100113891.png)

# 联系方式

如果有问题或者bug可以通过VX qiwentaid1加我联系