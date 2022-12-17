(CVE-2021-21972) VMware vCenter Server Remote Code Execution Vulnerability
CVSSv3 score:-  9.8  Severity:- critical 
Description 
The vSphere Client (HTML5) contains a remote code execution vulnerability in a vCenter Server plugin. A malicious actor with network access to port 443 may exploit this issue to execute commands with unrestricted privileges on the underlying operating system that hosts vCenter Server. 
Affected versions 
This affects VMware vCenter Server (7.x before 7.0 U1c, 6.7 before 6.7 U3l and 6.5 before 6.5 U3n) 
 VMware Cloud Foundation (4.x before 4.2 and 3.x before 3.10.1.2).
Impact 
A malicious actor with network access to port 443 may exploit this issue to execute commands with unrestricted privileges on the underlying operating system that hosts vCenter Server. 
How the vulnerability happen 
CVE-2021-21972 is an unauthorized file upload vulnerability in vCenter Server. The issue rise from a lack of authentication in the vRealize Operations vCenter Plugin with endpoint /ui/vropspluginui/rest/services/*. url is accessible without authentication. The web application works with plugins. Each plugin is located in a separate jar file. Vropsplugin-service.jar file contains the implementation of vropspluginui. This plugin was configured to allow unauthorized users to access any URL it handles. And also the uploadOvaFile function in the Vropsplugin-service.jar file responsible for the URL /ui/vropsplugin/rest/services/uploadova. The method doesn't check the name of .tar entries. This means a malicious actor can create an archive entry containing path traversal characters which allow uploading arbitrary  files to an arbitrary directory on the server. To summarize authentication bypass and directory traversal vulnerabilities are identified vulnerabilities in the vRealize Operations vCenter Plugin.
How to detect the vulnerability 
Simply send GET or POST request to the vulnerable endpoints /ui/vropspluginui/rest/services/*.
Example: -
curl –i -s -k $’https://ip/ui/vropspluginui/rest/services/getstatus’ if this request response with status code 200 the server is vulnerable. 
curl –i -s -k $’https://ip/ui/vropspluginui/rest/services/uploadova’  if this request response with status code 405 the server is vulnerable.
How  to exploit this vulnerability 
A remote attacker could exploit this vulnerability by uploading a specially crafted file to a vulnerable vCenter Server endpoint that is publicly accessible over port 443. The server may be exploited for window based systems and linux based systems in different ways.For Windows systems, an attacker could upload a specially crafted .jsp file in order to gain SYSTEM privileges on the underlying operating system. For Linux systems, an attacker would need to generate and upload a public key to the server’s authorized_keys path and then connect to the vulnerable server via SSH.
solution 
The affected vCenter Server plugin for vROPs is available in all default installations. Temporary solution is changing the status of vrops plugin to ”incompatible” in the compatibility-matrix.xml file.vROPs does not need to be present to have this endpoint available.
Update vcenter server to version 7.0 U1c or  6.7 U3l or 6.5 U3n.
Update vmware cloud foundation to  version 4.2 or 3.10.1.2.
