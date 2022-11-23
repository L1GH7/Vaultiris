# CVE-2020-11492

Proof-of-Concept (PoC) for Docker Desktop for Windows privilege escalation vulnerability. This vulnerability was
patched in Docker version 2.3.0.2 on May 11th, 2020.

This PoC performs the following:

- creates a named pipe mimicking docker named pipe `\\.\\pipe\\dockerLifecycleServer`,
- call `ImpersonateNamedPipeClient` after connection from docker service,
- retrieve and duplicate the impersonated access token from the current thread,
- launch a new process with the token. The new process will run as `SYSTEM`.

# References
- https://www.pentestpartners.com/security-blog/docker-desktop-for-windows-privesc-cve-2020-11492/
