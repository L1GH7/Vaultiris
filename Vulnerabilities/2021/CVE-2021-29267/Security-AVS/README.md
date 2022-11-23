# CVE-2021-29267
SherlockIM ChatBot XSS

[Suggested description]: SherlockIM is vulnerable to Persistent XSS.

[Additional Information]: SherlockIM - Platform for manage all kind of conversations with customers.

[Vulnerability Type]: Cross Site Scripting (XSS)

[Vendor of Product]: https://sherlockim.ae

A letter was sent to the vendor about the vulnerability.

[Attack Type]: Remote

[Attack Vectors]: Attacker uploads malicious HTML file via chat bot. Malicious file is loaded in customer subdomain (every customer has unique subdomain in the sherlockcrm domain). Malicious code in uploaded file successfully executed in "customer.sherlockcrm.ru". Link to uploaded file leaks via chat bot and has the following format: https://customer.sherlockcrm.ru/api/Files/Attachment/unique_ID.html. Link is valid until manual delete. Attacker can attack help desk employees. 

[Discovered]: Alexander Semenenko

[Reference]: https://sherlockim.ae

[Proof of Concept]:

![alt text](https://github.com/Security-AVS/CVE-2021-29267/blob/main/Successful%20execution%20of%20code.png)
