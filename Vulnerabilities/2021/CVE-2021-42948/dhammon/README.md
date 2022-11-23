
# CVE-2021-42948
HotelDruid Hotel Management Software v3.0.3 and below was discovered to have exposed session tokens in multiple links via GET parameters, allowing attackers to access user session id's.

The session token used to access authenticated pages of the application is passed using GET methods generated from the head.php file.  The head.php file creates the application's menu bar to navigate to various application functions.  Every link in the menu bar exposes the session token as the value for the parameter id_sessione.

The session token plaintext is exposed in GET requests which can be intercepted by attackers through proxies, man in the middle attacks, or similar and used to hijack application user sessions.

## Remediation
The vendor has decided not to pursue a patch to remediate this vulnerability.

## Product Reference
https://www.hoteldruid.com/

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-42948
