# CVE-2020-14064
# Icewarp Email Server 12.3.0.1 incorrect_access_control
https://nvd.nist.gov/vuln/detail/CVE-2020-14064

## Introduction :
### first step:  Login to your account and then send request to delete whole inbox and capture this request with Burp suit. (security is attacker account)
![alt text](https://github.com/pinpinsec/CVE-2020-14064/blob/main/incorrect1.png)

### second step: Sniff your local network, may be your office and find a ice warp account and its SID.

### third step:  Replace your SID and username with victim SID and username and then send the request. (security2 is victim account)
![alt text](https://github.com/pinpinsec/CVE-2020-14064/blob/main/incorrect2.png)

result: victim's Inbox has been deleted.
