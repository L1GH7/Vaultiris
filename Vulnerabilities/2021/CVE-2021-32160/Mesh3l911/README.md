# ..| XSS to Privileged user |..

# Description :
<b>Exploiting a Reflected Cross-Site Scripting (XSS) attack to create a privileged user through the Webmin's add users feature then getting a reverse shell through the Webmin's running process feature<br>
</b><br>
<h1>Tested Version :</h1>
<b>Webmin 1.973 ( GitHub's latest version 07/03/2021 )
</b><br>
<h1>Attack Type: </h1>
<b>Remote
</b>
<br>
<h1>Impact :<br></h1>
<b>Remote Command Execution
</b><br>
<br>
<h1>eXploit's C0de POC :</h1>
<p>
    <img src="exploitPOC.png" width="1200" height="550" />
</p>
<h1>Vendor of Product :</h1>
<code> https://www.webmin.com </code><br><br>
<h1>Additional Information :</h1>
<b>
<code> "Webmin is a web-based system administration tool for Unix-like servers, and services with over 1,000,000 installations worldwide. Using it, it is possible to configure operating system internals, such as users, disk quotas, services or configuration files, as well as modify, and control open-source apps, such as BIND DNS Server, Apache HTTP Server, PHP, MySQL, and many more" </code> According to Webmin's GitHub <br>

<br>
</b>
<h1>Discoverers :</h1>
<b>
Mesh3l_911 & Z0ldyck <br>
Twitter: <code> @mesh3l_911 </code> ,<code>@electronicbots </code>
<br>
</b>
