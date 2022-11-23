# CVE-2021-44217
> [Suggested description]
> In Ericsson CodeChecker through 6.18.0,
> a Stored Cross-site scripting (XSS) vulnerability in the comments component of the
> reports viewer allows remote attackers to inject arbitrary web script or HTML via the POST JSON data of the /CodeCheckerService API.
> 
> ------------------------------------------
> 
> [Additional Information]
> CodeChecker web server has a permission system to isolate users with
> different privileges. And it also stores the cookie of each user in
> document.cookie. Therefor a low-priv attacker(such as the guest
> account) can utilize this bug to steal secret cookie of superuser or
> any other sensitive information of scanning reports by controlling the
> victims to request some data-fetching api. Using some out-of-band
> techniques, these sensitive information can be easily delivered out to
> the attacker's server.
> 
> ------------------------------------------
> 
> [Vulnerability Type]
> Cross Site Scripting (XSS)
> 
> ------------------------------------------
> 
> [Vendor of Product]
> Ericsson
> 
> ------------------------------------------
> 
> [Affected Product Code Base]
> CodeChecker - <= 6.18.0
> 
> ------------------------------------------
> 
> [Affected Component]
> "Comments" component of reports viewer
> 
> ------------------------------------------
> 
> [Attack Type]
> Remote
> 
> ------------------------------------------
> 
> [Impact Code execution]
> true
> 
> ------------------------------------------
> 
> [Impact Escalation of Privileges]
> true
> 
> ------------------------------------------
> 
> [Impact Information Disclosure]
> true
> 
> ------------------------------------------
> 
> [Attack Vectors]
> To exploit this vulnerability, someone needs to add a comment under any scanning report.
> 
> ------------------------------------------
> 
> [Reference]<br>
> https://codechecker-demo.eastus.cloudapp.azure.com/<br>
> https://user-images.githubusercontent.com/9525971/142965091-e118b012-a7fc-4c2f-ad0c-80aeed6f7ec9.png<br>
> https://github.com/Ericsson/codechecker/releases<br>
> 
> ------------------------------------------
> 
> [Discoverer]
> Xinyi Chen - S&G Security TMG

The comments component of reports viewer doesn't check the input of user, which leads to a stored XSS under this page.<br>
![image](https://user-images.githubusercontent.com/9525971/143382398-655a3dac-272c-4e67-b064-e52592794daf.png)<br>
An attacker may exploit this bug to steal secret cookie or any other sensitive information via data-fetching api.<br>
![image](https://user-images.githubusercontent.com/9525971/142965091-e118b012-a7fc-4c2f-ad0c-80aeed6f7ec9.png)
