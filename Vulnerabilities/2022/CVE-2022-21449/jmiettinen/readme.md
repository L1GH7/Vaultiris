CVE-2022-21449 Vulnerability tester
--------------

# Introduction
There's a new [CVE-2022-21449](https://neilmadden.blog/2022/04/19/psychic-signatures-in-java/) that had bug in ECDSA
signature verification. It's one of the algorithms used with JWT for example. Citing the blog post
> If you have deployed Java 15, Java 16, Java 17, or Java 18 in production then you should stop what you are doing and
> immediately update to install the fixes in the April 2022 Critical Patch Update.

# Running
```bash
./gradlew package && java -jar target/vulntest.jar
```
Will state `VULNERABLE` in case the JVM you used to run it is vulnerable to the signature verification bug and
`not vulnerable` otherwise.
Your JVM vendor and version will also be printed just to be sure that