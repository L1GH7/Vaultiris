# CVE-2020-16270

**[Suggested description]**: OLIMPOKS under 3.3.39 allows Auth/Admin ErrorMessage XSS.  
Remote Attacker can use discovered vulnerability to inject malicious JavaScript payload to victim’s browsers in context of vulnerable applications. Executed code can be used to steal administrator’s cookies, influence HTML content of targeted application and perform phishing-related attacks. Vulnerable application used in more than 3000 organizations in different sectors from retail to industries.  
**[Additional Information]**:   
A letter was sent to the vendor about the vulnerability. Vulnerability was confirmed by vendor.  
Vulnerability was confirmed by government agency: https://bdu.fstec.ru/vul/2020-04623  
**[Vulnerability Type]**: Cross Site Scripting (XSS)  
**[Vendor of Product]**: olimpoks.ru  
**[Affected Product Code Base]**: online olimpoks system under 3.3.39.  
**[Attack Type]**: Remote  
**[Impact Denial of Service]**: False  
**[Impact Information Disclosure]**: True  
**[Attack Vectors]**: Vulnerable URL and parameter: http://olimpoks.TARGET-HOST:9001/Auth/Admin?ErrorMessage=bb%27);alert(1);//  
**[Discovered]**: Alexander Semenenko, Luka Safonov, Sergey Zelensky  
**[Proof of Concept]**:  
![stack Overflow]( https://raw.githubusercontent.com/Aleksander-Semenenko/CVE-2020-16270/main/POC_CVE-2020-16270.png)
