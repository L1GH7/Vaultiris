# CVE-2020-1971
CVE-2020-1971 Auto Scan &amp; Remote Exploit Script. Auto Local Scan & Patch Script.

## Currently in development
- [x] Patched Source Code of OpenSSL 1.1.1i
- [x] Install from source/Update .deb Script  ~~Currently in development 5:38pm~~ Completed 5:50pm
- [x] Install from source/Update (Windows) ~~Currently in development 6:06pm~~ Completed 6:40pm
- [x] Self-Extracting EXEs **Download here:** [32-bit](https://github.com/MBHudson/CVE-2020-1971/blob/main/Win32OpenSSL_Light-1_1_1i.exe)/[64-bit](https://github.com/MBHudson/CVE-2020-1971/blob/main/Win64OpenSSL_Light-1_1_1i.exe)
- [ ] Autoscan for vulnerability/Remote Exploit scripts (Local)
- [ ] Autoscan for vulnerability/Remote Exploit scripts (Remote)

---

### December 9Th 2020

5:03pm

I am current developing the scripts and hope to have everything out by tonight. For now this repository only contains the patched OpenSLL 1.1.1i Source. Currently I am working on an Install/Update patch script.

---

5:50pm

Debian/Ubuntu Linux installation script uploaded. For install instructions see "**Linux Installation**"

---

6:40pm
1. Windows installation from source instructions under **Windows Installation From Source Code**
1. Self-Installer 32/64 bit uploaded
        **Download here:** [32-bit](https://github.com/MBHudson/CVE-2020-1971/blob/main/Win32OpenSSL_Light-1_1_1i.exe)/[64-bit](https://github.com/MBHudson/CVE-2020-1971/blob/main/Win64OpenSSL_Light-1_1_1i.exe)

---

# Vulnerability Description 

The X.509 GeneralName type is a generic type for representing different types of names. One of those name types is known as EDIPartyName. OpenSSL provides a function GENERAL_NAME_cmp which compares different instances of a GENERAL_NAME to see if they are equal or not. This function behaves incorrectly when both GENERAL_NAMEs contain an EDIPARTYNAME. A NULL pointer dereference and a crash may occur leading to a possible denial of service attack. 

**OpenSSL itself uses the GENERAL_NAME_cmp function for two purposes**

1) Comparing CRL distribution point names between an available CRL and a CRL distribution point embedded in an X509 certificate 

2) When verifying that a timestamp response token signer matches the timestamp authority name (exposed via the API functions TS_RESP_verify_response and TS_RESP_verify_token) 

If an attacker can control both items being compared then that attacker could trigger a crash. For example if the attacker can trick a client or server into checking a malicious certificate against a malicious CRL then this may occur. 

Note that some applications automatically download CRLs based on a URL embedded in a certificate. This checking happens prior to the signatures on the certificate and CRL being verified. OpenSSL's s_server, s_client and verify tools have support for the "-crl_download" option which implements automatic CRL downloading and this attack has been demonstrated to work against those tools. 

Note that an unrelated bug means that affected versions of OpenSSL cannot parse or construct correct encodings of EDIPARTYNAME. However it is possible to construct a malformed EDIPARTYNAME that OpenSSL's parser will accept and hence trigger this attack. 

All OpenSSL 1.1.1 and 1.0.2 versions are affected by this issue. Other OpenSSL releases are out of support and have not been checked. Fixed in OpenSSL 1.1.1i (Affected 1.1.1-1.1.1h). Fixed in OpenSSL 1.0.2x (Affected 1.0.2-1.0.2w).

---

# Linux Installation

```sh
sudo chmod +x Linux_Patched_Install_From_Source_Script
```

Run:
```sh
sudo ./Linux_Patched_Install_From_Source_Script
```
---
# Windows Installation From Source Code

 "Native" OpenSSL uses the Windows APIs directly at run time.
 To build a native OpenSSL you can either use:

 Microsoft Visual C++ (MSVC) C compiler on the command line
  
 or
 
 MinGW cross compiler
 run on the GNU-like development environment MSYS2
 or run on Linux or Cygwin

 "Hosted" OpenSSL relies on an external POSIX compatibility layer
 for building (using GNU/Unix shell, compiler, and tools) and at run time.
 For this option you can use Cygwin.

 Visual C++ native builds, aka VC-*
 =====================================

 Requirement details
 -------------------

 In addition to the requirements and instructions listed in INSTALL.md,
 these are required as well:

 - Perl.
   Strawberry Perl, available from <http://strawberryperl.com/>
   Please read NOTES.PERL for more information, including the use of CPAN.
   An alternative is ActiveState Perl, <https://www.activestate.com/ActivePerl>
   for which you may need to explicitly build the Perl module Win32/Console.pm
   via <https://platform.activestate.com/ActiveState> and then download it.

 - Microsoft Visual C compiler.

 - Netwide Assembler (NASM), available from <https://www.nasm.us>
   Note that NASM is the only supported assembler.

 Quick start
 -----------

 1. Install Perl

 2. Install NASM

 3. Make sure both Perl and NASM are on your %PATH%

 4. Use Visual Studio Developer Command Prompt with administrative privileges,
    choosing one of its variants depending on the intended architecture.
    Or run "cmd" and execute "vcvarsall.bat" with one of the options x86,
    x86_amd64, x86_arm, x86_arm64, amd64, amd64_x86, amd64_arm, or amd64_arm64.
    This sets up the environment variables needed for nmake.exe, cl.exe, etc.
    See also
    <https://docs.microsoft.com/cpp/build/building-on-the-command-line>

 5. From the root of the OpenSSL source directory enter
    perl Configure VC-WIN32    if you want 32-bit OpenSSL or
    perl Configure VC-WIN64A   if you want 64-bit OpenSSL or
    perl Configure             to let Configure figure out the platform

 6. nmake

 7. nmake test

 8. nmake install

 For the full installation instructions, or if anything goes wrong at any stage,
 check the INSTALL.md file.

 Installation directories
 ------------------------

 The default installation directories are derived from environment
 variables.

 For VC-WIN32, the following defaults are use:

     PREFIX:      %ProgramFiles(86)%\OpenSSL
     OPENSSLDIR:  %CommonProgramFiles(86)%\SSL

 For VC-WIN64, the following defaults are use:

     PREFIX:      %ProgramW6432%\OpenSSL
     OPENSSLDIR:  %CommonProgramW6432%\SSL

 Should those environment variables not exist (on a pure Win32
 installation for examples), these fallbacks are used:

     PREFIX:      %ProgramFiles%\OpenSSL
     OPENSSLDIR:  %CommonProgramFiles%\SSL

 ALSO NOTE that those directories are usually write protected, even if
 your account is in the Administrators group.  To work around that,
 start the command prompt by right-clicking on it and choosing "Run as
 Administrator" before running 'nmake install'.  The other solution
 is, of course, to choose a different set of directories by using
 --prefix and --openssldir when configuring.

 Special notes for Universal Windows Platform builds, aka VC-*-UWP
 --------------------------------------------------------------------

 - UWP targets only support building the static and dynamic libraries.

 - You should define the platform type to "uwp" and the target arch via
   "vcvarsall.bat" before you compile. For example, if you want to build
   "arm64" builds, you should run "vcvarsall.bat x86_arm64 uwp".
---

# References 

References are provided for the convenience of the reader to help distinguish between vulnerabilities.

https://git.openssl.org/gitweb/?p=openssl.git;a=commitdiff;h=2154ab83e14ede338d2ede9bbe5cdfce5d5a6c9eURL

https://git.openssl.org/gitweb/?p=openssl.git;a=commitdiff;h=2154ab83e14ede338d2ede9bbe5cdfce5d5a6c9eCONFIRM

https://git.openssl.org/gitweb/?p=openssl.git;a=commitdiff;h=f960d81215ebf3f65e03d4d5d857fb9b666d6920URL

https://git.openssl.org/gitweb/?p=openssl.git;a=commitdiff;h=f960d81215ebf3f65e03d4d5d857fb9b666d6920CONFIRM

https://www.openssl.org/news/secadv/20201208.txtURL:https://www.openssl.org/news/secadv/20201208.txtDEBIAN:DSA-4807URL

https://www.debian.org/security/2020/dsa-4807FREEBSD:FreeBSD-SA-20:33URL 

https://security.FreeBSD.org/advisories/FreeBSD-SA-20:33.openssl.asc

---
