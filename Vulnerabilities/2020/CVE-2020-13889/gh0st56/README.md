Author: Andre k Lorenci
Contact: avlorenci@gmail.com

CVE-2020-13889

Hello, this vulnerability consists in a function called showAlert() in the administration panel of bludit,that when accessed in DOM, allows users define the text to be popped up in the message box. But this function dont have any sanatization and the user can inject any javascript code or even HTML in the page

To demonstrate this function return I used the web development toolkit to pass a XSS code as argument of that function.

![XSS in admin page](https://github.com/gh0st56/bludit-DOM-xss/blob/master/xss-bludit.jpg)

the payload used was:
showAlert("<script>alert(1)</script>");

The versions that i tested was the Bludit 3.x. 

Thank you.

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-13889
