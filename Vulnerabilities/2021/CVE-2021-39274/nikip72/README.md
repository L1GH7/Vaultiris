# CVE-2021-39273
## _In XeroSecurity Sn1per 9.0 (free version), insecure permissions (0777) are set upon application execution, allowing an unprivileged user to modify the application, modules, and configuration files. This leads to arbitrary code execution with root privileges._ 

# CVE-2021-39274
## _In XeroSecurity Sn1per 9.0 (free version), insecure directory permissions (0777) are set during installation, allowing an unprivileged user to modify the main application and the application configuration file. This results in arbitrary code execution with root privileges._

## Tested on: Ubuntu 21.04

## From Sn1per installation insctructions:
```
KALI/UBUNTU/DEBIAN/PARROT LINUX INSTALL:

git clone https://github.com/1N3/Sn1per
cd Sn1per
bash install.sh
```
## Installation

```
root@snipertest:~# git clone https://github.com/1N3/Sn1per
Cloning into 'Sn1per'...
remote: Enumerating objects: 2838, done.
remote: Counting objects: 100% (13/13), done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 2838 (delta 5), reused 10 (delta 4), pack-reused 2825
Receiving objects: 100% (2838/2838), 43.18 MiB | 18.76 MiB/s, done.
Resolving deltas: 100% (1950/1950), done.
root@snipertest:~# cd Sn1per/
root@snipertest:~/Sn1per# bash install.sh
                ____
    _________  /  _/___  ___  _____
   / ___/ __ \ / // __ \/ _ \/ ___/
  (__  ) / / // // /_/ /  __/ /
 /____/_/ /_/___/ .___/\___/_/
               /_/

 + -- --=[ https://xerosecurity.com
 + -- --=[ Sn1per by @xer0dayz

[>] This script will install sn1per under /usr/share/sniper. Are you sure you want to continue? (Hit Ctrl+C to exit)
y
[snip]
'/usr/share/sniper/sniper.conf' -> '/root/.sniper.conf'
Please run msfdb as a non-root user
[*] Adding start menu and desktop shortcuts...
[>] Done!
[>] To run, type 'sniper'!
root@snipertest:~/Sn1per#
```
## CVE-2021-39274

Right after the installation install directory permissions are set as:
```
root@snipertest:~/Sn1per# ls -ld /usr/share/sniper/
❗drwxrwxrwx❗ 10 root root 4096 Aug 16 19:19 /usr/share/sniper/

root@snipertest:~/Sn1per# ls -la /usr/share/sniper/
total 6668
drwxrwxrwx  10 root root    4096 Aug 16 19:19 .
drwxr-xr-x 222 root root   12288 Aug 16 19:41 ..
-rw-r--r--   1 root root   36518 Aug 16 18:33 CHANGELOG.md
-rw-r--r--   1 root root    1269 Aug 16 18:33 Dockerfile
-rw-r--r--   1 root root     306 Aug 16 18:33 LICENSE.md
-rw-r--r--   1 root root   11697 Aug 16 18:33 README.md
-rw-r--r--   1 root root 6491364 Aug 16 18:33 Sn1per.gif
-rw-r--r--   1 root root  159147 Aug 16 18:33 Sn1per.jpg
drwxr-xr-x   2 root root    4096 Aug 16 18:33 bin
drwxr-xr-x   2 root root    4096 Aug 16 19:41 conf
rwxr-xr-x   1 root root   11548 Aug 16 18:33 install.sh
drwxr-xr-x   9 root root    4096 Aug 16 19:42 loot
drwxr-xr-x   2 root root    4096 Aug 16 18:33 modes
drwxr-xr-x  43 root root    4096 Aug 16 19:27 plugins
drwxr-xr-x   2 root root    4096 Aug 16 18:33 pro
-rw-r--r--   1 root root     276 Aug 16 18:33 sn1per.desktop
-rw-r--r--   1 root root    4283 Aug 16 18:33 sn1per.png
-rwxr-xr-x   1 root root   28290 Aug 16 18:33 sniper
-rw-r--r--   1 root root    9752 Aug 16 18:33 sniper.conf
drwxr-xr-x   4 root root    4096 Aug 16 18:33 templates
-rwxr-xr-x   1 root root     980 Aug 16 18:33 uninstall.sh
drwxr-xr-x   2 root root    4096 Aug 16 18:33 wordlists
```

Although all files are owned by root, permissions on the containing directory allow us to manipulate the files, so a backdoor can be placed in one of the scripts. Another, probable more stealthy, modification would be to place the backdoor in the config file (`sniper.conf`) as this file is sourced via bash `source` command.

As the application must be executed by root, ensured by an EUID check in the script:

```
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi
```
placing backdoor in it will execute it with `root privileges` next time the application is used.

## Root cause

Lines 37-40 of the installation script:
```
mkdir -p $INSTALL_DIR 2> /dev/null
chmod 777 -Rf $INSTALL_DIR 2> /dev/null
chown root $INSTALL_DIR/sniper 2> /dev/null
chmod 4777 $INSTALL_DIR/sniper 2> /dev/null
```

## CVE-2021-39273

`Different issue than CVE-2021-39274`

Upon first execution of the application as a scanner main script `sniper` executes function `init` that recursevly changes permissions on the installation directory and it's contents.

As with _CVE-2021-39274_ this allows modification of all application files including executables and configurations and placing backdoor in them that would be executed with `root privileges` next time application is used by `root`.
 
 ## Root cause
 
 Lines 464-466 of the the main script:
  ```
  chmod 777 -Rf $INSTALL_DIR 2> /dev/null
  chown root $INSTALL_DIR/sniper 2> /dev/null
  chmod 4777 $INSTALL_DIR/sniper 2> /dev/null
 ```
 
 
## _Discovered by Nikola Pepelishev, August 2021_
