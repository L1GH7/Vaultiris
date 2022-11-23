# Summary

Exploits CVE-2020-10759 - `fwupd` PGP signature verification bypass. See
<https://github.com/justinsteven/advisories/blob/master/2020_fwupd_dangling_s3_bucket_and_CVE-2020-10759_signature_verification_bypass.md>
for more details.

# Requirements

Note: You need to install `python3-gpg` from your OS vendor. This module
doesn't like being installed via `pip` because it needs to match your system's
installation of `libgpgme`.

```
apt install python3-flask python3-gpg python3-lxml
```
