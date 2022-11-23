
from pyasn1.codec.ber import decoder
from pyasn1.type.univ import *
from uuid import UUID
from struct import unpack,pack
from proto.cldap import *

def parse_cldap_req(d):

	msg = decoder.decode(d, asn1Spec=CLDAPMessage())[0]

	return msg

#
# http://www.ietf.org/rfc/rfc1035.txt
#
def pack_string(s):
	if len(s)>0:
		return pack('%sp'%(len(s)+2),s)
	else:
		return '\0'

def format_cldap_res_netlogon(msgId, attrs={}, flags=0x000033fd):

	if not 'Domain GUID' in attrs:
		attrs['Domain GUID'] = UUID('{12345678-1234-5678-1234-567812345678}')

	if not 'Domain' in attrs:
		attrs['Domain'] = 'example.com'

	if not 'Forest' in attrs:
		attrs['Forest'] = attrs['Domain']

	if not 'Hostname' in attrs:
		attrs['Hostname'] = 'fakedc.example.com'

	if not 'NetBIOS Domain' in attrs:
		attrs['NetBIOS Domain'] = 'EXAMPLE'

	if not 'NetBIOS Hostname' in attrs:
		attrs['NetBIOS Hostname'] = 'FAKEDC'

	if not 'Username' in attrs:
		attrs['Username'] = ''

	if not 'Server Site' in attrs:
		attrs['Server Site'] = 'Default-First-Site-Name'

	if not 'Client Site' in attrs:
		attrs['Client Site'] = attrs['Server Site']

	ds = pack_string(attrs['Forest']) \
		+ pack_string(attrs['Domain']) \
		+ pack_string(attrs['Hostname']) \
		+ pack_string(attrs['NetBIOS Domain']) \
		+ pack_string(attrs['NetBIOS Hostname']) \
		+ pack_string(attrs['Username']) \
		+ pack_string(attrs['Server Site']) \
		+ pack_string(attrs['Client Site'])

	attrVal = pack('<HHI',23,0, flags)
	attrVal = attrVal + attrs['Domain GUID'].bytes
	attrVal = attrVal + ds
	attrVal = attrVal + pack('<IHH',5,0xffff,0xffff)

	a = CLDAPMessage() \
		.setComponentByPosition(0, MessageID(msgId)) \
		.setComponentByPosition(2, Choice() \
			.setComponentByPosition(1, SearchResultEntry() \
				.setComponentByPosition(0, LDAPDN('')) \
				.setComponentByPosition(1, PartialAttributeList() \
					.setComponentByPosition(0, Sequence() \
						.setComponentByPosition(0, AttributeDescription('netlogon')) \
						.setComponentByPosition(1, SetOf() \
							.setComponentByPosition(0, AttributeValue(attrVal))
						)
					)
				)
			)
		)

	b = CLDAPMessage() \
		.setComponentByPosition(0, MessageID(msgId)) \
		.setComponentByPosition(2, Choice() \
			.setComponentByPosition(2, SearchResultDone() \
				.setComponentByPosition(0, 0) \
				.setComponentByPosition(1, LDAPDN('')) \
				.setComponentByPosition(2, LDAPString(''))
			)
		)
	
	buf = encoder.encode(a)
	buf = buf + encoder.encode(b)

	return buf

