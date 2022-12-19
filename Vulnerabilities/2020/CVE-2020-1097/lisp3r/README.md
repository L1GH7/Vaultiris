# CVE-2020-10977 read and execute

## About CVE-2020-10977

- HackerOne Report: https://hackerone.com/reports/827052
- Exploit-DB: https://www.exploit-db.com/exploits/48431
- How to reproduce excecution part manually: [From reading to execution](from-reading-to-execution.md) 

## About this repository

- `get_secret.py` - main script. It uses thewhiteh4t's code to exploit cve-2020-10977 at the first time and hook a `secret_key_base` from given repository. Then it launch `cookie_maker.sh` to generate cookie with payload.

      Usege: python get_secret.py http://gitlab.vh foo gfhjkm123

- `cookie_maker.sh` - lauchs docker and generates malicious cookie. Can be used standalone.

      Usage: cookie_maker.sh <secret_key_base> "echo /etc/passwd > /tmp/owned"

### Dependencies

- Docker

### Submodules

- [cve-2020-10977](cve-2020-10977/cve_2020_10977.py) - submodule by [thewhiteh4t](https://github.com/thewhiteh4t/cve-2020-10977)

## Creds

Based on thewhiteh4t's repository: https://github.com/thewhiteh4t/cve-2020-10977

## Warning

It ~~can~~ should contain bugs. If `get_secret.py` ended up correctly but no cookies it output - run it again.