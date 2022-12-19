#!/usr/bin/env python3

import pwn
import struct as s
from common import info, sendRebootCmd, readResp

from config import *

def main():
    pwn.context.update(os='linux', arch='mips')

    r = pwn.remote(remoteIp, remotePort)

    sendRebootCmd(r)
    readResp(r)

    info("DONE")
    r.close()

if __name__ == "__main__":
    main()
