# CVE-2020-14066
# Icewarp Email Server 12.3.0.1 insecure_permissions
#https://nvd.nist.gov/vuln/detail/CVE-2020-14066

## Introduction :
### firs step: Login to account and upload malicious file via attachments menu. 
![alt text](https://github.com/pinpinsec/CVE-2020-14066/blob/main/unrestric1.png)

### second step: Click to download file and capture this request with burp suit.
![alt text](https://github.com/pinpinsec/CVE-2020-14066/blob/main/unrestric2.png)

### third step: Send download link to victim.

### forth step: Victim download malicious file from web server with out any authentication.
![alt text](https://github.com/pinpinsec/CVE-2020-14066/blob/main/unrestric3.png)
