# CVE-2020-10977.py
*authenticated arbitrary file read for Gitlab (CVE-2020-10977)*

**usage:** CVE-2020-10977.py [-h] [--verifySSL] [-f FILELIST] host token

Exploit for the gitlab local file inclusion (arbitrary authenticated file read - CVE-2020-10977). modify the variable pullfiles to include all the files you want to get from the host or provide them in a list to the script.

**positional arguments:**
- **host**                  hostname of the gitlab instance, e.g. [https://git.xxx.yyy]
- **token**                 the API access token generated within the gitlab account

**optional arguments:**
- **-h, --help**            show this help message and exit
- **--verifySSL**           use 'cert.pem' in the current working dir to verify the server certificate
- **-f FILELIST, --filelist FILELIST**
                        the relative path to the file containing a list with all files you want to pull via LFE

# Credits
Original Exploit Author: Jasper Rasenberg  
Original Exploit: https://www.exploit-db.com/exploits/49076
