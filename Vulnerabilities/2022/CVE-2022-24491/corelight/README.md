CVE-2022-24491
=================================

A Zeek detector for CVE-2022-24491:

http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-24491

This detector will generate a notice if it sees a portmap set
and dump, an action performed when exploiting CVE-2022-24491.

Example:

```
#separator \x09
#set_separator	,
#empty_field	(empty)
#unset_field	-
#path	notice
#open	2022-04-13-20-55-55
#fields	ts	uid	id.orig_h	id.orig_p	id.resp_h	id.resp_p	fuid	file_mime_type	file_desc	proto	note	msg	sub	src	dst	p	n	peer_descr	actions	email_dest	suppress_for	remote_location.country_code	remote_location.region	remote_location.city	remote_location.latitude	remote_location.longitude
#types	time	string	addr	port	addr	port	string	string	string	enum	enum	string	string	addr	addr	port	count	string	set[enum]	set[string]	interval	string	string	string	double	double
1649715078.678609	C4J4Th3PJpwUYZZ6gc	192.168.88.146	63839	192.168.88.157	111	-	-	-	udp	CVE202224491::POTENTIAL_CVE_2022_24491	Possible CVE-2022-24491 exploit attempt.  An RPC portmap set with a RPC portmap dump was observed.	-	192.168.88.146	192.168.88.157	111	-	-	Notice::ACTION_LOG	(empty)	3600.000000	-	-	-	-	-
#close	2022-04-13-20-55-55
```