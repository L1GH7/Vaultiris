# RocaCmTest
Windows tool that analyzes your TPM and certificates for Infineon weak RSA keys (CVE-2017-15361) 

The tool is directly inspired by this GitHub project (https://github.com/crocs-muni/roca) and uses its detection logic. This project just represents a more user-friendly and comprehensive version of detecting the vulnerability. It re-uses and expands the project structure and the detection implementation.

This tool is a windows executable console application that upon execution tests your TPM chip and certificates in user and system store.  

There are 2 phases of testing carried on. First the tool attempts to generate RSA 2048 key using your TPM chip. This is done via employing either MS Platform Crypto Provider (TPM wrapper) on Win 8 and newer OR via Charismathics TPM Software Stack on Windows 7. As soon as the RSA 2048 key is generated its public part is checked against CVE-2017-15361. The key is then deleted. In second phase depending on parameters the tool inspect a single certificate, directory of certificates or if no parameter is provided user & system certificate stores. 

The most common scenario is executing the test by double-clicking the RocaCmTest.exe. This results in testing the user & system certificate stores. The console window will remain opened till ENTER is pressed. Launching this tool from cmdline instead will not prompt for the keypress so that you can dump the console output to a file like "RocaCmTest.exe > result.txt". 

Note that during phase 2 all certificates are analyzed. This means that you can also use it to evaluate certificates comming from any other source (smart cards, USB Tokens, HSM, etc...) against this issue! 

Resulting summary is displayed with issues in red and warnings in yellow.

Requirements: .NET 4 is required, BouncyCastle Dll library

The vulnerability is described here: https://crocs.fi.muni.cz/public/papers/rsa_ccs17

Infineon info is here: https://www.infineon.com/cms/en/product/promopages/tpm-update/?redirId=59160
