#CVE-2019-12889

Summary

An unauthenticated privilege escalation exists in SailPoint Desktop Password Reset 7.2. A user with local access to only the Windows logon screen can escalate their privileges to NT AUTHORITY\System. An attacker would need local access to the machine for a successful exploit. The attacker must disconnect the computer from the local network / WAN and connect it to an internet facing access point / network. At that point, the attacker can execute the password-reset functionality, which will expose a web browser. Browsing to a site that calls local Windows system functions (e.g., file upload) will expose the local file system. From there an attacker can launch a privileged command shell.

Steps to reproduce:

1. At the login screen, disconnect from the office network and join any other Wi-Fi Internet access point. Then select Forgot Password.

2. A browser window will pop up but fail to connect to the password reset intranet site and display an error message with additional options. Click on the Refresh the page link.

3. Once browser window has been refreshed, click the Search for this site on Bing link.

4. On the Bing search page, click on the Camera button (Search using an image).

5. In the Try Visual Search pop-up box, click on the browse link.

6. A File Explorer window will appear displaying the C:\Windows\System32 folder. Click on the Custom Files selector box and change it to All Files.  Scroll down to the cmd application, right click on it and select the Run as administrator option. 

7. Close the File Explorer and Desktop Password Rest window by pressing the X button in the top right corner. Typing whoami will show the privileges: NT AUTHORITY\SYSTEM

