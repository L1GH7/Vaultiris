# CVE-2018-3990-TALOS-2018-0658
 An exploitable pool corruption vulnerability exists in the 0x8200E804 IOCTL handler functionality of WIBU-SYSTEMS WibuKey.sys Version 6.40 (Build 2400). 
 
 A specially crafted IRP request can cause a buffer overflow, resulting in kernel memory corruption and, potentially, privilege escalation. An attacker can send an IRP request to trigger this vulnerability.

 A complete technical analysis of this vulnerability will be created upon the finishing of this exploit proof-of-concept.

 Unfortunately, this vulnerability appears to only be present in the 32-bit version of the binary, so I have to somehow get ahold of the 32-bit version of this driver. This repository, like many others, will be post-poned until then.
