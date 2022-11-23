# CVE-2022-23342
Security Advisory – Username Enumeration in OnBase

 

Affected software versions and builds: OnBase Application Server

 

OnBase releases prior to 20.3.58.1000 and OnBase releases 21.1.1.1000 through 21.1.15.1000 are impacted.

 

Fixed software versions and builds:

OnBase EP3 releases greater than or equal to 20.3.58.1000
OnBase EP5 releases greater than or equal to 21.1.16.1000
 

Bulletin: 202206-ONBASE-1

CVSS severity: Medium

CVSS score: 5.3

CVSS vector: AV:N/AC:L/PR:N/UI:N/S:U/C:L/I:N/A:N

 

Issue description    

Through responsible disclosure with a security researcher, Hyland is aware that OnBase is susceptible to Active Directory username enumeration during authentication. Successful exploitation could allow unauthenticated users to identify valid and invalid Active Directory usernames. The reported vulnerability has been investigated and the findings have been remediated.

 

Corrective action

If you are using software listed in the “Affected Software Versions and Builds” section of this bulletin, you can remediate this vulnerability by upgrading to a version listed in the “Fixed Software Versions and Builds” section by engaging your first line of support.


POC
The Hyland OnBase allows an external attacker to perform username enumeration due to the response returned by the system e.g. the system responds with different errors for valid and invalid users. For invalid users, the system returns “An unknown login error occurred.” for valid users, a “success” or " Login failed." error is returned.
An attack can enumerate instances by sending a login POST request to the  /mobilebroker/ServiceToBroker.svc/Json/Connect endpoint. This can lead to user enumeration against the underlying Active Directory-integrated systems.

![image](https://user-images.githubusercontent.com/954507/174392068-dbfe64cf-a2c6-43fc-b16a-5a242707bbc5.png)

