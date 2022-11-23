# CVE-2019-18845
 The MsIo64.sys and MsIo32.sys drivers in Patriot Viper RGB before 1.1 allow local users (including low integrity processes) to read and write to arbitrary memory locations, and consequently gain NT AUTHORITY\SYSTEM privileges, by mapping \Device\PhysicalMemory into the calling process via ZwOpenSection and ZwMapViewOfSection.

 To be honest, I really do not want to figure out how the arbitrary physical memory read/write primitive works. Yes, I am lazy, but I will be coming back to this after I read some more papers.
