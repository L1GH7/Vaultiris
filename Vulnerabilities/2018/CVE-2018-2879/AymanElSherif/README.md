# Oracle Access Manager (OAM) Authentication Bypass Exploit

### Introduction
Exploiting Oracle Access Manager (OAM) padding oracle vulnerability (CVE-2018-2879) to perform authentication bypass and login to any web app protected by OAM using valid username. 
<br /><br />This exploit is based on OAM padding oracle vulnerability discovered by SEC Consult and was tested on OAM v12.2.1.3.0

```
https://www.sec-consult.com/en/blog/2018/05/oracle-access-managers-identity-crisis/
```

### Dependencies

```
pip install urllib3 paddingoracle requests
```
### Syntax
```
# python oam-auth-bypass.py -h
                                                                                                                                                                                                                                                                                 
 $$$$$$\   $$$$$$\  $$\      $$\        $$$$$$\              $$\     $$\                 $$$$$$$\
$$  __$$\ $$  __$$\ $$$\    $$$ |      $$  __$$\             $$ |    $$ |                $$  __$$\
$$ /  $$ |$$ /  $$ |$$$$\  $$$$ |      $$ /  $$ |$$\   $$\ $$$$$$\   $$$$$$$\            $$ |  $$ |$$\   $$\  $$$$$$\   $$$$$$\   $$$$$$$\  $$$$$$$\
$$ |  $$ |$$$$$$$$ |$$\$$\$$ $$ |      $$$$$$$$ |$$ |  $$ |\_$$  _|  $$  __$$\           $$$$$$$\ |$$ |  $$ |$$  __$$\  \____$$\ $$  _____|$$  _____|
$$ |  $$ |$$  __$$ |$$ \$$$  $$ |      $$  __$$ |$$ |  $$ |  $$ |    $$ |  $$ |          $$  __$$\ $$ |  $$ |$$ /  $$ | $$$$$$$ |\$$$$$$\  \$$$$$$\
$$ |  $$ |$$ |  $$ |$$ |\$  /$$ |      $$ |  $$ |$$ |  $$ |  $$ |$$\ $$ |  $$ |          $$ |  $$ |$$ |  $$ |$$ |  $$ |$$  __$$ | \____$$\  \____$$\
 $$$$$$  |$$ |  $$ |$$ | \_/ $$ |      $$ |  $$ |\$$$$$$  |  \$$$$  |$$ |  $$ |$$\       $$$$$$$  |\$$$$$$$ |$$$$$$$  |\$$$$$$$ |$$$$$$$  |$$$$$$$  |
 \______/ \__|  \__|\__|     \__|      \__|  \__| \______/    \____/ \__|  \__|\__|      \_______/  \____$$ |$$  ____/  \_______|\_______/ \_______/
                                                                                                   $$\   $$ |$$ |
                                                                                                   \$$$$$$  |$$ |
                                                                                                    \______/ \__|


                                                                                                OAM Authentication Bypass Exploit
                                                                                                            Developed by: Ayman ElSherif


usage: oam-auth-bypass.py [-h] [-a <agentid>] [-p <prefix>] [-e <Clear-text>]
                          [-d <Cipher-text>] [-i <username>] [-z <authid>]
                          [-c <cookie>] [-v]
                          url

positional arguments:
  url                   URL of a resource protected by OAM (Oracle WebGate)

optional arguments:
  -h, --help            show this help message and exit
  -a <agentid>, --agentid <agentid>
                        Agent ID for Oracle Web Gateway to use
  -p <prefix>, --prefix <prefix>
                        Prefix: a valid base64 encoded encquery value with
                        last block starts with a space character
  -e <Clear-text>, --encrypt <Clear-text>
                        Clear-text value to encrypt
  -d <Cipher-text>, --decrypt <Cipher-text>
                        Cipher-text value to decrypt
  -i <username>, --impersonate <username>
                        Username to create a login cookie for
  -z <authid>, --authid <authid>
                        Authorization ID
  -c <cookie>, --cookie <cookie>
                        A valid OAM authentication cookie
  -v, --verbose         Verbose output



```

### Decrypting OAMAuthnCookie cookie
![Alt text](example/01-decrypt.png?raw=true)
<br />
### Generating OAMAuthnCookie for admin user
![Alt text](example/02-impersonate.png?raw=true)
<br />
### Encrypting new OAMAuthnCookie cookie
![Alt text](example/03-encrypt.png?raw=true)
<br />
