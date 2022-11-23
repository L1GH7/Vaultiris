# **CVE-2022-29932 - Primeur Spazio MFT - Information Disclosure (Memory Leak)**

### Description
The HTTP Server in PRIMEUR SPAZIO 2.5.1.954 (Managed File Transfer) allows a remote unauthenticated attacker to obtain sensitive data (related to the content of transferred files) via a crafted HTTP request.

Vendor has acknowledged the vulnerability and promptly notified all impacted customers and provided a patch.

### Affected Product Code Base
Primeur Spazio - 2.5.1.954.

Other versions may also be affected.

### Attack Vectors
In order to exploit the vulnerability, an unauthenticated attacker should send crafted HTTP Request

### Discovered by
Andrea Mattiazzo, Alessandro Cudini, Antonio Montesano, Emanuele Chiossi, Giovanni Battista Colonna

### Reference
https://www.primeur.com/managed-file-transfer

#
#


## **Proof-of-Concept (POC)**

Navigating the website without any kind of credentials and executing fuzzing on the root directory, it has been observed that some web resources were built at real-time.

<img width="220" alt="1" src="https://user-images.githubusercontent.com/104774764/166418582-42228277-413e-4860-abba-66317948e76d.png">

After an in-depth analysis it was observed those resources (i.e. folders) are strictly related to already existent data directories (i.e. subfolders of root web folder) on the web server.

<img width="336" alt="2" src="https://user-images.githubusercontent.com/104774764/166418621-7c562c9c-8782-42be-8771-7c81dd39e3e3.png">

Creating a crafted HTTP request pointing to these data directories cutting the trailing "/" character, the web server answers with an unmanaged memory leak in HTTP response.
As an example we created two folders on the webserver called "assets" and "pippo" in order to reproduce the unsecure behaviour obtaining the memory leak.
 
<img width="587" alt="3" src="https://user-images.githubusercontent.com/104774764/166418638-6ff2fd14-8447-48c5-b15a-e356de71a12e.png">

<img width="624" alt="4" src="https://user-images.githubusercontent.com/104774764/166418651-37bab3d9-e399-47dd-b2b8-9bf1a2e851fc.png">

By downloading or navigating the resource containing the memory leak, it has been possible to gain access to the exfiltrated buffer of memory which contained the content of files exchanged by authorized users who are using the File Transfer solution, hence leading to an information disclosure.
  
<img width="826" alt="5" src="https://user-images.githubusercontent.com/104774764/166418677-61782c6a-4272-4e66-a360-302fffd1cb21.png">

Since folders name is usually easy-enumerable by any fuzzer, and some folders have standard naming (errors, templates, images), the bug can be easily exploited.
