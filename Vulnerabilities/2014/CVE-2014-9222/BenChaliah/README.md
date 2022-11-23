
## Disclaimer

Tutorials are for informational and educational purposes only. I believe that ethical hacking, information security and cyber security should be familiar subjects to anyone using digital information and computers. I believe that it is impossible to defend yourself from hackers without knowing how hacking is done. 

## Requirements


```
$ IDA Pro +7.0 (you can use Radar2 since IDA is quite expensive)
$ Router running Rompager 4.07 : there're immense number of them such as TP-Link TD-W8951*, ZyXEL P-660R, Billion BiPAC 51**/52**
$ UART to USB
```

The misfortune cookie vulnerability has been around for a while but still lacking an analysis which illustrate the techinical details of the vulnerability in public. According to Check Point the affected software is the embedded web server RomPager from AllegroSoft. Internet-wide scans suggest RomPager is likely the most popular web server software in the world with respect to number of available endpoints. RomPager is typically embedded in the firmware released with the device. This specific vulnerability was introduced to the code base in 2002. Moreover, the devices exposed are the one using RomPager services with versions before 4.34 (and specifically 4.07).

> For this tutorial I'll be using a ZXHN H108L that uses Rompager 4.07

```
HTTP/1.1 401 Unauthorized
WWW-Authenticate: Basic realm="ZXHN H108L"
Content-Type: text/html
Transfer-Encoding: chunked
Server: RomPager/4.07 UPnP/1.0
EXT:
```

Now lets use the serial port, first we must determine the Baud rate

```
$ sudo stty -F /dev/ttyUSB0
```
>OUT :
```
speed 115200 baud; line = 0;
-brkint -imaxbel
```
![device](https://benchaliah.github.io/assets/img/posts/device.jpeg)

I use __minicom__, you can use anything from __screen__ to __HyperTerminal__, but I prefer this one for its stability ;)

### You must first deactivate the __Hardware Flow Control__
```
$ sudo minicom -s
```

```
    +-----------------------------------------------------------------------+
    | A -    Serial Device      : /dev/modem                                |
    | B - Lockfile Location     : /var/lock                                 |
    | C -   Callin Program      :                                           |
    | D -  Callout Program      :                                           |
    | E -    Bps/Par/Bits       : 115200 8N1                                |
    | F - Hardware Flow Control : No                                        |
    | G - Software Flow Control : No                                        |
    |                                                                       |
    |    Change which setting?                                              |
    +-----------------------------------------------------------------------+
            | Screen and keyboard      |
            | Save setup as dfl        |
            | Save setup as..          |
            | Exit                     |
            | Exit from Minicom        |
            +--------------------------+
```

Now we're good to go:
```
$ sudo minicom -D /dev/ttyUSB0 -b 115200
```
```
Bootbase Version: VTC_SPI1.26 |  2012/12/26 16:00:00    
RAM: Size = 8192 Kbytes
Found SPI Flash 2MiB GD25Q16 at 0xbfc00000
SPI Flash Quad Enable
Turn off Quad Mode

RAS Version: ZXHN H108LV4.0.0a_ZRQ_MA                                          
System   ID: $2.12.162.0(G94.BY.2)3.20.19.0| 2014/03/21   20140321_v006  | 2014 

Press any key to enter debug mode within 3 seconds.
..........
```

To show possible commands:

`$ ATHE`

```
======= Debug Command Listing =======
AT          just answer OK
ATHE          print help
ATBAx         change baudrate. 1:38.4k, 2:19.2k, 3:9.6k 4:57.6k 5:115.2k
ATENx,(y)     set BootExtension Debug Flag (y=password)
ATSE          show the seed of password generator
ATTI(h,m,s)   change system time to hour:min:sec or show current time
ATDA(y,m,d)   change system date to year/month/day or show current date
ATDS          dump RAS stack
ATDT          dump Boot Module Common Area
ATDUx,y       dump memory contents from address x for length y
ATRBx         display the  8-bit value of address x
ATRWx         display the 16-bit value of address x
ATRLx         display the 32-bit value of address x
ATGO(x)       run program at addr x or boot router
ATGR          boot router
ATGT          run Hardware Test Program
ATRTw,x,y(,z) RAM test level w, from address x to y (z iterations)
ATSH          dump manufacturer related data in ROM
ATDOx,y       download from address x for length y to PC via XMODEM
ATTD          download router configuration to PC via XMODEM
ATUR          upload router firmware to flash ROM

< press any key to continue >
```

This firmware has a set of hidden commands that are very useful to our dynamic analysis, the problem is that we can't see the registers nor set breakpoints but hopefully with these hidden commands we'd have that features, in order to see and use them we need to use __ATEN__. However, this command require a password, so what we can do now? let reverse engineer the firmware to find the password generator's algorithm.

(Static analysis) Let see the firmware file first:

```
$ binwalk ras
```

![binwalk1](https://benchaliah.github.io/assets/img/posts/binwalk1.png)

```
$ dd if=ras of=rom1.lzma bs=1 skip=85043 count=66696
$ dd if=ras of=rom2.lzma bs=1 skip=350259 count=3350556
$ lzma -d <rom1.lzma> rom1
$ lzma -d <rom2.lzma> rom2
```

Let see now the signature in the extracted files:

```
$ binwalk -Y rom1
```

```
DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             MIPS executable code, 32/64-bit, big endian, at least 1250 valid instructions
```

__string__ is a great tool to get an idea of what a binary file is or what it contains, let see if we can find the password algorithm :

```
$ strings rom1 | grep -i aten
$ strings rom2 | grep -i aten
```

Apparently, we can't see it because the firmware is still compressed, which means we must dump it from the router using the debugging mode:

```
$ ATDO bfc00000, 1f0000
```

![xmodem](https://benchaliah.github.io/assets/img/posts/xmodem_2.png)

The first thing that comes in mind is to start looking for a dispatch table. Why? just intuition, the only alternative is if-elif but it'll be a long set of them, in order to check for the validity of the password provided by the user, and here's what I got:


```assembly
ROM:800158A0 CmdTable:       .word unk_80015C20       # DATA XREF: sub_8000E3D8+4Co
ROM:800158A0                                          # sub_8000E3D8+90o ...
ROM:800158A4                 .word sub_8000E6D0
ROM:800158A8                 .byte    0
ROM:800158A9                 .byte    0
ROM:800158AA                 .byte    0
ROM:800158AB                 .byte    0
ROM:800158AC                 .word aJustAnswerOk      # "          just answer OK"
ROM:800158B0                 .word aAthe              # "ATHE"
ROM:800158B4                 .word CMD_SHOW_HELP_ATHE
ROM:800158B8                 .byte    0
ROM:800158B9                 .byte    0
ROM:800158BA                 .byte    0
ROM:800158BB                 .byte    0
ROM:800158BC                 .word aPrintHelp         # "          print help"
ROM:800158C0                 .word aAtba              # "ATBA"
ROM:800158C4                 .word CMD_ATBA
...
ROM:800158D0                 .word aAten              # "ATEN"
ROM:800158D4                 .word CMD_ATEN
```

Keep in mind that MIPS word is 4 bytes, now let see the __CMD_ATEN__:

```assembly
ROM:8000FF84                 lw      $a0, 8($s0)
ROM:8000FF88                 jal     sub_80013654
ROM:8000FF8C                 li      $a1, 0
ROM:8000FF90                 jal     passwd_generator
```

and now: __passwd_generator__

```assembly
ROM:80010A2C passwd_generator:                         # CODE XREF: CMD_ATEN+50p
ROM:80010A2C                 lui     $t9, 0xA11F
ROM:80010A30                 lw      $v1, ptrDO_GTABLE
ROM:80010A34                 lw      $t8, dword_80017050
ROM:80010A38                 lw      $v1, 0x18($v1)   # take the seed part, generated by the ATSE command
ROM:80010A3C                 lbu     $t8, 0x6D($t8)   # take the last byte of the MAC Address
ROM:80010A40                 sll     $v1, 8     # $v1 = v1 << 8
ROM:80010A44                 andi    $t7, $t8, 7    # $t7 = t8 & 0x7
ROM:80010A48                 li      $t8, 0     # $t8 = 0
ROM:80010A4C                 srl     $v1, 8     # $v1 = v1 >> 8 
ROM:80010A50                 li      $t9, 0xA11F5AC6    # $t9 = MAGIC
ROM:80010A54                 j       loc_80010A68
ROM:80010A58                 addu    $t9, $v1, $t9    # $t9 = $v1 + $t9
ROM:80010A5C  # ---------------------------------------------------------------------------
ROM:80010A5C
ROM:80010A5C loc_80010A5C:                              # CODE XREF: passwd_generator+40j
ROM:80010A5C                 sll     $t9, 31      # $t9 = $t9 << 31
ROM:80010A60                 or      $t9, $t6, $t9    # $t9 = $t6 | $t9
ROM:80010A64                 addiu   $t8, 1       # $t8 = $t8 + 1
ROM:80010A68
ROM:80010A68 loc_80010A68:                # CODE XREF: passwd_generator+28j
ROM:80010A68                 sltu    $t6, $t8, $t7    # $t6 = (t8 < t7? 1:0)
ROM:80010A6C                 bnez    $t6, loc_80010A5C    # take jump if $t6 != 0
ROM:80010A70                 srl     $t6, $t9, 1    # $t6 = $t9 >> 1
ROM:80010A74                 jr      $ra      # all done, exit
ROM:80010A78                 xor     $v0, $t9, $v1    # $v0 = $t9 ^ $v1
```
What we can conclude:

* $v1 is the seed value (generated by the ATSE command, zero if ATSE is not used)
* $t8 initially contains the last byte of MAC Address (last octet)
* 0xA11F5AC6 is the magic value we were looking for!
* Those two instructions: "$v1 = v1 << 8" and "$v1 = v1 >> 8" are equal to "seed = seed & 0x00FFFFFF"
* The loop with (sll-or-srl) is nothing more than a ROR instruction (ROR instruction on MIPS is a pseudoinstruction meaning it is typically emulated by using sll-or-srl sequence)
* $v0 is the result

Now let make a C program that does the same thing and compile it:

```C
U32 passwd_generator(U32 seed, U8 last_mac_octet)
{
      U32 b = seed & 0x00FFFFFF;
      U32 r = ROR(b + 0xA11F5AC6, last_mac_octet & 7);
      return r^b;
}
```

Now let run __ATSE__ to get the seed and use it (for instance: I got 088521A78838):

```
$ ./gen_pass 088521A78838
```

Let run the magical command we just hacked :p
```
$ ATEN1, A12F5AC6
```

```
ATEN1, A12F5AC6
OK
ATHE
======= Debug Command Listing =======
AT          just answer OK
ATHE          print help
ATBAx         change baudrate. 1:38.4k, 2:19.2k, 3:9.6k 4:57.6k 5:115.2k
ATENx,(y)     set BootExtension Debug Flag (y=password)
ATSE          show the seed of password generator
ATTI(h,m,s)   change system time to hour:min:sec or show current time
ATDA(y,m,d)   change system date to year/month/day or show current date
ATDS          dump RAS stack
ATDT          dump Boot Module Common Area
ATDUx,y       dump memory contents from address x for length y
ATWBx,y       write address x with  8-bit value y
ATWWx,y       write address x with 16-bit value y
ATWLx,y       write address x with 32-bit value y
ATRBx         display the  8-bit value of address x
ATRWx         display the 16-bit value of address x
ATRLx         display the 32-bit value of address x
ATGO(x)       run program at addr x or boot router
ATGR          boot router
ATGT          run Hardware Test Program
AT%Tx         Enable Hardware Test Program at boot up
ATBTx         block0 write enable (1=enable, other=disable)

< press any key to continue >
ATRTw,x,y(,z) RAM test level w, from address x to y (z iterations)
ATWEa(,b,c,d) write MAC addr, Country code, EngDbgFlag, FeatureBit to flash ROM
ATCUx         write Country code to flash ROM
ATCB          copy from FLASH ROM to working buffer
ATCL          clear working buffer
ATSB          save working buffer to FLASH ROM
ATBU          dump manufacturer related data in working buffer
ATSH          dump manufacturer related data in ROM
ATWMx         set low 6 digits MAC address in working buffer
ATMHx         set hight 6 digits MAC address in working buffer
ATBS          show the bootbase seed of password generator
ATLBx         xmodem upload bootbase,x is password
ATSMx         set 6 digits MAC address in working buffer
ATCOx         set country code in working buffer
ATFLx         set EngDebugFlag in working buffer
ATSTx         set ROMRAS address in working buffer
ATSYx         set system type in working buffer
ATVDx         set vendor name in working buffer
ATPNx         set product name in working buffer
ATFEx,y,...   set feature bits in working buffer
ATMP          check & dump memMapTab
ATDOx,y       download from address x for length y to PC via XMODEM

< press any key to continue >
ATTD          download router configuration to PC via XMODEM
ATUPx,y       upload to RAM address x for length y from PC via XMODEM
ATUR          upload router firmware to flash ROM
ATDC          hardware version check disable during uploading firmware
ATLC          upload router configuration file to flash ROM
ATUXx(,y)     xmodem upload from flash block x to y
ATERx,y       erase flash rom from block x to y
ATWFx,y,z     copy data from addr x to flash addr y, length z
ATXSx         xmodem select: x=0: CRC mode(default); x=1: checksum mode
ATLD          Upload Configuration File and Default ROM File to Flash
ATBR              Reset to default Romfile
ATCD          Convert Running ROM File to Default ROM File into Flash

OK
```

Finally, we get a whole new set of commands that allows us to dump the memory

```
$ ATMP
```


```
ROMIO image start at bfc30000
code version: 
code start: 80008000
code length: 1A5D4E
memMapTab: 18 entries, start = bfc44000, checksum = 9770
$RAM Section:
  0: BootExt(RAMBOOT), start=80030000, len=18000
  1: HTPCode(RAMCODE), start=80048000, len=E0000
  2: RasCode(RAMCODE), start=80048000, len=420000
$ROM Section:
  3: BootBas(ROMIMG), start=bfc28000, len=4000
  4: DbgArea(ROMIMG), start=bfc2c000, len=2000
  5: RomDir2(ROMDIR), start=bfc2e000, len=2000
  6: BootExt(ROMIMG), start=bfc30030, len=13FD0
  7: MemMapT(ROMMAP), start=bfc44000, len=C00
  8: HTPCode(ROMBIN), start=bfc44c00, len=8000
     (Compressed)
     Version: HTP_TC V 0.05, start: bfc44c30
     Length: 10488, Checksum: CB32
     Compressed Length: 41CF, Checksum: D5A5
  9: termcap(ROMIMG), start=bfc4cc00, len=400
 10: RomDefa(ROMIMG), start=bfc4d000, len=2000
 11: LedDefi(ROMIMG), start=bfc4f000, len=400
 12: LogoImg(ROMIMG), start=bfc4f400, len=2000
 13: LogoImg2(ROMIMG), start=bfc51400, len=2000
 14: StrImag(ROMIMG), start=bfc53400, len=32000
 15: StrImag2(ROMIMG), start=bfc85400, len=32000
 16: Rt11nE2p(ROMIMG), start=bfcb7400, len=400
 17: RasCode(ROMBIN), start=bfcb7800, len=246C00
     (Compressed)
     Version: ADSL ATU-R, start: bfcb7830
     Length: 321D2C, Checksum: 582D
     Compressed Length: 11E54B, Checksum: B30B
$USER Section:
Msecs   128
Heap0   16   300 16
Heap1   32   64  4
Heap2   64   64  4
Heap3   128  160 4
Heap4   192  256 4
Heap5   256  80  4
Heap6   320  20   4
Heap7   384  4   2
Heap8   448  20  2
Heap9   512  34  2
Heap10  1024 52  4
Heap11  2048 20  2
Heap12  3172 4   2
Heap13  4096 2   2
Heap14  0 0
...
```

What we just learnt will help us analysing the rom files we extracted earlier 

```
code start: 80008000
code length: 1A5D4E
```

Now we can set the __ROM start address__ and __Loading address__ to __0x80008000__ after dumping a new rom file using:

```
$ ATDO 80008000, 1A5D4E
```

We know that __ATGR__ boot the router so what we could do is tracking it to find the instructions that passes the execution to the 2nd part of the firmware.



![atgr1](https://benchaliah.github.io/assets/img/posts/atgr1.png)

```assembly
ROM:80011E8C loc_80011E8C:            # CODE XREF: LoadAndRunImage+D8j
ROM:80011E8C                 jal     sub_80009B24
ROM:80011E90                 li      $a0, 0x1F4
ROM:80011E94
ROM:80011E94 execute_image:           # execute image
ROM:80011E94                 jalr    $s0        # jump and link to address in $s0
ROM:80011E98                 nop
```

Now we need to ask what is the address stored at $s0? Obviously we still can't set a breakpoint, we can't view the registers so we need to leak it using some trick. Fortunately, we can read and write memory, take a look on the following command:

```
ATWL 80011E94, ae30001c
ATGR
```

From what it looks like, this command patches the instruction at __0x80011E94__ with __ae30001c__ (the byte representation of MIPS __sw $s0, 0x1C($s1)__ instruction). By overwriting the original __jalr $s0__ with __sw $s0, 0x1C($s1)__ we have forced the program to store the contents of __$s0__ register to __0x8001FF1C__ memory address. So now the second's stage firmware will not be executed instead the "ERROR" message will be presented and we will still have the access to the first stage console, Hooray! So let's take a look what was stored at __0x8001FF1C__.


```
ATRL 8001FF1C
8001FF1C: 80020000
```

Gotcha! ... using __0x80020000__ we can dump and properly analyse the 2nd part of the firmware

To be continued ...
