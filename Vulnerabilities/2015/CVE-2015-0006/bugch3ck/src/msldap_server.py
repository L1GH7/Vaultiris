
from SocketServer import TCPServer, StreamRequestHandler, ThreadingMixIn
from msldap_utils import *
from ntlm import *

PORT = 389

class LDAPServer(ThreadingMixIn, TCPServer):

	def __init__(self, host):
		TCPServer.__init__(self, (host,PORT), LDAPHandler)

	def server_activate(self):
		print '[*] ldap server: starting tcp service on %s:%d' % (self.server_address[0],self.server_address[1])
		TCPServer.server_activate(self)

	def shutdown(self):
		print '[*] ldap server: shutting down...'
		TCPServer.shutdown(self)

	def verify_request(self, request, client_address):
		print "[+] ldap server: connection from %s:%d" % (client_address[0],client_address[1])
		return True


class LDAPHandler(StreamRequestHandler):

	def handle(self):

		socket = self.request

		while (True):

			data = self.request.recv(65507) # max UDP payload
			req = ldap_request_parse(data)
			res = None

			if req != 0:
				print "[*] ldap server: received request with (messageID=%s)" % (req['messageID'])

				if req['protocolOp']['bindRequest'] != None:
					print "[*] ldap server: parsed bindRequest"
					res = process_bind_request(req);
				elif req['protocolOp']['searchRequest'] != None:
					print "[*] ldap server: parsed searchRequest"
					res = process_search_request(req);
				elif req['protocolOp']['unbindRequest'] != None:
					print "[*] ldap server: parsed unbindRequest"
					socket.close()
					break
				else:
					raise Exception('ldap server: request type not implemented')

				if res != None:
					data = ldap_response_encode(res)
					socket.sendall(data)
			else:
				print "[-] ldap server: request not parsed (may be SASL buffer)"
				# act like it is an unbindRequest
				socket.close()
				break


def process_search_request(req):

	res = [ \
		ldap_search_result_entry(req['messageID']), \
		ldap_search_result_done(req['messageID'], 0) \
	]

	return res

def process_bind_request(req):

	sasl_mech = req['protocolOp']['bindRequest']['authentication']['sasl']['mechanism']

	print "[*] ldap server: received bindRequest with sasl method %s" % (sasl_mech)

	if sasl_mech == 'NTLM':

		sasl_creds = str(req['protocolOp']['bindRequest']['authentication']['sasl']['credentials'])

		msg = parse_ntlm(sasl_creds)

		if msg != None:
			if msg.type == 1:
				res = [ \
					ldap_bind_response_type1(req['messageID'], 'saslBindInProgress') \
				]
			elif msg.type == 3:
				res = [ \
					ldap_bind_response_type3(req['messageID'], 0) \
				]
		else:
			raise Exception('ldap server: could not parse ntlm message')

	elif sasl_mech == 'GSS-SPNEGO':

		sasl_creds = str(req['protocolOp']['bindRequest']['authentication']['sasl']['credentials'])

		msg = parse_ntlm(sasl_creds)

		if msg != None:
			if msg.type == 1:
				res = [ \
					ldap_bind_response_type1(req['messageID'], 'saslBindInProgress') \
				]
			elif msg.type == 3:
				res = [ \
					ldap_bind_response_type3(req['messageID'], 0) \
				]
		else:
			raise Exception('ldap server: could not parse ntlm message')
	else:
		raise Exception('ldap server: unknown sasl mechanism (%s)' % (sasl_mech))


	return res

if __name__ == "__main__":
	
	HOST, PORT = '0.0.0.0', 389
	server = LDAPServer((HOST, PORT), LDAPHandler)
	server.serve_forever()

