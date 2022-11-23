# CVE-2016-5640
Crestron AirMedia AM-100 RCE (CVE-2016-5640) Metasploit Module

Module for exploiting a Remote Command Injection vulnerability in the wireless diagnostics page for Crestron AirMedia AM-100 devices with a firmware version <1.4.0.13. Commands execute as the account running the service (i.e. usually root). An older exploit I worte a module for because I wanted experience writing checks and using the cmdstager .

All credit for the original exposure and writeup of the vulnerabilities should go to Cylance, I guess: https://github.com/CylanceVulnResearch/disclosures/blob/master/CLVA-2016-05-001.md
