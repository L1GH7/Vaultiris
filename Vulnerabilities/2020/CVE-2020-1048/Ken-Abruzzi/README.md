# CVE-2020-1048

An elevation of privilege vulnerability exists when the Windows Print Spooler service improperly allows arbitrary writing to the file system. An attacker who successfully exploited this vulnerability could run arbitrary code with elevated system privileges. An attacker could then install programs; view, change, or delete data; or create new accounts with full user rights.

To exploit this vulnerability, an attacker would have to log on to an affected system and run a specially crafted script or application.

The Microsoft Windows update addresses the vulnerability by correcting how the Windows Print Spooler Component writes to the file system.

## Steps to reproduce

1.Change the variables g_PortName and g_InputFile present at the top of Source.c.

2.Compile and run using Visual studio.

3.Restart the printer service(spoolsv) or Restart your system.

## Notes

In case you want to persist the port and printer creation or want the attack to occur after restart of system, you can exit the program using Ctrl^C once the program ask to press Enter.

getshell.dll is included which you can use as a payload to spawn a command prompt as SYSTEM privilege.
