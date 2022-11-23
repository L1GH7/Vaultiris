# CVE-2021-3130

[Suggested description]

Within the Open-AudIT up to 4.0.2 application, the web interface hides SSH secrets, Windows passwords, and SNMP strings from users using HTML 'password field' obfuscation. By using Developer tools or similar, it is possible to change the obfuscation so that the credentials are visible.

[Vulnerability Type]

Insufficiently Protected Credentials

[Vendor of Product]

Open-AudIT by Opmantek

[Affected Product Code Base]

Open-AudIT up to version 3.5.3.

[Attack Type]

Remote

[Impact Code execution]

false

[Has vendor confirmed or acknowledged the vulnerability?]

true

[Reference]

https://opmantek.com/network-discovery-inventory-software/

[Discoverer]

Vsevolod Shamov (Jet Infosystems (jet.su), Moscow, Russia)
