Zeek test script for CVE-2020-13777
===================================

This script performs a simple test to check if a server is potentially vulnerable to CVE-2020-13777.

CVE-2020-13777 causes GnuTLS to create unencrypted session tickets. This seems to be detectable by checking gnutls sets the ``key_name`` to zero - for which it uses the first 16 bytes of the session-ticket. This script checks if:

* A server sends a suspicious session ticket
* A client resumes a connection to a server succesfully with a suspicious session ticket

Both cases indicate that a server is employing a vulnerable version of GnuTLS.

Found cases are written to notice.log. Example output:

```
1591660828.497706	CHhAvVGS1DHFjwGM9	::1	58800	::1	5556	-	-	-	tcp	CVE_2020_13777::CVE_2020_13777_Server	Server potentially vulnerable to CVE-2020-13777 detected	-	::1	::1	5556	-	-	Notice::ACTION_LOG	3600.000000	-	-	-	-	-
1591660837.423412	ClEkJM2Vm5giqnMf4h	::1	58802	::1	5556	-	-	-	tcp	CVE_2020_13777::CVE_2020_13777_Resumed	Server potentially vulnerable to CVE-2020-13777 detected; client resumed with suspicious session ticket	-	::1	::1	5556	-	-	Notice::ACTION_LOG	3600.000000	-	-	-	-	-
```

Notice are deduplicated and only logged once per default_suppression_interval (default: 1 hour) for each server.

To install via zkg, just use

```
zkg install 0xxon/cve-2020-0601
```

Note:
----
This script only works for TLS 1.2 (or below) connections. It will not alert on vulnerable servers that use TLS 1.3. The 

Disclaimer:
-----------

This script has not been extensively tested. It works with my self-generated test-data and should find vulnerable servers. There is a chance that it will raise false positives with different server software, but this is somewhat unlikely.