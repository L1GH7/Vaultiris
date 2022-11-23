# CVE-2019-3980
CVE-2019-3980 exploit written in win32/c++ (openssl dependency). Port of https://github.com/tenable/poc/blob/master/Solarwinds/Dameware/dwrcs_dwDrvInst_rce.py

## Instructions
This PoC requires staticly linked openssl libraries. Place openssl includes in third_party\include\openssl and libs in third_party\libs\openssl\$(PlatformShortName)\

Rough build instructions for openssl:
* Install strawberry perl - https://strawberryperl.com/
* Install NASM - https://www.nasm.us/pub/nasm/stable/win64/
* From x64 Native Tools Command Prompt
```
cd c:\path\to\openssl\src
perl Configure VC-WIN64A no-shared --prefix=C:\path\to\build_directory
nmake
nmake install
 ```
IMPORTANT: The copy of openssl in this project is modified to allow unsafe prime numbers in the diffie-hellman function. 512 bit changes to 128. If building manually you will need to find and change the DH_MIN_MODULUS_BITS in dh_local.h to 128. (#define DH_MIN_MODULUS_BITS     128)

## Usage
```
-t <target ip>
-e <path to exe>
-p <port number (default: 6129>
```
