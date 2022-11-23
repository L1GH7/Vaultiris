# CVE-2020-24955
### **SUPERAntiSpyware Professional X Trial <= 10.0.1206 Local Privilege Escalation**

SUPERAntiSpyware Professional X Trial versions prior to 10.0.1206 are vulnerable to local privilege escalation because it allows unprivileged users to restore quarantined files to a privileged location through a NTFS directory junction. 

**Home Page:** https://www.superantispyware.com/

**Proof of Concept**
1. Place a dll payload in an empty folder
2. Scan the payload with the  SUPERAntiSpyware Professional X Trial in order to get it detected. 
3. Once it is detected and moved to quarantine, create a NTFS directory junction.
4. Restore the payload and reboot the system.

**Full PoC video:** https://www.youtube.com/watch?v=jdcqbev-H5I

**Timeline:**
* **16-07-2020** - Vulnerability discovered 
* **16-07-2020** - Notified the vendor via support form (vendor did not response)
* **19-07-2020** - Notified the vendor via email (vendor did not response)
* **25-07-2020** - Vulnerability report to CERT/CC (VRF#20-07-GBPVY)
* **25-08-2020** - Vulnerability Disclosed
* **01-09-2020** - CVE Assigned

**References:**
https://bogner.sh/2017/11/avgater-getting-local-admin-by-abusing-the-anti-virus-quarantine/
