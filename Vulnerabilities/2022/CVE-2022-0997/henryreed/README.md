# Fidelis Network and Deception - CVE-2022-0997 - Insecure File Permissions Privilege Escalation
#### Releasability and Acknowledgements
The exploit was approved for public release by the United States Government. Special thanks to the servicemen and government liaisons tasked with approving the documentation for unlimited distribution. Please refer to the following OTR numbers:
* OTR-2022-00487
#### Metadata
**Date:** 20220516 0123H UTC

**Exploit Author:** Henry Reed, The Aerospace Corporation

**Vendor Homepage:** https://fidelissecurity.com/

**Affected Versions:** 

* Fidelis CommandPost prior to 9.4.5
* Fidelis Collector prior to 9.4.5
* Fidelis Sensor prior to 9.4.5

**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

**CVE:** [CVE-2022-0997](https://nvd.nist.gov/vuln/detail/CVE-2022-0997)

**CVE Description:** Improper file permissions in the CommandPost, Collector, and Sensor components of Fidelis Network and Deception enables an attacker with local, administrative access to the CLI to modify affected script files, which could result in arbitrary commands being run as root upon subsequent logon by a root user. The vulnerability is present in Fidelis Network and Deception versions prior to 9.4.5. Patches and updates are available to address this vulnerability.

## Bottom Line Up Front
Scripts in the /etc/profile.d/ directory are launched on login. The fidelis user owns the login.sh script, which allows the fidelis user to arbitrarily modify the file. Upon a subsequent logon by a root user, this script is launched, allowing an adversary to gain arbitrary code execution as root. 

It is recommended that affected Fidelis Deception or Fidelis Network users update their software to patch this vulnerability. It is also recommended for all users, regardless of if they were affected by this vulnerability, to disable shell access to their Fidelis products after initial configuration. 

If, for some reason, either of these is not possible, a mitigation for the vulnerable file is described in this document. To the extent of the author's testing, it appears that users will need to use the vulnerability to escalate privileges before they are able to apply the mitigations described, so updating Fidelis and disabling SSH is strongly recommended, instead.

## login.sh Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /etc/profile.d/login.sh allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. Scripts in /etc/profile.d are launched each time a user opens a shell. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the script and, when the root user logs in, run arbitrary code, such as a reverse shell.

#### Exploit:
1. Modify the login.sh file using vim or another text editor:
```bash
$ vim /etc/profile.d/login.sh
```
2. Wait until the desired user, for instance the root user, logs in. At this point, the modified script would run with that user’s permissions.

#### Walkthrough:
The vulnerability was first discovered using [LinPEAS](https://github.com/carlospolop/PEASS-ng/tree/master/linPEAS), a Linux privilege escalation enumeration script: 

![](images/login.sh_linPEAS.png)

Note the text in red highlight, "You have write privileges over /etc/profile.d/login.sh". Red-highlighted output indicates high confidence in the exploit being function.

Each potential category of exploits disclosed by LinPEAS is accompanied by a link to a guide on nook.hacktricks.xyz explaining how to exploit the vulnerability. After a quick reading, it was decided that a reverse shell through Python2 would be the easiest way to exploit this vulnerability; however, any other code could have also been used. The new code begins at the if statement `if [[ $(whoami) = root ]]`:

![](images/login.sh_evil.png)

After adding the reverse shell to login.sh, we simulate a root login by manually logging in as that user. Note that in the real world, the attacker would have to wait until an administrator manually logs in as the root user.

![](images/login.sh_root_login.png)

The attacker then gets a reverse root shell:

![](images/login.sh_revvie.png)

#### Mitigation:
Changing the user and group owners of /etc/profile.d/login.sh to root:root and only allowing read and execute permissions to the “others” category of users on /etc/profile.d/login.sh would mitigate this vulnerability:

![](images/login.sh_mitigation.png)
