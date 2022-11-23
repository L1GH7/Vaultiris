# CVE-2019-8561

Proof of concept exploit for CVE-2019-8561 discovered by Jaron Bradley ([@jbradley89](https://twitter.com/jbradley89))  (Patched in macOS 10.14.4). This script exploits a TOCTOU bug in `installer` which enables code execution as root.

See Jaron's Objective By the Sea v2 talk "[Bad Things in Small Packages](https://www.youtube.com/watch?v=5nOxznrOK48)" where he demonstrates getting r00t and bypassing SIP.

(**N.B All scripts other than `gpg_poc` are half finished and likely don't work in their current state. Published for sake of completeness**)

My accompanying blog post "[CVE-2019-8561 Proof of Concept Exploit](https://0xmachos.com/2021-04-30-CVE-2019-8561-PoC/)".

# [gpg_poc](https://github.com/0xmachos/CVE-2019-8561/blob/master/gpg_poc)

Monitors`$HOME/Downloads` for a GPG Suite DMG. When it finds one it converts the DMG from read only to RW then resizes it to 60MB. 

Once the installer starts it expands the package, modifies the `preinstall` script to create `/var/test` via `touch` then flattens it in place of the original package. 

The modified package contents will be used by installer however the UI will still indicate that the package is correctly code signed.

Tested on:
* 10.14.2

