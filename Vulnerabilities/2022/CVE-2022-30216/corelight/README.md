# CVE-2022-30216
A Zeek package which raises notices for attempts and exploits of CVE-2022-30216, a technique used against Windows Server to force an NTLM authorization to an arbitrary server. An attacker can reuse the NTLM token to generate a client certificate, enabling them to request a Kerberos ticket that accesses the domain controller.
  
  
## Installation

`$ zkg install cve-2022-30216`

Use against a pcap you already have:

`$ zeek -Cr scripts/__load__.zeek your.pcap`


## Example Notice

Two notices can be generated from this package:
  - `CVE_2022_30216_Detection::ExploitAttempt`, and
  - `CVE_2022_30216_Detection::ExploitSuccess`

The first is generated when an attack is attempted, but does not necessarily succeed. The second is fired only when a successful exploit is detected and should be investigated immediately. Below is an example of a successful exploit notice.
```
XXXXXXXXXX.XXXXXX	CFLRIC3zaTU1loLGxh	192.168.56.104	53084	192.168.56.102	445	-	-	-	tcp	CVE_2022_30216_Detection::ExploitSuccess	Successful CVE-2022-30216 exploit: 192.168.56.104 exploited 192.168.56.102 relaying to 192.168.56.105	-	192.168.56.104	192.168.56.102	445	-	-	Notice::ACTION_LOG	(empty)	360XXXXXXXXXX.XXXXXX	-	-	-	-	-
```

## Installing

This package can be installed with `zkg` using the following commands:

```
$ zkg refresh
$ zkg install cve-2022-30216
```

## Test PCAPs
Our test pcaps were created by exploiting a proof of concept payload on an instance of [DetectionLab](https://www.detectionlab.network), slightly modified to use Windows Server 2022 for the domain controller and WEF machines, instead of the default, Windows Server 2016.

## References

1. https://cve.mitre.org/cgi-bin/cvename.cgi?name=2022-30216
2. https://www.detectionlab.network