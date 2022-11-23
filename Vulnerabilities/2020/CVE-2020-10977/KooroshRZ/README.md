# Python Exploit for gitlab private instance arbitrary file read
At the date of 03/23/2020, a vulnerability report with its PoC was released in hackerone by **William Bowling**\
With this vulnerability we can read any world readable files (with permission 444 or more) on the gitlab server like /etc/passwd and so on... (worldreadable files only)\
```
Notice that you should be authenticated user
```

I've tested it on gitlab version 21.9.0 (ubuntu 18.04) but it's OS independent

# exploit-db
Many thanks to **exploit-db** team for publishing this exploit\
https://www.exploit-db.com/exploits/48431


# Credit
Many thanks to **William Bowling**\
vulnerability disclosed by : **William Bowling** of Biteable, a.k.a vakzz\
https://hackerone.com/reports/827052
