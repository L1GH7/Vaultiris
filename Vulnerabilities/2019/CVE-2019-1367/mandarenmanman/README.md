0x00背景介绍

Internet Explorer，是微软公司推出的一款网页浏览器。用户量极大。

9月23日微软紧急发布安全更新，修复了一个影响IE浏览器的远程代码执行漏洞。由谷歌威胁分析小组发现此漏洞，据称该漏洞已遭在野利用。



0x01漏洞描述


此漏洞是由InternetExplorer脚本引擎中处理内存对象的方式中的内存损坏引起的。要利用此漏洞，攻击者必须引导用户打开已经托管漏洞的恶意网站。

利用此漏洞可以导致攻击者获得用户的当前权限，并执行任意代码。如果当前用户拥有管理权限攻击者可以在系统上执行各种操作，从创建具有完全权限的新帐户到安装程序甚至修改数据。


0x02漏洞编号


CVE-2019-1367




0x02受影响版本

InternetExplorer 9

InternetExplorer 10

InternetExplorer11



0x03修复建议


1.安装补丁：

修复影响 InternetExplorer 11 Windows 101903 版本的安全更新可查看链接

https://support.microsoft.com/help/4522016

 

2.缓解措施：

对于32位系统，在管理员命令提示符中输入如下命令：

 takeown /f %windir%\system32\jscript.dll

 cacls%windir%\system32\jscript.dll /E /P everyone:N

 对于64位系统，在管理员命令提示符中输入如下命令：

 takeown /f%windir%\syswow64\jscript.dll

 cacls%windir%\syswow64\jscript.dll /E /P everyone:N

 takeown /f%windir%\system32\jscript.dll

 cacls %windir%\system32\jscript.dll /E /Peveryone:N
