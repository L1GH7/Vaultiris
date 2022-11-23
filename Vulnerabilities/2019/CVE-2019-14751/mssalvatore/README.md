# CVE-2019-14751_PoC
A Proof of Concept for CVE-2019-14751


## Vulnerability Description

NLTK Downloader before version 3.5 is vulnerable to a directory traversal,
allowing attackers to write arbitrary files via a ../ (dot dot slash)
in an NLTK package (ZIP archive) that is mishandled during extraction.

For more information see
https://salvatoresecurity.com/zip-slip-in-nltk-cve-2019-14751/.


## Steps to Reproduce

1. Place index.xml and zip-slip.zip in a directory where they will be served by
   a web server. Adjust permissions to allow the files to be served.

1. Run NLTK Downloader
    ```
    $> python3
    >>> import nltk
    >>> nltk.download()
    ```

1. Change the value in the "Server Index" field to point to the index.xml
   from step 1

1. Click "Download" to install the malicious package

1. Check for the existence of "/tmp/evil.txt".
    ```
    $> cat /tmp/evil.txt
    This is an evil file
    $>
    ```


## Remediation

This vulnerability is fixed in NLTK version 3.4.5 and later. The following commit
resolves the vulnerability:
https://github.com/nltk/nltk/commit/f59d7ed8df2e0e957f7f247fe218032abdbe9a10
