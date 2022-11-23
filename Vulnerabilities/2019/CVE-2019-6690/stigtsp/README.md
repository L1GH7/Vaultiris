# CVE-2019-6690: Improper Input Validation in python-gnupg 0.4.3

We discovered a way to inject data trough the passphrase property of the
gnupg.GPG.encrypt() and gnupg.GPG.decrypt() methods when symmetric encryption is
used.

The supplied passphrase is not validated for newlines, and the library passes
`--passphrase-fd=0` to the gpg executable, which expects the passphrase on the
first line of stdin, and the ciphertext to be decrypted or plaintext to be
encrypted on sebsequent lines.

By supplying a passphrase containing a newline an attacker can control/modify
the ciphertext/plaintext being decrypted/encrypted.

# Vulnerable

python-gnupg 0.4.3, and maybe earlier versions

# Mitigation

Users should upgrade to 0.4.4

# Timeline

- 2019-01-19: Vulnerability discovered during Insomni'hack teaser 2019
- 2019-01-20: PoC created
- 2019-01-22: Applied for CVE, Vendor notified
- 2019-01-23: CVE-2019-6690 assigned
- 2019-01-23: Vendor responded, fix committed
- 2019-01-24: Vendor released 0.4.4

# References

- https://pypi.org/project/python-gnupg/
- https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-6690
- https://github.com/hackeriet/CVE-2019-6690-python-gnupg-vulnerability
- https://ctftime.org/task/7458

# Proof of Concept

Hypothetical application using sucessful decryption of data to authenticate
a user, and a way to exploit it is available here: 

https://github.com/hackeriet/CVE-2019-6690-python-gnupg-vulnerability


## Dependencies 

Debian: `apt install libmojolicious-perl python3-gnupg python3-flask`

Nix: `nix-shell`

## Run the server

`./server.py`

## Run the exploit

`./exploit.pl`

# Credits

Vulnerability discovered by Alexander Kjäll and Stig Palmquist. 

Thanks to remmer.
