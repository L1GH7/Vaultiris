# CVE-2020-3153 Cisco AnyConnect Secure Mobility Client EoP PoC

Thanks to Yorick Koster for publishing details. This is a proof of concept for a path traversal vulnerability in Cisco AnyConnect Secure Mobility Client.
Tested with Windows 7 and Windows 10 and AnyConnect version 4.5.x and 4.6.x. For version 4.7.04x and 4.8.x you need to run anypoc_4.7.exe.


Copy the files from [anyconnect.zip](anyconnect.zip) to C:\anyconnect\

1. search and download "anyconnect-win-4.6.03049-predeploy-k9.zip" from the internet

2. unzip anyconnect-win-4.6.03049-predeploy-k9.zip

3. download 7-zip_portable, https://portableapps.com/apps/utilities/7-zip_portable

4. extract anyconnect-win-4.6.03049-posture-predeploy-k9.msi with 7-zip_portable

5. copy cstub.exe to C:\anyconnect\

Run C:\anyconnect\anypoc.exe

or run C:\anyconnect\anypoc_4.7.exe for version 4.7.04x and above

## Links

* https://seclists.org/fulldisclosure/2020/Apr/43
* https://nvd.nist.gov/vuln/detail/CVE-2020-3153
* https://ssd-disclosure.com/ssd-advisory-cisco-anyconnect-privilege-elevation-through-path-traversal/
