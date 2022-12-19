# CVE-2020-11932 :bug::mag:

Check CVE-2020-11932 and test for host relating to this vulnerability

## Usage
### Download
```console
git clone https://github.com/Staubgeborener/CVE-2020-11932
cd CVE-2020-11932
chmod +x cve-2020-11932.sh
./cve-2020-11932.sh
```

### With curl (so no download)
```console
bash <(curl -s https://raw.githubusercontent.com/Staubgeborener/CVE-2020-11932/master/cve-2020-11932.sh)
```

## Explanation
This is kind of a proof of concept of the vulnerability [CVE-2020-11932](https://nvd.nist.gov/vuln/detail/CVE-2020-11932). It's possible, that the `Ubuntu Server` logs the password of the `LUKS` full disk encryption in *plain text*. This one is tested on `Ubuntu Server 20.04`.

Created `LUKS` encryption (`LVM`) with password `T0pS3cr3tP4ssw0rd`. We can find five files that contain the password in plain text `sudo grep -Rl "T0pS3cr3tP4ssw0rd" /`:
* subiquity-curtin-install.conf
* curtin-install-cfg.yaml
* curtin-install.log
* installer-journal.txt
* autoinstall-user-data

```bash
user@encryptiontest:~$ sudo grep -Rl "T0pS3cr3tP4ssw0rd" /
/var/log/installer/subiquity-curtin-install.conf 
/var/log/installer/curtin-install-cfg.yaml 
/var/log/installer/curtin-install.log 
/var/log/installer/installer-journal.txt 
/var/log/installer/autoinstall-user-data 
```
```bash
user@encryptiontest:~$ grep "T0pS3cr3tP4ssw0rd" /var/log/installer/subiquity-curtin-install.conf
- {volume: partition-2, key: T0pS3cr3tP4ssw0rd, preserve: false, type: dm_crypt, 
```
```bash
user@encryptiontest:~$ sudo grep T0pS3cr3tP4ssw0rd /var/log/installer/curtin-install-cfg.yaml 
[sudo] password for user: 
key: T0pS3cr3tP4ssw0rd 
```
```bash
user@encryptiontest:~$ sudo grep "T0pS3cr3tP4ssw0rd" /var/log/installer/curtin-install.log 
get_path_to_storage_volume for volume dm_crypt-0({'volumel: 'partition-2', 'key': T0pS3cr3tP4ssw0rd, 'preserve': False, 'type': 'dm_crypt', 'id': 'dm_crypt-0'})
```
```bash
user@encryptiontest:~$ sudo grep -o "T0pS3cr3tP4ssw0rd" /var/log/installer/installer-journal.txt 
T0pS3cr3tP4ssw0rd 
T0pS3cr3tP4ssw0rd 
```
```bash
user@encryptiontest:~$ sudo grep T0pS3cr3tP4ssw0rd /var/log/installer/autoinstall-user-data
- {volume: partition-2, key: T0pS3cr3tP4ssw0rd, preserve: false, type: dm_crypt, 
```
```bash
root@encryptiontest:/home/user# ./CVE-2020-11932.sh 
Checking subiquity-curtin-install.conf: 
volume: partition-2, key: T0pS3cr3tP4ssw0rd, preserve: false, type: dm_crypt 
```


⇒ After running `cve-2020-11932.sh` you will get an output like this:

```bash
Checking curtin-install-cfg.yaml: 
key: T0pS3cr3tP4ssw0rd 

Checking curtin-install.log: 
get_path_to_storage_volume for volume dm_crypt-0({'volume': 'partition-2', 'key': 'T0pS3cr3tP4ssw0rd', 'preserve': False, 'type': 'dm_crypt', 'id': 'dm_crypt-01'}) 

Checking installer-journal.txt: 
'T0pS3cr3tP4ssw0rd', 'preserve': False, 'type': 'dm_crypt', 'id': 'dm_crypt-0'}, {'name': 'ubuntu-vg', 'devices': ['dm_crypt-0'], 'preserve': False, 'type': 'lvm_volgroup', 'id': lvm_volgroup-0', 'name': 'ubuntu-lv', 'volgroup': lvm_volgroup-0', 'size': '42949672966', 'preserve': False, 'type': 'lvm_partition', 'id': lvm_partition-01, {'fstype': 'ext4', 'volume': lvm_partition-0' 
'T0pS3cr3tP4ssw0rd' 

Checking autoinstall-user-data: 
{volume: partition-2, key: T0pS3cr3tP4ssw0rd, preserve: false, type: dm_crypt 

CVE-2020-11932 vulnerability on this Ubuntu Release: 20.04 ! 

Found: key1: T0pS3cr3tP4ssw0rd 
key2: T0pS3cr3tP4ssw0rd 
key3: get_path_to_storage_volume for volume dm_crypt-0({'volumel: 'partition-2', 'key': 'T0pS3cr3tP4ssw0rd', 'preserve': False, 'type': 'dm_crypt', 'id': 'dm_crypt-0'})
key4: T0pS3cr3tP4ssw0rd 
T0pS3cr3tP4ssw0rd 
key5: T0pS3cr3tP4ssw0rd 
```


## License
 [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
 
This project is licensed under The MIT License. Take a look at the [license file](https://github.com/Staubgeborener/CVE-2020-11932/blob/master/LICENSE) for more informations.
