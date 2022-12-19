# Pyrescom-Termod-PoC
This is the Proof-of-concept exploit code for three vulnerabilities discovered by Jonas Mattsson and Hugo van den Toorn at [Outpost24](https://outpost24.com/) Ghost Labs. These vulnerabilities are discovered in the web server component on a [Pyrescom Termod4](https://pyres.com/en/solutions/termod-4/) time control machine. These vulnerabilities can be chained and allow a remote attacker to bypass authentication, read files from the local filesystem and execute arbitrary commands. The following CVE IDs can be used to reference to these vulnerabilities: CVE-2020-23160, CVE-2020-23161 and CVE-2020-23162.

![](img.png)

# CVE Details
These vulnerabilities are identified by three CVE IDs, described below.

## CVE-2020-23160 - Remote code execution
Remote code execution in Pyrescom Termod4 time management devices before 10.04k allows authenticated remote attackers to arbitrary commands as root on the devices. [See MITRE CVE](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-23160)

## CVE-2020-23161 - Local file inclusion
Local file inclusion in Pyrescom Termod4 time management devices before 10.04k allows authenticated remote attackers to traverse directories and read sensitive files via the Maintenance > Logs menu and manipulating the file-path in the URL. [See MITRE CVE](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-23161)

## CVE-2020-23162 - Sensitive information disclosure & weak encryption
Sensitive information disclosure and weak encryption in Pyrescom Termod4 time management devices before 10.04k allows remote attackers to read a session-file and obtain plain-text user credentials. [See MITRE CVE](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-23162)

# Proof-of-Concept
Chained attack against Termod4 devices, allowing an unauthenticated attacker to:

* Obtain plain credentials by reading and decrypting exposed session information (CVE-2020-23161 & CVE-2020-23162).
* Perform remote code execution (RCE) as root (CVE-2020-23161).
* Take screenshots of the active device's current physical screen (potential eavesdropping on user-interaction on physical device, untested).


## Installation
Clone this repository, (optionally) setup a Python virtual environment and install dependencies. Assuming you have Python3 and python3-venv installed (`apt-get install python3-venv`).

0. (optional) Setup virtualenv called 'Pyrescom-CVE': `python3 -m venv Pyrescom-CVE`, change dir `cd Pyrescom-CVE` and activate it `. bin/activate`.

1. Clone the repository into the current directory: 
```sh
git clone https://github.com/Outpost24/Pyrescom-Termod-PoC.git .
```

2. Install dependencies with Python-pip: 
```sh
python -m pip install -r requirements.txt
```

3. You should now be able to the actual script:
```sh
python Pyrescom-poc.py
```

## Usage
Basic usage of this PoC is as follows:

* Basic usage, obtains plain credentials: `python Pyrescom-poc.py http://<target-URI or IP>`
* Take screenshot of webserver interface `python Pyrescom-poc.py http://<target-URI or IP> -s`
* Execute command `python Pyrescom-poc.py http://<target-URI or IP> <command>`
* Help `python Pyrescom-poc.py -h`

## License
Distributed under the GPL3 License. See `LICENSE` for more information.

# Additional information

## Further reading
Details on these vulnerabilities and how they were discovered can be found on our [blog - Tales from the frontline: Outsmarted by smart devices](https://outpost24.com/blog/Tales-from-the-frontline-outsmarted-by-smart-devices). 

## Remediation
The vulnerabilities are fixed, exact details on the patch versions are unknown and not acknowledged by the vendor. We discovered the vulnerabilities in version 10.02r and verify that in version 10.04k the vulnerabilities are remediated. How intermediate releases are affected is unknown, therefore we recommend upgrading to the latest stable firmware version available for your device.

## Disclosure timeline

|   Day #    |    Date    |                                                            Activity                                                             |
| ---------- | ---------- | ------------------------------------------------------------------------------------------------------------------------------- |
| 0-day	     | 18-12-2019 | 	Initial contact with vendor.                                                                                                |
| 2 days	 | 20-12-2019 | 	Second attempt at contact with vendor.                                                                                      |
| 21 days	 | 08-01-2020 | 	Third attempt at contact with vendor.                                                                                       |
| 29 days	 | 16-01-2020 | 	Vendor meeting set-up through our customer.                                                                                 |
| 40 days	 | 27-01-2020 | 	Meeting with vendor and our customer, vendor acknowledged issues and received full write-up.                                |
| 82 days	 | 09-03-2020 | 	Update released and rolled out, performed brief re-test for our customer. Vulnerabilities resolved.                         |
| 83 days	 | 10-03-2020 | 	Confirmed patch effectiveness with vendor. Proposed disclosure timeline, no response from vendor.                           |
| 124 days   | 20-04-2020 | 	Proposed disclosure timeline, no response from vendor.                                                                      |
| 141 days   | 07-05-2020 | 	CVE reservation requested. More than 100 days since confirmation by vendor.                                                 |
| 401 days   | 22-01-2021 | 	Multiple requests towards Mitre for update/CVE in past 204 days. After re-submission CVE IDs were released on January 22nd. |
| 408 days   | 29-01-2021 | 	Release of this blog, CVE IDs and PoC.                                                                                      |

Information our responsible disclosure policy can be found [on our website](https://outpost24.com/policies/responsible-disclosure).

## Contact

Jonas Mattsson - [LinkedIn](https://www.linkedin.com/in/jonas-mattsson-b7447891/)

Hugo van den Toorn - [@HugovdToorn](https://twitter.com/HugovdToorn) - [LinkedIn](https://www.linkedin.com/in/hugovdt/)

## Acknowledgement
Many thanks to Jonas who has the great habbit of finding the most awesome findings in the weirdest of places. Unfortunately for us, today marks your last day at Outpost24. So we would like to celebrate your departure with these three CVEs and we wish you all the best in your future endeavours.

`"That might have been a bit out of scope..."`

From Ghostlabs with love <3
