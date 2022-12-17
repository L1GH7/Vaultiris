## What is included?
* CVE-2021-3438_detection v2_sccm.ps1 is a script to run via sccm to get a small report on the vulnerability of managed devices.
* CVE-2021-3438_detection.ps1 is a script to run manually or remote on a managed device.
* CVE-2021-3438_remediation.ps1 is a script to check and run the installers if vulnerable drivers are detected.
* HP-Samsung.exe is the HP/Samsung fix installer
* Xerox is the Xerox fix installer.

## How to execute these scripts?
Instant run:
```
PS> CVE-2021-3438_detection.ps1
```
Importing the function:
```
PS> import-module CVE-2021-3438_detection.ps1
PS> Get-VulnerablePrinter
```
or
```
PS> . .\CVE-2021-3438_detection.ps1
PS> Get-VulnerablePrinter
```

## Sources
* https://support.hp.com/us-en/document/ish_3900395-3833905-16/hpsbpi03724
* https://securitydocs.business.xerox.com/wp-content/uploads/2021/05/cert_Security_Mini_Bulletin_XRX21K_for_B2XX_PH30xx_3260_3320_WC3025_32xx_33xx.pdf
