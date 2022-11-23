# C-Ark Credential Decoder
Exploit tool for **CVE-2021-31796**  
A Tool for decoding C-Ark Credential files  
  
By: Aaron Mizrachi &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- https://twitter.com/unmanarc/  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Enrique Vaamonde - https://twitter.com/_ejvm  
First release: **2/Sep/2019**   
Disclosure: **11/Oct/2021**  

## References
  
- https://packetstormsecurity.com/files/164023/CyberArk-Credential-File-Insufficient-Effective-Key-Space.html
- https://vuldb.com/?id.181904

## Responsible Disclosure:

This vulnerability was pending for release since Sep/2019.

And... here is the timeline:
 
- **2019-08-1x** During some exercise, Our Team discovered and reported to the local vendor representative a potential crypto weakness in some credential storage methods used.
- **2019-08-30** Until this date, we only had a **"ollydbg" in-memory proof of concept** using their own tools. We were trying to make the point of how this can become an attack vector for certain specific situations, but we failed to make the point. So we decided to start coding this proof of concept to have a more obvious argument.
- **2019-09-02** **We successfully implemented** the hashing and the crypto algorithm in **our own proof of concept** (totally detached from the product).
- **2019-09-03** We announced our findings to the vendor and our interest to make this publicly available.
- **2019-09-20** We received a request from the vendor to delay the public release until the issue was fixed.
- **2020-05** We contacted them again to be cleared to release the tool and exchanged a couple of mails saying that they were not ready.
- **2021-09/2021-10** We have found that other unrelated researchers have also recently found and disclosed the very same vulnerability publicly, and given this... we've finally (after 2 years!) been cleared by the vendor to share with you our findings and proof-of-concept tool for exploiting the CreateCredFile crypto-weakness.

## Potential usage:

During a pentest, if someone is smart enough to reach the PSM and accidentally get access to the CredFile, someone can potentially use this file to establish a connection to the Vault and get the whole reign...

To have a countermeasure, most Credentials Files place some "restrictions" to avoid the password to be used in a different environment/computer (Eg. the hacker very own PSM). 

However, those restrictions can be modified *if you reverse and get the raw key portion*. This  decrypted key portion can be used to re-create another file with another "security" parameters (Eg. another host, another application, another OS User).

## Operation Mode

To generate the AES-256 (32 bytes) raw decryption key,  we take a pair of SHA1SUM from the "AdditionalInformation" credential field appending "0x00000000" and "0x00000001" for each Hash; the first Hash provides the first 20 bytes of the key, and the second one only the last 12 bytes. 

If there is any environmental restrictions (like IP/Host/exepath/...), we prepend each plaintext value to AdditionalInformation before taking both SHA1SUMs.

It's important to mention that the "ClientApp" field is transformed with BASE64(SHA1SUM(strlower(ClientApp))) before being prepended to "AdditionalInformation" and generating both SHA1SUMs.

The decryption is done using AES-256-CBC OpenSSL function using the Password or NewPassword field. (https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption)

We are using (verificationflags-16) for figuring which validation/restriction is in place:

    usingClientApp      = ((uVerificationsFlag&0x1) != 0);
    usingAppPath        = ((uVerificationsFlag&0x2) != 0);
    usingClientIP       = ((uVerificationsFlag&0x4) != 0);
    usingOSUser         = ((uVerificationsFlag&0x8) != 0);
    usingClientHostname = ((uVerificationsFlag&0x20) != 0);

and in the case that some restrictions are not displayed in the output credential file, you can always introduce them by hand. I think that we can both agree in that neither "app path" nor "client IP” is a truly random value.

## Mitigation:

Use the HSM \o/, don't store the decryption key in the cred file.


## Build Howto:


```
qmake . 
make -j8
```

