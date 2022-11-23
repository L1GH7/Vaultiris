# IMPOSTER

A pentest tool used to attack windows clients on rogue networks.

## Current version

###Features

* Downgrade LDAP to NTLM authentication
* Fake initial steps of an domain controller to fool Network Location Awareness

### Servers
DNS, CLDAP, LDAP

## Setup

The current version have been tested on Kali Linux but should on other operating systems as well.

### Dependencies

* pyasn1
* dnspython

### Quick setup on Kali Linux
Use the following steps to setup imposter on a Kali Linux machine.
```
git clone https://github.com/bugch3ck/imposter.git
cd imposter/src
git clone https://github.com/rthalley/dnspython.git
ln -s dnspython/dns dns
apt-get install pyasn1
```

## Changelog

### Version 0.1
Private release 2014-06-05. Implements DNS, CLDAP and LDAP. Can downgrade LDAP bind to use NTLM and fake successful authentication to trick Network Location Awareness to set the domain policy.
