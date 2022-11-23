# CVE-2019-6693
Decrypt FortiGate configuration secrets

[Original advisory](https://medium.com/@bart.dopheide/decrypting-fortigate-passwords-cve-2019-6693-1239f6fd5a61)
[Vendor advisory](https://www.fortiguard.com/psirt/FG-IR-19-007)

For Fortigate VM/appliances below versions 6.2.0, 6.0.0 to 6.0.6, 5.6.10 configuration secrets are stored encrypted with a unique key.
For versions above the non-default ```private-data-encryption``` parameter lets the user use a custom key. This is rarely used, even today.

This [script](./fortigate_decrypt.py) decrypts secrets from dumped configurations.
