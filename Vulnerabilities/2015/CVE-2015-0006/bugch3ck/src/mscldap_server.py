
from SocketServer import UDPServer, BaseRequestHandler, ThreadingMixIn
from mscldap_utils import *

PORT = 389

class CLDAPServer(ThreadingMixIn, UDPServer):

	def __init__(self, host):
		UDPServer.__init__(self, (host,PORT), CLDAPHandler)

	def server_activate(self):
		print '[*] cldap server: starting udp service on %s:%d' % (self.server_address[0],self.server_address[1])
		UDPServer.server_activate(self)

	def shutdown(self):
		print '[*] cldap server: shutting down...'
		UDPServer.shutdown(self)

	def verify_request(self, request, client_address):
		print "[+] cldap server: connection from %s:%d" % (client_address[0],client_address[1])
		return True

class CLDAPHandler(BaseRequestHandler):

	def handle(self):

		data = self.request[0]
		socket = self.request[1]
		
		req = parse_cldap_req(data)

		print "[*] cldap server: received request with (messageID=%s)" % (req['messageID'])

		try:
			dnsDomain = str(req['protocolOp'][0]['filter']['and'][0]['equalityMatch']['assertionValue'])

			if dnsDomain[-1] == '.':
				dnsDomain = dnsDomain[:-1] # remove last dot

			print "[*] cldap server: message is a LDAP Ping for %s" % (dnsDomain)

		except:
			print "[-] cldap server: could not parse dnsDomain from request"
			dnsDomain = None

		attrs = {}
		if dnsDomain != None:
			attrs['Domain'] = dnsDomain

		data = format_cldap_res_netlogon(req['messageID'],attrs)

		socket.sendto(data, self.client_address)

if __name__ == "__main__":
	
	HOST, PORT = '0.0.0.0', 389
	server = CLDAPServer((HOST, PORT), CLDAPHandler)
	server.serve_forever()

