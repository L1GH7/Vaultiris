# CVE-2017-8809 - RFD(Reflected File Download) for MediaWiki

> A remote user can create a specially crafted URL for the target site that, when loaded by the target user, will cause the 'api.php' script to download a file containing shell commands [CVE-2017-8809]. The file will be served by the target site.

## Environment

- Google Chrome 79.0
- MediaWiki 1.29.1

## Using

1. Run
```
$ docker-compose up
```

2. Access trap page (`http://127.0.0.1:8080/poc.html`)  

3. Click "`Click here`"  

## Note

- If change container port, edit `$wgServer` variable in `mediawiki/LocalSettings.php`.
- MediaWiki Account `admin / pass1234`.

## References

- [NVD - CVE-2017-8809](https://nvd.nist.gov/vuln/detail/CVE-2017-8809)
- [MediaWiki Multiple Flaws Let Remote Users Modify Data, Obtain Potentially Sensitive Information, and Conduct Cross-Site Scripting Attacks and Let Local Users Obtain Passwords - SecurityTracker](https://securitytracker.com/id/1039812)
- [⚓ T128209 Reflected File Download from api.php](https://phabricator.wikimedia.org/T128209)
- Fix commit
  - [SECURITY: API: Avoid some silliness with browser-guessed filenames · wikimedia/mediawiki@66b21e0](https://github.com/wikimedia/mediawiki/commit/66b21e0d7df586afd40ad6c97f10ec7deafd9d39)
- [714373 - Ignore &lt;a download&gt; for cross origin URLs - chromium - An open-source project to help move the web forward. - Monorail](https://bugs.chromium.org/p/chromium/issues/detail?id=714373)

