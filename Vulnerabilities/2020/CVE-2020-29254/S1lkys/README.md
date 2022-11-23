# CVE-2020-29254
TikiWiki 21.2 allows to edit templates without the use of a CSRF protection. 


Please read the full writeup by Maximilian Barz for this CVE and other vulnerabilities including a full attack chain [here](https://github.com/S1lkys/CVE-2020-29254/blob/main/Tiki-Wiki%2021.2%20by%20Maximilian%20Barz.pdf)


==========================

Cross-Side-Request-Forgery (CSRF):

TikiWiki 21.2 allows to edit templates without the use of a CSRF protection. 
This could allow an unauthenticated, remote attacker to conduct a cross-site request forgery (CSRF) attack and perform arbitrary actions on an affected system. The vulnerability is due to insufficient CSRF protections for the web-based management interface of the affected system. An attacker could exploit this vulnerability by persuading a user of the interface to follow a maliciously crafted link. A successful exploit could allow the attacker to perform arbitrary actions on an affected system with the privileges of the user. These action include allowing attackers to submit their own code through an authenticated user resulting in local file Inclusion. If an authenticated user who is able to edit TikiWiki templates visits an malicious website, template code can be edited.


# Watch the full procedure here:
[![CVE-2020-29254 yt video](https://img.youtube.com/vi/Uc3sRBitu50/0.jpg)](https://youtu.be/Uc3sRBitu50)

# A response from the Tiki project
https://doc.tiki.org/CVE-2020-29254
