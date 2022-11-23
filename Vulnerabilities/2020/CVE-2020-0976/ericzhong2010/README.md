# GUI Check CVE-2020-0796

#### 勘误：
正确的CVE名称是CVE-2020-0796，而不是CVE-2020-0976。此程序不改了，知道就好的。

#### Information

```
Microsoft SMBv3 contains a vulnerability in the handling of compression, which may allow a remote,unauthenticated attacker to execute arbitrary code on a vulnerable system.Microsoft Server Message Block 3.1.1 (SMBv3) contains a vulnerability in the way that it handles connections that use compression.

This vulnerability may allow a remote, unauthenticated attacker to execute arbitrary code on a vulnerable system.It has been reported that this vulnerability is "wormable."

By connecting to a vulnerable Windows machine using SMBv3, or by causing a vulnerable Windows system to initiate a client connection to a SMBv3 server,a remote, unauthenticated attacker to execute arbitrary code with SYSTEM privileges on a vulnerable system.
```



#### Infected system

```
  + Windows 10 Version 1903 for 32-bit Systems
  + Windows 10 Version 1903 for ARM64-based Systems
  + Windows 10 Version 1903 for x64-based Systems
  + Windows 10 Version 1909 for 32-bit Systems
  + Windows 10 Version 1909 for ARM64-based Systems
  + Windows 10 Version 1909 for x64-based Systems
  + Windows Server, version 1903 (Server Core installation)
  + Windows Server, version 1909 (Server Core installation)
```



#### Snapshots

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314155534618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zODYyMzk5NA==,size_16,color_FFFFFF,t_70)



#### Workaround

1. CVE-2020-0796 Patch
   https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/CVE-2020-0796
   
2. Disable SMBv3 compression

   ```
   Set-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Services\LanmanServer\Parameters" DisableCompression -Type DWORD -Value 1 -Force
   ```

   
