# CVE-2022-39959

###### Affected Vendor- Panini https://www.panini.com/en
###### Affected Product - Panini Everest Engine
###### Affected Version - 2.0.4

There is an Unquoted Service Path in Everest Engine(EverestEngine.exe) in version **2.0.4** on Windows. This allows an unauthorized local user to insert arbitrary code into the unquoted service path and escalate privileges to system


**File Path:** C:\ProgramData\Panini\Everest Engine


#### Exploit
Add arbitrary code named Everest.exe file to the unquoted path %PROGRAMDATA%\Panini and once system is rebooted or service restarted the attacker will gain system privileges to the system

##### Impact
Escalation to System Privileges



