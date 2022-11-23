# CVE-2022-26157

An issue was discovered in the web application in Cherwell Service
Management (CSM) 10.2.3. The ASP.NET_Sessionid cookie is not protected
by the Secure flag. This makes it prone to interception by an attacker
if traffic is sent over unencrypted channels.
