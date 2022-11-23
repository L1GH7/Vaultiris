# SecDrvPoolLeak - A PoC for CVE-2018-7250

### Description
An issue was discovered in secdrv.sys as shipped in Microsoft Windows Vista, Windows 7, Windows 8, and Windows 8.1 before KB3086255, and as shipped in Macrovision SafeDisc. An uninitialized kernel pool allocation in IOCTL 0xCA002813 allows a local unprivileged attacker to leak 16 bits of uninitialized kernel PagedPool data.

The vulnerability was reported to Microsoft, and since it does not affect an up-to-date Windows machine (only versions prior to KB3086255), they will not take any action. Was tested and exploited successfully on Windows 7 x86.

Also related to [CVE-2018-7249](https://github.com/Elvin9/NotSecDrv), the link contains details about both vulnerabilities.

### Screenshots
The allocated PagedPool chunk uninitialized:
![Alt text](https://github.com/Elvin9/SecDrvPoolLeak/raw/master/allocation.png)

The uninitialized part copied to usermode:
![Alt text](https://github.com/Elvin9/SecDrvPoolLeak/raw/master/leak.png)

### Test Enviroment
**OS:** Windows 7 Kernel Version 7600 MP (1 procs) Free x86 compatible Built by: 7600.16385.x86fre.win7_rtm.090713-1255
**VM:** 4GB RAM, 1 CPU
**Hardware:** Windows 10 Pro 64 bit, Motherboard Gigabyte Z370 HD3, 16GB RAM, Intel i5-8400 2.80GHz (6 CPUs)
