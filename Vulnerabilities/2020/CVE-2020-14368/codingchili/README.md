# CSWSH-THEIA-CVE-2020-14368

- Report target: Eclipse CHE deployment available on che.openshift.io
- Vulnerability type: Cross-site websocket hijack
- Discovery date: 2020-04-08
- Author: Robin Duda (codingchili@github)

## Summary

The /services websocket endpoint in Eclipse CHE adn Theia is vulnerable to cross-site websocket hijacking.
This vulnerability affects Eclipse CHE servers that uses cookie or basic authentication as the websocket
connection doesn't perform any cross-site checks or in-channel authentication, the browser automatically
includes any credentials when connecting from third-party domains. The attack works just like a cross-site
request forgery attack, except it is much more powerful as it grants an attacker two-way communicaiton.

Read more about CSWSH here: https://portswigger.net/web-security/websockets/cross-site-websocket-hijacking
