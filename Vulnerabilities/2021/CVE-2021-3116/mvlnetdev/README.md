# CVE-2021-31166 Detection Rules
Different rules to detect if CVE-2021-31166 is being exploited

Rules available:
- Suricata
- Snort
  - The snort rules also work on suricata.
- Zeek

To Do:
- Make PCAPs available which where used during the development of the signatures

# Zeek script
- Detection when the CVE-2021-31166 vulnerability is being exploited.
- Detection if the 'exploited host' is down (this has a high threshold and may give some false negatives).
- Only detect on a given subnet. Default: 192.168.0.0/16, 172.16.0.0/12 and 10.0.0.0/8.
- Learn hosts that have IIS installed or have the WinRM or WSDAPI enabled an thus may be vulnerable. Only detect for the exploit on those hosts after they have been found. 
  - This feature is turned off by default. Change the value of `learn_iis_webservers` to `T` if you want it turned on.

# FAQ

## Which OS versions are vulnerable?
- Windows Server, version 2004 (or 20H1) (Server Core installation),
- Windows 10 Version 2004 (or 20H1) for ARM64/x64/32-bit Systems,
- Windows Server, version 20H2 (Server Core Installation),
- Windows 10 Version 20H2 for ARM64/x64/32-bit Systems.

For more information visit: <https://msrc.microsoft.com/update-guide/vulnerability/CVE-2021-31166>

The following software is vulnerable on those versions:
- IIS
- WinRM
- WSDAPI

## Do the suricata rules and the zeek script also work on WinRM and WSDAPI?
Yes they do. The ports for WinRM and WSDAPI are:
- WinRM = 5357
- WSDAPI = 5985 (HTTP)

# Credits:
- Initial PoC: <https://github.com/0vercl0k/CVE-2021-31166>
- WinRM affected: <https://twitter.com/JimDinMN/status/1395071966487269376>
- WSDAPI affected: <https://twitter.com/HenkPoley/status/1394309837304082439>

# Disclaimer
The scripts or rules have not been tested in production. Use at your own risk.

I basically have no idea what I am doing.

Please send feedback if you have it.
