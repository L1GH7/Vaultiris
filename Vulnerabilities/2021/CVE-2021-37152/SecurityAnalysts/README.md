# CVE-2021-37152

Exploit Accsess network clients by sending packets in wirless TP-LINK and preparing for a mitm attack

# exploit

This exploit is designed for accessing and scanning network clients
and you will easily access and prepare for a mitm attack,
the exploit will attack in layers three and two, you will learn more
in the following, this exploit will be performed on most devices The range
in the tested models is as follows and attack options 
TP-LINK 54Mbps Wirless ADSL2+ Modem Router TD-W8901G power 9V == 0.85A ,
TD-W9960-v1.20 , TP-LINK TL-WR840N300Mbps New Design Wireless N Router
Exploit works on most devices tested on these models

# Divaces Test

- TP-LINK
- TP-LINK 54Mbps Wirless ADSL2+ Modem Router TD-W8901G power 9V == 0.85A 
- TD-W9960-v1.20
- TP-LINK TL-WR840N300Mbps New Design Wireless N Router
- 
# Install

*Git clone exploit :*
```
  git clone https://github.com/lhashashinl/CVE-2021-37152.git
 ```
 *Edit as code import class*
 ```
  exploit(target="").pyload(message="" , DefaultGetway="" , interface="" , count=None , countLeyer2=)
 ```
 *Run :*
 ```
  sudo python3 CVE-2021-37152.py
 ```
