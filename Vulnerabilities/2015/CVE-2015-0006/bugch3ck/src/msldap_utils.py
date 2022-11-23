
from pyasn1.codec.ber import decoder
from pyasn1.type.univ import *
from uuid import UUID
from struct import unpack
from proto.ldap import *
from ntlm import *

def ldap_request_parse(data):

	msg = decoder.decode(data, asn1Spec=LDAPMessage())[0]

	return msg

def ldap_search_result_entry(msgId):

	msg = LDAPMessage() \
		.setComponentByName('messageID', msgId) \
		.setComponentByName('protocolOp', Choice() \
			.setComponentByPosition(2, SearchResultEntry() \
				.setComponentByName('objectName', '') \
				.setComponentByName('attributes', PartialAttributeList() \
					.setComponentByPosition(0, Sequence() \
						.setComponentByPosition(0, AttributeDescription('supportedSASLMechanisms')) \
						.setComponentByPosition(1, SetOf() \
							.setComponentByPosition(0, LDAPString('GSS-SPNEGO'))
						)
					)
				)
			)
		)

	return msg
	
def ldap_search_result_done(msgId, status):

	msg = LDAPMessage() \
		.setComponentByName('messageID', msgId) \
		.setComponentByName('protocolOp', Choice() \
			.setComponentByPosition(2, SearchResultDone() \
				.setComponentByName('resultCode', 'success') \
				.setComponentByName('matchedDN', '') \
				.setComponentByName('errorMessage', '')
			)
		)

	return msg

def ldap_bind_response_type1(msgId, status):

	info = [
		AvPair(MsvAvNbDomainName, "TEST"),
		AvPair(MsvAvNbComputerName, "TEST"),
		AvPair(MsvAvDnsDomainName, "test"),
		AvPair(MsvAvDnsComputerName, "test"),
		AvPair(MsvAvDnsTreeName, "test"),
		AvPair(MsvAvTimestamp, datetime.datetime.utcnow()),
		AvPair(MsvAvEOL)
	]

	ntlm_type2 = NtlmChallenge('TEST', targetinfo=info)

	saslCreds = ntlm_type2.encode()

	msg = LDAPMessage() \
		.setComponentByName('messageID', msgId) \
		.setComponentByName('protocolOp', Choice() \
			.setComponentByPosition(2, BindResponse() \
				.setComponentByName('resultCode', status) \
				.setComponentByName('matchedDN', '') \
				.setComponentByName('errorMessage', '') \
				.setComponentByName('serverSaslCreds', saslCreds)
			)
		)

	return msg

def ldap_bind_response_type3(msgId, status):

	msg = LDAPMessage() \
		.setComponentByName('messageID', msgId) \
		.setComponentByName('protocolOp', Choice() \
			.setComponentByPosition(2, BindResponse() \
				.setComponentByName('resultCode', status) \
				.setComponentByName('matchedDN', '') \
				.setComponentByName('errorMessage', '') \
				.setComponentByName('serverSaslCreds', '')
			)
		)

	return msg


def ldap_response_encode(res):

	buf = ''
	for msg in res:
		buf = buf + encoder.encode(msg)

	return buf

