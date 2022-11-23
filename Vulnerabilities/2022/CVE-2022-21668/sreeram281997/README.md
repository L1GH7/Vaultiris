# CVE-2022-21668-Pipenv-RCE-vulnerability


##   1.  Introduction

In this document the Pipenv vulnerability(CVE-2022-21668) is discussed in detail. The following sections cover the bug code in pipenv/utils.py file which opens up the door for various RCE attacks and also about the recent fix which validates the SSL/TLS connection and check the hostname against a list of trusted hosts. 

##  1.1. List item

Pipenv
Pipenv is a tool that automatically creates and manages a virtualenv for your projects, as well as adds/removes packages from your Pipfile as you install/uninstall packages.

For more infor [ click here ](https://pypi.org/project/pipenv/)


## 1.2 requirements.txt

We need a requirements.txt file whenever we are dealing with complex systems. This textfile also comes handy when we want to port the application to another environment.This file holds all the packages/ dependencies required to run an application successfully along with their version numbers.

Sample requirements.txt file:

![requirements sample](https://github.com/sreeram281997/CVE-2022-21668-Pipenv-RCE-vulnerability/blob/main/requirements.JPG)

Image source: https://www.idkrtm.com/what-is-the-python-requirements-txt/ 

If we don't have a requirements.txt file then it may not be possible to port our application successfully to another system, as there could be a compatability issue between the package versions.

## 2. CVE-2022-21668
Starting with Pipenv version 2018.10.9 and prior to version 2022.1.8, a flaw in pipenv's parsing of requirements files allows an attacker to insert a specially crafted string inside a comment anywhere within a requirements.txt file, which will cause victims who use pipenv to install the requirements file to download dependencies from a package index server controlled by the attacker. RCE attack is highly feasible against such highly vulnerable systems.

cvedetails rate this vulnerability to be **critical / severe**. 

CVSS score from cvedetials:
![CVSS Score](https://github.com/sreeram281997/CVE-2022-21668-Pipenv-RCE-vulnerability/blob/main/cvss.png)
(Source: https://www.cvedetails.com/cve/CVE-2022-21668/)
## 3. Code Bug
![CVE-2022-21668 Bug](https://github.com/sreeram281997/CVE-2022-21668-Pipenv-RCE-vulnerability/blob/main/parse-index.png)
This function is called iteratively on each line of a requirements file, and uses the argparse module to find and process --index-url, --extra-index-url, and --trusted-host options
However, it does not ignore these options when they appear in comments.  The problem with the code is it will parse the strings that starts with "--index-url, --extra-index-url, and --trusted-host" anywhere in the file. This functionality makes it more vulnerable.
For More information [click here](https://github.com/pypa/pipenv/security/advisories/GHSA-qc9x-gjcv-465w) 

## 4. RCE Exploit
Remote code execution (RCE) attacks allow an attacker to remotely execute malicious code on a computer. The impact of an RCE vulnerability can range from malware execution to an attacker gaining full control over a compromised machine.

If an attacker is able to hide a malicious  `--index-url`  option in a requirements file that a victim installs with pipenv, the attacker can embed arbitrary malicious code in packages served from their malicious index server that will be executed on the victim's host during installation (remote code execution/RCE). Exploitation using this technique would be relatively simple to achieve for an attacker with basic knowledge of Python, as the attacker can simply build a source distribution for any of the packages specified in the requirements file, and embed arbitrary malicious code in the setup.py file. When pip installs from a source distribution, any code in the setup.py is executed by the install process. 

For more details [click here](https://github.com/pypa/pipenv/security/advisories/GHSA-qc9x-gjcv-465w)


## 5. Fix
Version 2022.1.8 and above have this issue fixed. A new function “get_host_and_port(url)” is added in the pipenv/utils.py file. This function retrieves the host and port of the url submitted in requirements.txt file. The retrieved host address is parsed using urllib3 which internally validates the SSL/TLS connection. The host address is also checked against a list of trusted host address.
![Fix for CVE-2022-21668](https://github.com/sreeram281997/CVE-2022-21668-Pipenv-RCE-vulnerability/blob/main/parse_index_fix.png)
More Info [here](https://github.com/pypa/pipenv/commit/439782a8ae36c4762c88e43d5f0d8e563371b46f)

## 6. Impact

Basic attacks might use the initial RCE triggered when a victim installs the attacker's malicious package to steal credentials from the victim's host, leach the host's resources to mine cryptocurrency, or install exploit kits or other malware. More sophisticated attackers may use more advanced techniques to persist access to the victim's host, hide or remove evidence of their attack by deleting references to the malicious index server in the Pipfile and Pipfile.lock generated by pipenv or other potential indicators of compromise. Highly sophisticated attackers could attempt to pivot to additional targets from the initial compromised host, and might leverage any exposed credentials in the compromised host environment or implicit authorization granted to the host to gain privileged access to other systems or resources, such as source repositories or package registries. (Source: https://github.com/pypa/pipenv/security/advisories/GHSA-qc9x-gjcv-465w)

## 7. Summary

 - A parsing error which doesn’t consider parsing the comments properly,
   has created a critical vulnerability.
   
  
 - List item With such vulnerability in hand even setting up the malicious index
   server to serve compromised package versions is relatively simple,
   even for a non-sophisticated attacker.

## Additional Resources

 - [FEDORA FEDORA-2022-508e460384](https://lists.fedoraproject.org/archives/list/package-announce@lists.fedoraproject.org/message/KCROBYHUS6DKQPCXBRPCZ5CDBNQTYAWT/)
 - [FEDORA FEDORA-2022-77ce20f03a](https://lists.fedoraproject.org/archives/list/package-announce@lists.fedoraproject.org/message/QHQRIWKDP3SVJABAPEXBIQPKDI6UP7G4/)
 - [Merge pull request from GHSA-qc9x-gjcv-465w](https://github.com/pypa/pipenv/commit/439782a8ae36c4762c88e43d5f0d8e563371b46f)
 - [Fixed Release](https://github.com/pypa/pipenv/releases/tag/v2022.1.8)
 - [Pipenv's requirements.txt parsing allows malicious index url in comments](https://github.com/pypa/pipenv/security/advisories/GHSA-qc9x-gjcv-465w)
