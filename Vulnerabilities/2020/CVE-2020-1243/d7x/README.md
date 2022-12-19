# CVE-2020-12432

Collabora CODE <= 4.2.2 Vereign's WOPI API Stored XSS and Insecure permissions (account hijack) - 4.2.2  
  
Affected versions: Collabora CODE <= 4.2.2  
Exploitation requirements: The Vereign's /wopi/ API interface has to be accessible using the provided generated token, which is the default configuration.  
  
The wopi API interface developed by vereign (www.verign.com) and integrated into Collabora CODE's main package (https://www.collaboraoffice.com/code/) is prone to an xss attack and insecure file access permissions. After uploading a document with arbitrary html file, it is possible to steal the localstorage data by providing a specifically crafted link with an XSS redirect to a victim. 

PoC: 

1) Upload document file containing javascript code and intercept
the request through burp (or any other intercepting proxy). Put html
contents within the file, containing an html form and a javascript
sending cookies and local storage user data to a third party domain

2) Access the document via the web application and get the document
access token via the API response at the /wopi/getAccessToken endpoint
 
3) Send the malicious html link to the victim, you may include it in
an iframe or in a forum signature:
 
```
<a href=https://target.com/wopi/files/10/contents?access_token=<token>   >Click here</a>
```
 
4) The victim opens the link and gets redirected to another service
with all his cookies and local storage data sent over to another
network

Video: https://www.youtube.com/watch?v=_tkRnSr6yc0
