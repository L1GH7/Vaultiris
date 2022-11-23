# Exploit Title: Citrix Workspace app before 1912 for Windows - Privilege Escalation #2

Date: 2020-06-07<br/>
Author: Andrew Hess<br/>
Software Link: https://www.citrix.com/downloads/workspace-app/legacy-workspace-app-for-windows/workspace-app-for-windows-1911.html/<br/>
Bulletin: https://support.citrix.com/article/CTX275460<br/>
CVE: CVE-2020-13884<br/>


# History

2020.02.10 - Vulnerability discovered<br/>
2020.02.10 - Initial contact with the vendor<br/>
2020.XX.XX - ~~Vendor Patch 2006.1~~ The 1912 version, which was released after my report, already does not have the vulnerability<br/>
2020.06.06 - I created a CVE because the vendor did not publish the vulnerability<br/>
2020.06.10 - Contact with the vendor and correction of the CVE <br/>


# Software description

High performance access to Windows virtual apps and desktops, anywhere access from your desktop, start menu, Workspace app UI or web access with Chrome, Internet Explorer or Firefox.

Citrix Workspace app can be used on domain and non-domain joined PCs, tablets, and thin clients. Provides high performance use of virtualized Skype for Business, line of business and HDX 3D Pro engineering apps, multimedia, local app access.


# Exploit description

Citrix Workspace App before 1912 on Windows has Insecure Permissions for %PROGRAMDATA%\Citrix and an unquoted UninstallString, which allows local users to gain privileges by copying a malicious citrix.exe there.


# Affected Components

- C:\ProgramData\Citrix (Write ACL for Users)
- UninstallString in HKLM\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\CitrixOnlinePluginPackWeb is unquoted (C:\ProgramData\Citrix\Citrix Workspace 1911\TrolleyExpress.exe /uninstal)


## Steps To Reproduce

- Copy a malicious citrix.exe to C:\ProgramData\Citrix
- Wait until an admin or software distribution uninstalls the Citrix Workspace app. This will execute the malicious Citrix.exe.

## Impact

A possible attacker obtains system privileges
