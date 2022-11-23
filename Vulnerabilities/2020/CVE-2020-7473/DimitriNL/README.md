# CTX-CVE-2020-7473
Citrix Sharefile Vulnerbility check<br><br>

<b>Fast Research details CTX-CVE-2020-7473</b><br>
Made on 2020.04.16<br>
<br>
<b>Vulnerbility check: </b><br>
Open in a webbrowser<br>
https://yoursharefileserver.companyname.com/UploadTest.aspx<br>
Or<br>
curl https://yoursharefileserver.companyname.com/UploadTest.aspx --path-as-is<br>
<br>
Blank page = Server vulnerable<br>
Error 404 = server has been patched<br>
<br>
<b>Notes:</b><br>
Output can be different if behind a WAF/Netscaler:<br>
https://docs.citrix.com/en-us/storage-zones-controller/5-0/install/sf-deploy-cfg-netscaler.html<br>
Credit: https://twitter.com/chris_e_tweets<br>
<br>
<b>Inportant changes after mitigation tool:</b><br>
Changes web.config<br>
delete files UploadTest.aspx & XmlFeed.aspx<br>
<br>
<b>Installed:</b><br>
AjaxControlToolkit<br>
2013.12.14 | Version: 4.1.7.1213<br>
<br>
<b>Citrix mitigation tool & details:</b><br>
CVE-2020-7473 - CVE-2020-8982 - CVE-2020-8983<br>
https://support.citrix.com/article/CTX269106<br>
<br>
Credits to the Danske Bank Red-Team<br>
<br>
<br>
Do you have more tips? Let me know on my twitter and I will put it in this document.<br>
<b>My Twitter:</b> https://twitter.com/dimitrinl<br>

