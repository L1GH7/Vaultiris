# CVE-2022-22828 Synametrics - SynaMan version 4.9

## Summary
Viden Labs has found an Insecure Direct Object Reference (IDOR) vulnerability in Synametric's SynaMan. This vulnerability is present in versions 4.9 and earlier.

This vulnerability has since been patched in version 5.0.

“Insecure direct object references (IDOR) are a type of access control vulnerability that arises when an application uses user-supplied input to access objects directly.” - https://portswigger.net/web-security/access-control/idor. 

The vulnerability can be exploited resulting in an access to (potentially sensitive) files in a SynaMan user’s home folder (including subfolders), that they do not have authorisation to access. 


## Scenario Setup

A SynaMan user named “internaluser”. This user was AD joined but doesn’t make a difference if they are not AD joined. 

In internaluser’s home folder, there are two files: 
- file-download.docx 
- secret.txt 

<img src="https://user-images.githubusercontent.com/27877740/151269955-3cc1f76d-e860-4b03-b84f-1d5ef078ff36.png" width=80% height=80%>
<p><i>Figure 1: User Directory with Files</i></p>

A download link has been created for file-download.docx and shared to a simulated external party. 

<img src="https://user-images.githubusercontent.com/27877740/151270760-308b546c-bad7-4229-b45e-3b1aeb0ce9b6.png" width=80% height=80%>
<p><i>Figure 2: SynaMan File Download Page</i></p>



## Test Environment
The web application test was performed from the 10.0.2.0/24 network, using Burp Suite on Kali. 

The Windows Server 2019 instance is running an out of the box configuration, with the latest security updates applied, and port 443 inbound open. 

SynaMan was configured in accordance with best practices, following guidance from the Synametrics Technologies website. 

Outlined in Figure 3 below is an overview of the network setup for the lab test. 

<img src="https://user-images.githubusercontent.com/27877740/151271316-072a5864-348b-412b-8173-284aa05650f8.png" width=80% height=80%>
<p><i>Figure 3: SynaMan Testing Topology</i></p>

|Computer|Role|OS|Browser|
|--------|----|---|-------|
|10.0.2.30|Web Application Testing Server|Kali Linux (VM)|Burp Suite embedded browser|
|10.0.3.50|SynaMan Server|Windows Server 2019 (VM)|N/A|

<p><i>Table 1 - Additional setup information</i></p>


## Vulnerability Finding

A vulnerability was found when an external party accesses a download link, either shared with them, or found by googling "pubopp?". The file download hyperlink contains a base64 encoded file name, including the path. This path can be modified to download other files from the user’s folder that have not been shared. While this requires an attacker to correctly guess a filename, tools such as Burp Suite make this brute-forcing trivial and automated. A graphical representation of this is outlined in Figure 4 below.  

<img src="https://user-images.githubusercontent.com/27877740/151272114-83077f68-23d9-448d-96c4-2fe40c6139fd.png" width=80% height=80%>
<p><i>Figure 4: Graphical representation of GET request vulnerability</i></p>

Shown below in Figure 5 is the resulting GET request when the download link is clicked. 

<img src="https://user-images.githubusercontent.com/27877740/151272146-2380489f-a927-41f6-97af-33ba5bbb3fa9.png">
<p><i>Figure 5: Burp Suite GET request</i></p>

Inspection of the base64 encoded filename (passed as the parameter named “fn”) shows the file location. 

<img src="https://user-images.githubusercontent.com/27877740/151272184-adf47e5a-1b98-452b-b0c8-6bd5b4d1f75f.png">
<p><i>Figure 6: Burp Suite Base64 filename</i></p>

Using Burp Suite, this can be modified to another file name, and substitute this in the GET request. For this example, we will use “secret.txt”, as we know this file exists in the folder. 

<img src="https://user-images.githubusercontent.com/27877740/151272210-98e9ac2a-9f3a-485d-b3ce-387016eb5111.png">
<p><i>Figure 7: Burp Suite Modified Base64 filename</i></p>

<img src="https://user-images.githubusercontent.com/27877740/151272232-70109e90-2170-4b41-81aa-2fe1f0097311.png">
<p><i>Figure 8: Burp Suite Modified GET Request</i></p>

Submitting the modified GET request allows us to download the file which was not shared, as shown in Figure 9 below. 

<img src="https://user-images.githubusercontent.com/27877740/151272262-02976e23-10d4-42d4-afb4-678eb8a64507.png">
<p><i>Figure 9: SynaMan Unauthorised Download</i></p>

The currently available security settings in SynaMan do not provide a control to mitigate this attack, however all downloaded files are logged. 
