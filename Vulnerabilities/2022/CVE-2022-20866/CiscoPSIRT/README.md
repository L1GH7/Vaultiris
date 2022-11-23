# RSA Key Checker for CVE-2022-20866
A [vulnerability in the handling of RSA keys](https://tools.cisco.com/security/center/content/CiscoSecurityAdvisory/cisco-sa-asaftd-rsa-key-leak-Ms7UEfZz) on devices running the Cisco Adaptive Security Appliance Software and Firepower Threat Defense Software could allow an unauthenticated, remote attacker to retrieve the RSA private key. This vulnerability is due to a logic error when the RSA key is stored in memory on a hardware platform that performs hardware based cryptography. An attacker could exploit this vulnerability by using a Lenstra side-channel attack against the targeted device. A successful exploit could allow the attacker to retrieve the RSA private key. 

The following conditions could be observed in a vulnerable device:

- The RSA key could be malformed and invalid. The malformed RSA key is not functional and a TLS client connection to a Cisco ASA or FTD device that uses the malformed RSA key will result in a TLS signature failure, which means a vulnerable software release created an invalid RSA signature that fails verification. If an attacker obtains the RSA private key, such key could be used to impersonate a Cisco ASA or FTD device.
- The RSA key could be valid but have specific characteristics that make it vulnerable to the potential leak of the RSA private key. If an attacker obtains the RSA private key, such key could be used to impersonate a Cisco ASA or FTD device. Please see the [security advisory Indicators of Compromise section](https://tools.cisco.com/security/center/content/CiscoSecurityAdvisory/cisco-sa-asaftd-rsa-key-leak-Ms7UEfZz) for more information on the detection of this type of RSA key.

This tool allows you to check the private RSA key in the Cisco ASA or FTD devices.

The Cisco security advisory is available at the following link:
https://tools.cisco.com/security/center/content/CiscoSecurityAdvisory/cisco-sa-asaftd-rsa-key-leak-Ms7UEfZz﻿

## How to Use the RSA Key Checker Tool
The purpose of the `key_check.py` tool is to analyze keys exported from ASA or FTD for susceptibility to the "Cisco RSA Private Key Leak Vulnerability (CVE-2022-20866)".

### System requirements

The `key_check.py` is a community supported tool that runs in the linux shell environment and has the following dependencies:
- Python 3
- OpenSSL command line tool

### Supported platforms
This tool is designed to be platform agnostic, and has been tested on the following platform configurations:

- MacOS Monterey 12.4, LibreSSL 2.8.3
- MacOS Catalina 10.15.7, LibreSSL 2.8.3
- Red Hat Enterprise Linux 7.6 (Maipo), OpenSSL 1.0.2k-fips
- Red Hat Enterprise Linux 9, OpenSSL 3.0.1
- Red Hat Enterprise Linux 8, OpenSSL 1.1.1k
- Ubuntu 16.04, OpenSSL 1.0.2g
- Ubuntu 18.04, OpenSSL 1.1.1
- Ubuntu 20.04 LTS, OpenSSL 1.1.1f
- Ubuntu 22.04 LTS, OpenSSL 3.0.2

### Tool command interface

Syntax:
```
    key_check.py [-h] --pkcs12 PKCS12 [--passwd PASSWD]
```

Parameters:

```
    -h 
        display help information
    --pkcs12 
        The name of the file containing the key to analyze in base64
     	encoded PKCS12 format.  This file is exported from the ASA
        or FTD device using the procedure described below.
    --passwd 
        The password required to decrypt the PKCS12.  This is the 
        same value that was entered on the ASA or FTD CLI while 
        exporting the key.  If this option is not specified, the 
        tool will prompt for the password.
```

### Key export on ASA

This section describes how to export one key from the ASA device.  This 
procedure will need to be followed for each key that is to be analyzed.
There are 2 scenarios:
    
1) The keypair is used in an ASA trustpoint certificate

Trustpoints let you manage and track certificate authorities (CAs) and certificates. A trustpoint is a representation of a CA or identity pair. A trustpoint includes the identity of the CA, CA-specific configuration parameters, and an association with one, enrolled identity certificate. If the keypair is used in a certificate that is configured on ASA, then
the keypair can be exported by entering:

```
# asa(config)# crypto ca export <tpname> pkcs12 test
```
where <tpname> is the name of the trustpoint.   

2) The keypair is not used in an ASA trustpoint certificate 

If the key is not present in a certificate that is configured on ASA,
then a temporary trustpoint needs to be created and enrolled with a 
self-signed certificate.

Assume keyname is the name of the keypair that is to be exported.
Assume temptp is a name used for a temprorary trustpoint

```
# asa(config)# crypto ca trustpoint temptp
# asa(config-ca-trustpoint)# keypair keyname
# asa(config-ca-trustpoint)# subject-name cn=temp
# asa(config-ca-trustpoint)# no serial-number
# asa(config-ca-trustpoint)# enroll self
# asa(config-ca-trustpoint)# crypto ca enroll temp noconfirm
```

Now generate the PKCS12:
```
# asa(config)# crypto ca export xtp pkcs12 test 
```
The two procedures above (1, 2) will dump a base64 encoded p12 with 
password "test" to the console.  Place all of the output lines into
a text file.  This file will be used as input to the tool.  

### Key export on FTD

On FTD devices, the procedure requires the use of FMC as follows.  Note 
that keys are not independently manageable on FTD, so all keys should have 
an associated certificate.

- Go to devices / certificates
- Click the export icon for desired certificate under the desired device.
- Select the PKCS12 radio button
- Enter a passphrase in both boxes and press OK
- Download the PKCS12 file

### Tool input

The following is included for the purpose of visual validation
of what the contents of an exported PKCS12 file should look as follows:

```
-----BEGIN PKCS12-----
MIIJvwIBAzCCCXUGCSqGSIb3DQEHAaCCCWYEggliMIIJXjCCA78GCSqGSIb3DQEH
BqCCA7AwggOsAgEAMIIDpQYJKoZIhvcNAQcBMBwGCiqGSIb3DQEMAQMwDgQIwsRZ

             <lines omitted>

geRPZxLUjt8nsc0bbrmEkXJ/27EbUm43UkioncY7JJwwQTAxMA0GCWCGSAFlAwQC
AQUABCAMZ7IR6nXHNbZ5zdtVAQX90VZyu2pvnJpiYgWWEf+WngQI9H/zlSTGKeAC
AggA
-----END PKCS12-----
```
### Running the script

The use of the tool is shown through the following examples which 
demonstrate all possible outcomes.

1) Normal valid key
```
linux>  ./key_check.py --passwd test --pkcs12 valid_normal.p12.b64

The RSA key is valid.
```

2) Valid key with exposure characteristics

```
linux> ./key_check.py --passwd test --pkcs12 valid_short.p12.b64
```

The RSA key is valid but is vulnerable to exposure if used in
product versions that are affected by the Cisco Private Key Leak
Vulnerability (CVE-2022-20866).  If this was done, this key should
no longer be used.

3) Invalid key with no vulnerability characteristics
```
linux> ./key_check.py --passwd test --pkcs12 inval_not_vulnerable.p12.b64
```
The RSA key is invalid due to the Cisco RSA Private Key Leak
Vulnerability (CVE-2022-20866) but does not have known exposure
characteristics. It is recommended that this key be replaced.

4) Invalid vulnerable key
```
linux> ./key_check.py --passwd test --pkcs12 inval_vulnerable.p12.b64
```
The RSA key is invalid and vulnerable to exposure due to the
Cisco RSA Private Key Leak Vulnerability (CVE-2022-20866).
This key should no longer be used.

5) Invalid password
```
linux> ./key_check.py --passwd test2 --pkcs12 valid_normal.p12.b64

ERROR:  Mac verify error: invalid password?
```
6) Invalid input file
```
linux> ./key_check.py --passwd test --pkcs12 bad_file.txt

ERROR:  139710534743952:error:0D07207B:asn1 encoding routines:ASN1_get_object:header too long:asn1_lib.c:157:
```
7) Valid but non-RSA key
```
linux> ./key_check.py --passwd test --pkcs12 ecdsa.p12.b64

ERROR:  140300676220816:error:0607907F:digital envelope routines:EVP_PKEY_get1_RSA:expecting an rsa key:p_lib.c:287:
```

                 
