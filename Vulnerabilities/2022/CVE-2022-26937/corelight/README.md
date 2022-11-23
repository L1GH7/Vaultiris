# CVE-2022-26937

A package to detect CVE-2022-26937, a vulnerability in Microsoft's NFS implementation.

## Example

You can run this logic on the included PCAP in the `testing\traces` directory:

```
$ zeek -Cr CVE-2022-26937-exploited.pcap ~/Source/CVE-2022-26937/scripts/__load__.zeek 
$ cat notice.log 
#separator \x09
#set_separator	,
#empty_field	(empty)
#unset_field	-
#path	notice
#open	2022-05-11-16-42-00
#fields	ts	uid	id.orig_h	id.orig_p	id.resp_h	id.resp_p	fuid	file_mime_type	file_desc	proto	note	msg	sub	src	dst	p	n	peer_descr	actions	email_dest	suppress_for	remote_location.country_code	remote_location.region	remote_location.city	remote_location.latitude	remote_location.longitude
#types	time	string	addr	port	addr	port	string	string	string	enum	enum	string	string	addr	addr	port	count	string	set[enum]	set[string]	interval	string	string	string	double	double
1652285129.626881	Ci4lmM2HkJESnOzn6g	fe80::88d1:4bb:492e:b104	49798	fe80::1550:7290:1622:4dce	111	-	-	-	tcp	CVE202226937::CVE_2022_26937_Attempt	Potential NFS CVE-2022-26937 exploit attempt: fe80::1550:7290:1622:4dce attempted exploit against fe80::88d1:4bb:492e:b104	-	fe80::88d1:4bb:492e:b104	fe80::1550:7290:1622:4dce	111	-	-	Notice::ACTION_LOG	(empty)	3600.000000	-	-	-	-	-
#close	2022-05-11-16-42-00
```
