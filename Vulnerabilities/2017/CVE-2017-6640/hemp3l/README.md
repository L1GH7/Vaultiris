# CVE-2017-6640-POC
Proof of concept for CVE-2017-6640 as burp extension

Cisco Prime Data Center Network Manager (DCNM) implements a static credentials. See also: 
https://tools.cisco.com/security/center/content/CiscoSecurityAdvisory/cisco-sa-20170607-dcnm2

More specifically, the Web UI requires users to authenticate using HTTP Digest Auth. This burp extension simply makes use of the hard-coded HA1 and completes the digest auth challenge-response:

```
HA1 = MD5(username:realm:password)
HA2 = MD5(method:digestURI)
response = MD5(HA1:nonce:HA2)
```

## How to use

Load the extension in burp and browse to the Cisco DCNM management web interface. When prompted for credentials, enter whatever. The plugin will complete the authentication. 

Proceed with uploading and deploying an enterprise app. 

## Limitations

This POC does not do quality of protection (QOP).
