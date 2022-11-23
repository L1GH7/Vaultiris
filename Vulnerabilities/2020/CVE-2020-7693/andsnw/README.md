# CVE-2020-7693: Meteor <1.10.2 SockJS 0.3.19 Denial of Service POC
### Author: Andrew Snow

[SockJS v0.3.19](https://github.com/sockjs/sockjs-node/issues/252) calls `res.end` instead of `res.write` when receiving websocket upgrade requests. This causes an `Error [ERR_STREAM_WRITE_AFTER_END]: write after end` which crashes the container running the app utilising the vulnerable SockJS.

Vulnerable versions affected:
* Meteor JS <1.10.2 which use SockJS 0.3.19
* SockJS 0.3.19

## Usage
This POC is targeted towards vulnerable MeteorJS apps running SockJS on `/sockjs`. To customise for other web apps running SockJS, change the payloads from `/sockjs` to corresponding routes managed by SockJS.

Install Python, then run in cmd/terminal:
```
pip install requests
python poc.py --target <domain>
```

A demo Meteor app running the vulnerable sockjs has been included. To test the exploit on this demo app, install Meteor from https://www.meteor.com/install and then:
```
cd demo/
meteor
```

And then point the payload target to `http://localhost:3000`

## Remediation
Update SockJS to 0.3.20

## References & CVE
* https://snyk.io/vuln/SNYK-JS-SOCKJS-575261
* https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2020-7693
* https://cwe.mitre.org/data/definitions/400.html
