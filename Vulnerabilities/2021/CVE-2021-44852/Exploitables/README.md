# CVE-2021-44852
 An issue was discovered in BS_RCIO64.sys in Biostar RACING GT Evo 2.1.1905.1700. A low-integrity process can open the driver's device object and issue IOCTLs to read or write to arbitrary physical memory locations (or call an arbitrary address), leading to execution of arbitrary code. This is associated with 0x226040, 0x226044, and 0x226000.

 After analyzing the vulnerability in question, it cannot be hit from the IO control code branches. The only function we can partially control is `nt!IoStartPacket`, and I wish all of you the best of luck if you plan on redirecting control flow via a crafted device object structure.
