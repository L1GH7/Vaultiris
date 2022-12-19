# Icewarp Email Server 12.3.0.1 insecure_permissions
#https://nvd.nist.gov/vuln/detail/CVE-2020-14066

## Introduction :
### firs step: Login to account and upload malicious file via attachments menu. 
![alt text](https://github.com/networksecure/icewarp_insecure_permissions/blob/master/unrestric2.png)

### second step: Click to download file and capture this request with burp suit.
![alt text](https://github.com/networksecure/icewarp_insecure_permissions/blob/master/unrestric3.png)

### third step: Send download link to victim.

### forth step: Victim download malicious file from web server with out any authentication.
![alt text](https://github.com/networksecure/icewarp_insecure_permissions/blob/master/unrestric4.png)
