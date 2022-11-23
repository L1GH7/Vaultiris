# CVE-2018-15727

Here I wrote a MSF Module for [CVE-2018-15727](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-15727) which is an auth bypass for Grafana with LDAP/OAuth authentification enabled. It's already integrated in the [metasploit-framework](https://github.com/rapid7/metasploit-framework/blob/master/modules/auxiliary/admin/http/grafana_auth_bypass.py). This module generates a remember me cookie for a valid username. Through unpropper seeding while userdate are requested from LDAP or OAuth it's possible to craft a valid remember me cookie. This cookie can be used for bypass authentication for everyone knowing a valid username.

## Vulnerbale Versions

- 2.x
- 3.x
- 4.x befroe 4.6.4
- 5.x before 5.2.3

## References

- [@Sebastian Solnica](https://twitter.com/lowleveldesign?lang=en): Original discoverd.
- [Mitre](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2018-15727)
- [Grafana fix](https://grafana.com/blog/2018/08/29/grafana-5.2.3-and-4.6.4-released-with-important-security-fix/)

