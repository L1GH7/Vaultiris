# monstra_cms-3.0.4--getshell
monstra_cms-3.0.4-上传getshell CVE- 2018-17418

代码分析（Code analysis）：

在monstra\plugins\box\filesmanager\ filesmanager.admin.php第150行中存在forbidden_types变量做黑名单限制，继续跟进该变量

In the line 150 of monstra\plugins\box\filesmanager\ filesmanager.admin.php, there is a forbidden_types variable to be blacklisted. Continue to follow the variable.

![Alt text](5.png) 

在同文件第22行发现相关黑名单名单，可以利用大小写绕过。

The list of related blacklists found on line 22 of the same document can be bypassed by capitalization.

 ![Alt text](6.png) 

实际演示（Actual demonstration）：

Content栏下Files功能存在上传按钮

The Upload function exists in the Files function under the Content column.

![Alt text](1.png) 

使用burp拦截数据包，修改后缀为PhP

Use burp to intercept the packet and modify the suffix to PhP.

![Alt text](2.png) 

上传成功

Successful upload

![Alt text](3.png) 

菜刀链接

use Chopper link it

![Alt text](4.png) 
