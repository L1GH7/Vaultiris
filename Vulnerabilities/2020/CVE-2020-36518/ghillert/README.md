# Reproduction of CVE-2020-36518 in Spring Boot 2.5.10

Execute:

```bash
./mvnw clean verify
```

It will fail with:

```
[ERROR] Failed to execute goal org.owasp:dependency-check-maven:7.0.0:check (default) on project jackson-demo:
[ERROR]
[ERROR] One or more dependencies were identified with vulnerabilities that have a CVSS score greater than or equal to '7.0':
[ERROR]
[ERROR] jackson-databind-2.12.6.jar: CVE-2020-36518(7.5)
[ERROR]
[ERROR] See the dependency-check report for more details.
[ERROR]
```
