<h1 align="center">CVE-2022-1077</h1>

<h2 align="center">Description</h2>

A vulnerability was found in TEM FLEX-1080 and FLEX-1085 1.6.0. It has been declared as problematic. This vulnerability log.cgi of the component Log Handler. A direct request leads to information disclosure of hardware information. The attack can be initiated remotely and does not require any form of authentication.

![](1.png)

<h2 align="center">Usage</h2>

```
$ git clone https://github.com/MrEmpy/CVE-2022-1077.git
$ cd CVE-2022-1077
$ chmod +x cve-2022-1077.py
$ ./cve-2022-1077.py -u http://<IP>
```

<h2 align="center">Reference</h2>

* https://vuldb.com/?id.194848
