# Infineon-CVE-2017-15361
Simple PowerShell script to check whether a computer is using an Infineon TPM chip that is vulnerable to CVE-2017-15361.

IMPORTANT: 
- THIS MUST BE EXECUTED AS ADMINISTRATOR!!
- This script only works on Windows 8.1 and Windows Server 2012 or later releases of Windows.

The script was reused from Microsoft: https://portal.msrc.microsoft.com/en-US/security-guidance/advisory/ADV170012

More information about how you can test for the vulnerable can be found on https://www.vanstechelman.eu/content/testing-for-impact-of-infineons-vulnerable-rsa-generation-cve-2017-15361

Note: if the script doesn't work at first, you can try modifying the Windows PowerShell Script Execution Policy:
- To view which Powershell Execution Policy is currently applied, execute `Get-ExecutionPolicy`
- To change the Powershell Execution Policy to unrestricted, execute `Set-ExecutionPolicy unrestricted`
- To rollback and reset the policy to initial value, execute `Set-ExecutionPolicy <restricted|allsigned|remotesigned|unrestricted>`