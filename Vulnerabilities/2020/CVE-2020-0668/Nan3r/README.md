# CVE-2020-0668

powershell 实现，利用[printconfig.dll](https://github.com/Nan3r/writedacl2system/tree/master/printconfig.dll)来提权

[evil dll](https://github.com/Nan3r/dll/tree/master/common%20dll/run_programdata_setup.bat)可以自己定义，我这里的dll会执行c:\programdata\setup.bat文件


## Reference:

https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/CVE-2020-0668

https://itm4n.github.io/cve-2020-0668-windows-service-tracing-eop



## TODO：

​	1.利用RASPLAP服务提权  https://github.com/NotGlop/SysExec/blob/master/source/SysExec.cpp 

​	2.利用RASMAN服务提权
