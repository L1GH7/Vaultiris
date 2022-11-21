# CVE-2022-34715-POC
CVE-2022-34715-POC pcap  
nfssvr.sys version 10.0.20348.825
```
0: kd> r
rax=0000000080000001 rbx=0000000000000000 rcx=ffffde87fe461150
rdx=0000000000000000 rsi=fffff8040811aa50 rdi=0000000000000008
rip=fffff80408067dbb rsp=ffffb400eb3e2630 rbp=ffffb400eb3e26b9
 r8=0000001000000020  r9=ffff870fe95d40a4 r10=fffff80409b8cec0
r11=0000000000000381 r12=0000000080000001 r13=0000000000000002
r14=ffffde87fe461150 r15=fffff8040810d898
iopl=0         nv up ei pl nz na pe nc
cs=0010  ss=0018  ds=002b  es=002b  fs=0053  gs=002b             efl=00040202
nfssvr!Nfs4SrvAclBuildWindowsAclsFromNfsAcl+0x39f:
fffff804`08067dbb e880f10900      call    nfssvr!memset (fffff804`08106f40)
0: kd> !pool rcx
Pool page ffffde87fe461150 region is Paged pool
 ffffde87fe461020 size:   30 previous size:    0  (Allocated)  CMNb
 ffffde87fe461050 size:   30 previous size:    0  (Allocated)  FLSk
 ffffde87fe461080 size:   30 previous size:    0  (Free)       Ntf0
 ffffde87fe4610b0 size:   30 previous size:    0  (Free)       SeUs
 ffffde87fe4610e0 size:   30 previous size:    0  (Allocated)  CMNb
 ffffde87fe461110 size:   30 previous size:    0  (Allocated)  Ntf0
*ffffde87fe461140 size:   30 previous size:    0  (Allocated) *TEMP
		Owning component : Unknown (update pooltag.txt)
1: kd> g
KDTARGET: Refreshing KD connection
.
.
.
nt!DbgBreakPointWithStatus:
fffff803`06c2f460 cc              int     3
1: kd> k
 # Child-SP          RetAddr               Call Site
00 ffffbf0f`b8e449e8 fffff803`06d643d2     nt!DbgBreakPointWithStatus
01 ffffbf0f`b8e449f0 fffff803`06d63c1d     nt!KiBugCheckDebugBreak+0x12
02 ffffbf0f`b8e44a50 fffff803`06c26ef7     nt!KeBugCheck2+0xa7d
03 ffffbf0f`b8e451b0 fffff803`06aea9a7     nt!KeBugCheckEx+0x107
04 ffffbf0f`b8e451f0 fffff803`06ac486e     nt!MiSystemFault+0xa77
05 ffffbf0f`b8e452f0 fffff803`06c359b5     nt!MmAccessFault+0x2ee
06 ffffbf0f`b8e45490 fffff803`04d270b5     nt!KiPageFault+0x335
07 ffffbf0f`b8e45620 fffff803`04c87dc0     nfssvr!_memset_repmovs+0x35
08 ffffbf0f`b8e45630 fffff803`04c7af07     nfssvr!Nfs4SrvAclBuildWindowsAclsFromNfsAcl+0x3a4
09 ffffbf0f`b8e45710 fffff803`04c7cd47     nfssvr!Nfs4SrvFhcpSet_fattr4_acl+0x37
0a ffffbf0f`b8e45750 fffff803`04c40fba     nfssvr!Nfs4SrvFhcSetAttributes+0x333
0b ffffbf0f`b8e45830 fffff803`04c29fac     nfssvr!Nfs4SrvOpSetAttr+0x36a
0c ffffbf0f`b8e458e0 fffff803`04c2a52e     nfssvr!Nfs4SrvCpProcessCompound+0x4cc
0d ffffbf0f`b8e45980 fffff803`04c2a755     nfssvr!Nfs4SrvInPacketInternal+0x262
0e ffffbf0f`b8e459f0 fffff803`04c2177f     nfssvr!Nfs4SrvDispatch+0x125
0f ffffbf0f`b8e45a30 fffff803`04c219e1     nfssvr!NfsSrvMessage+0x5b3
10 ffffbf0f`b8e45ad0 fffff803`0b5d5938     nfssvr!NfsSrvTcpMessage+0x11
11 ffffbf0f`b8e45b00 00000000`00000000     0xfffff803`0b5d5938
```

