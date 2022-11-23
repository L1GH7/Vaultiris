keypair vulnerable keys (CVE-2021-41117)
========================================

Keys generated with versions of the keypair javascript library
vulnerable to CVE-2021-41117.

Due to bugs in the random number generator this library will
generate certain keys with higher likelyhood. The likelyhood of
generating one of the keys in this repo with a vulnerable version
is around 70%.


t1
--

t1 contains the 256 most common keys. The likelyhood of generating
one of these keys is around 33%.

t2
--

t2 contains 71424 keys that are less common than the keys in t1, but
still appear relatively often. The likelyhood of generating one of
these keys is around 37%.

misc
----

This collection was created by [Hanno Böck](https://hboeck.de) for the
[badkeys](https://badkeys.info) project.

This work was funded in part by Industriens Fond through the CIDI project
(Cybersecure IOT in Danish Industry) and in part by the
[Center for Information Security and Trust (CISAT)](https://cist.dk/)
at the IT University of Copenhagen, Denmark.
