The script  will check if your Azure AD Connect is running a version affected by the vulnerability described in CVE-2021-36949. Affected versions are 1.6.4.0 and 2.0.3.0.
Run the script on the server that hosts the AAD Connect installation.

The script also checks if the auto-upgrade feature is enabled. By default, it is but as explained in the version history, only some releases will be deployed by auto-upgrade. The last one that is marked as auto-upgrade is 1.4.25.0 from 28.09.2019, so Microsoft obviously rarely makes use of this function. Therefore, see the description of the manual upgrade process to make sure your AAD Connect is up-to-date.

Links:

Microsoft Azure Active Directory Connect Authentication Bypass Vulnerability

https://msrc.microsoft.com/update-guide/vulnerability/CVE-2021-36949 

Azure AD Connect: Version release history

https://docs.microsoft.com/en-us/azure/active-directory/hybrid/reference-connect-version-history

Azure AD Connect: Upgrade from a previous version to the latest

https://docs.microsoft.com/en-us/azure/active-directory/hybrid/how-to-upgrade-previous-version

Two new Azure AD Connect versions were released to prevent MitM attacks towards Domain Controllers (CVE-2021-36949)

https://dirteam.com/sander/2021/08/10/two-new-azure-ad-connect-versions-were-released-to-prevent-mitm-attacks-towards-domain-controllers-cve-2021-36949/ 
