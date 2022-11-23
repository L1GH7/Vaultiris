# Mail Order

The Nalpeiron Licensing Service (`NLSSRV32.exe`) is installed as part of certain software installations to handle privileged software licensing operations. It is paired with a client component (`filechck.dll`) which communicates with the service via mailslots (hence the name).

Calls are performed to the service using custom serialization and many involve direct access to disk volumes attached to the machine. I believe these are used for integrity checking the service itself and client binaries. Many of these call types are not restricted to administrators, and can therefore be used to read (and potentially write) raw data from local drives.

I'm unaware how common this service is. The most notable example is Nitro PDF <= v10. Nalperion has since moved to a cloud-based licensing model and does not appear to recommend this model anymore.

### Usage

I've only implemented a basic read call for the proof of concept. However, there are more control codes that expose more functionality. It works against the primary drive when the service is hosted, and reads a sector in sector increments (512). Provided you were motivated, reading the entire disk and re-creating NTFS volume information should be entirely possible. An excercise for the reader I suppose.

```
PS C:\> MailOrder.exe c 4096

[+] Requesting sector from c: at offset 4096 ...
[+] Drive data [hex]:

45 46 49 20 50 41 52 54 00 00 01 00 5C 00 00 00 0D 24 5C 77 00 00 00 00 01 00 00 00 00 00 00 00
AF 6D 70 74 00 00 00 00 22 00 00 00 00 00 00 00 8E 6D 70 74 00 00 00 00 96 2F 01 83 CE 70 E7 41
...
```

### Mitigation / Exposure

The two strongest indicators of exploitabilty are:

- Nalpeiron Licensing Service running:
```
PS C:\> Get-Service nlsX86cc

Status   Name               DisplayName
------   ----               -----------
Running  nlsX86cc           Nalpeiron Licensing Service
```

- Server-side mailslot for communication is available: `\\.\mailslot\nlsX86ccMailslot`

Outside of uninstalling the service and/or the software it is used for, I unaware of a mitigation. The basic of exploitability is a weak ACL on the mailslot, but I believe it's required for the client side to communicate properly.

### Discolsure

I have attempted to contact the vendor on multiple occassions with no response. If anyone from Nalpeiron wishes to assist with details/remediation, please contact me.

- [11/16/19] : Vendor contacted via email requesting assistance with discolsure
- [11/20/19] : Follow up with vendor via email
- [11/26/19] : MITRE issued `CVE-2019-19315` for this vulnerability 
