# CVE-2021-31166

## 0x00.Description
This is a proof of concept for CVE-2021-31166 ("HTTP Protocol Stack Remote Code Execution Vulnerability"), a use-after-free dereference in http.sys patched by Microsoft in May 2021.

*As far as I know, it can only trigger the program to crash, please use it with caution.*

## 0x01.Impact

Windows Server, version 20H2 (Server Core Installation)

Windows 10 Version 20H2 for ARM64-based Systems

Windows 10 Version 20H2 for 32-bit Systems

Windows 10 Version 20H2 for x64-based Systems

Windows Server, version 2004 (Server Core installation)

Windows 10 Version 2004 for x64-based Systems

Windows 10 Version 2004 for ARM64-based Systems

Windows 10 Version 2004 for 32-bit Systems

## 0x02.Reference

[HTTP Protocol Stack Remote Code Execution Vulnerability](https://msrc.microsoft.com/update-guide/vulnerability/CVE-2021-31166)
