# eWeLink mobile Application - Incorrect Access Control Vulnerability (CVE-2020-12702)
Weak encryption in the Quick Pairing mode in the eWeLink mobile application (Android application V4.9.2 and earlier, iOS application V4.9.1 and earlier) allows physically proximate attackers to eavesdrop on Wi-Fi credentials and other sensitive information by monitoring the Wi-Fi spectrum during the pairing process.

# Vulnerability Exploitation Script
eWeLinkESPT is a python3 script that exploits the CVE-2020-12702 vulnerability. eWeLinkESPT automatically decodes and decrypts the WiFi network credentials transmitted by the *eWeLink* mobile application during a *Quick Pairing* association process. In particular, this tool reverses the *eWeLink* implementation of the *ESP Touch* protocol, which is used by-default in the WiFi pairing of the ITEAD SONOFF devices, and many other ESP-based IoT appliances.

### Requirements
```sh
python3, tshark, pyshark
```
### Use 
Run in a terminal:
```sh
$ sudo ./eWeLinkESPT.py
```
then lauch a "Quick Pairing" association through the eWeLink mobile application.

### Todo List
 - Loop cyclically over all the WiFi channels
 - Improve code styling

# Tested Versions
 This tool has been tested with the Android (v4.9.2 and ealier) and iOS (v4.9.1 and earlier) versions of the *eWeLink* mobile application.

# Disclosure Timeline
- Feb 02, 2020: Report submitted to Coolkit, the company behind eWeLink.
- May 05, 2020: No acknowledge received, second report submitted to Coolkit.
- May 09, 2020: Received acknowledge from Coolkit CTO, stating that they were deprecating the ESP Touch pairing.
- Nov 09, 2020: Disclosing the vulnerability @CpsIoTSec2020 Conference [1]
- Feb 23, 2021: The protocol is still supported, publishing the CVE-2020-12702 ref.

# References
[1] Salzillo, Giovanni, and Massimiliano Rak. "A (in) Secure-by-Design IoT Protocol: the ESP Touch Protocol and a Case Study Analysis from the Real Market." In Proceedings of the 2020 Joint Workshop on CPS&IoT Security and Privacy, pp. 37-48. 2020.

License
----

MIT
