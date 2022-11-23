# Confluence-Question-CVE-2022-26138
Atlassian Confluence Server and Data Center: CVE-2022-26138

When the 'Questions for Confluence' app is installed and enabled on Confluence Server or Data Center, it creates a Confluence user account with the username 'disabledsystemuser' and password 'disabled1system1user6708', which is a hardcoded password and is added to the confluence-users group, which allows viewing and editing of all non-restricted pages within Confluence application by default.

  An attacker could use  these hardcoded credentials to log into Confluence and access all the content within the confluence-users group. This user account is created when installing versions 2.7.34, 2.7.35, and 3.0.2 of the app.

  To fix the Questions for Confluence issue, Atlassian recommends updating Questions for Confluence app to a secure version, or disabling/deleting the disabledsystemuser account.

### Dorks

Shodan
` title:Confluence `
` http.favicon.hash:-305179312 `

fofa: 
`icon_hash="-305179312" `


  
 ## CVE-2022-26138 PoC 
 
 #### Questions for Confluence Plugin

![Vulnerable Plugin](https://user-images.githubusercontent.com/79006904/181476877-3a592d66-b5a0-4cad-a74f-6dc160cdf6a0.png)

#### Credentials
 
![Credentials](https://user-images.githubusercontent.com/79006904/181476901-75e9d21b-13d0-4b89-bb14-08372119446f.png)


Dorks:

Shodan
title:Confluence
http.favicon.hash:-305179312

fofa: 
icon_hash="-305179312"

#### Follow us 
#### [Vulnmachines](https://www.twitter.com/vulnmachines)
#### [YouTube](https://www.youtube.com/c/vulnmachines)
#### [Twitter](https://www.twitter.com/vulnmachines)
#### [Facebook](https://www.facebook.com/vulnmachines)
#### [LinkedIn](https://www.linkedin.com/company/vulnmachines)
