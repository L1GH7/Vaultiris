# CVE-2020-14181
Affected versions of Atlassian Jira Server and Data Center allow an unauthenticated user to enumerate users via an Information Disclosure vulnerability in the /ViewUserHover.jspa endpoint. This vulnerability was discovered by Mikhail Klyuchnikov of Positive Technologies.

Affected versions:

* version < 7.13.16
* 8.0.0 ≤ version < 8.5.7
* 8.6.0 ≤ version < 8.12.0

Fixed versions:

* 7.13.16
* 8.5.7
* 8.12.0


POC For CVE-2020-1481 - Jira Username Enumerator/Validator


Usage:
*python3 cve-2020-14181.py -u 'https://jira.domain.xyz' -w '/path/to/wordlist'*

optional for writing to an outfile:
*python3 cve-2020-14181.py -u 'https://jira.domain.xyz' -w'/path/to/wordlist' -o '/path/to/outfile'*
