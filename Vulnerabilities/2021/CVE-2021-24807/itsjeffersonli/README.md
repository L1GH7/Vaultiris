# CVE-2021-24807
Support Board 3.3.4 Authenticated Stored XSS

Authenticated Stored Cross Site Scripting (XSS) Vulnerability exists in Support Board Version 3.3.4 via Notes parameter.

The impact of cross-site scripting vulnerabilities can vary from one web application to another. It ranges from session hijacking to credential theft and other security vulnerabilities. By exploiting a cross-site scripting vulnerability, an attacker can impersonate a legitimate user and take over their account. If the victim user has administrative privileges, it might lead to severe damage such as modifications in code or databases to further weaken the security of the web application, depending on the rights of the account and the web application.

To Exploit this Vulnerabilites you need to have Agent or Admin Level Access.

### Add Notes Function

To exploit this parameter we need to go to chat and click on the + icon on the right side of the Notes and add like some text and press Add and intercept with BurpSuite
![enter image description here](https://cdn-images-1.medium.com/max/800/1*MdEk_L4IZlYUrqNpnvAURg.png)

As you can see in the Burpsuite Requests we can see the message Parameter we need to add the Payload inside it

<ScRiPt>alert(document.cookie)</sCriPt>

and Send the Request
![enter image description here](https://cdn-images-1.medium.com/max/1200/1*tAcwXhIG6_vfaEbvI__x2g.png)
now if we reload the webpage it will automatically show the cookies
![enter image description here](https://cdn-images-1.medium.com/max/800/1*lObsbeV4LxjThXD08awKpA.png)


Readmore:  
[Medium](https://medium.com/@lijohnjefferson/6bc22af2a444)  
[TIStory]()
