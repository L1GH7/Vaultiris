# Indicators of Compromise

### IP Addresses
Malicious IP addresses [Here](ip-addresses)

### Web Shell Hashes
A list of hashes [Here](hashes)

### Web Shell Paths
A list of common paths [Here](webshell_paths)

### Web Shell Names
Webshells can be found [Here](webshell_names)

### User-Agents
A list of user agents found [Here](useragents)


## Scan Exchange log files for indicators of compromise

### CVE-2021-26855 exploitation can be detected via the following Exchange HttpProxy logs

- These logs are located in the following directory: `%PROGRAMFILES%\Microsoft\Exchange Server\V15\Logging\HttpProxy`
- Exploitation can be identified by searching for log entries where the AuthenticatedUser is empty and the AnchorMailbox contains the pattern of ServerInfo~*/*
  - Here is an example PowerShell command to find these log entries:

`Import-Csv -Path (Get-ChildItem -Recurse -Path "$env:PROGRAMFILES\Microsoft\Exchange Server\V15\Logging\HttpProxy" -Filter '*.log').FullName | Where-Object { $_.AnchorMailbox -like 'ServerInfo~*/*' -or $_.BackEndCookie -like 'Server~*/*~*'} | select DateTime, AnchorMailbox, UrlStem, RoutingHint, ErrorCode, TargetServerVersion, BackEndCookie, GenericInfo, GenericErrors, UrlHost, Protocol, Method, RoutingType, AuthenticationType, ServerHostName, HttpStatus, BackEndStatus, UserAgent`

- If activity is detected, the logs specific to the application specified in the AnchorMailbox path can be used to help determine what actions were taken.
  - These logs are located in the `%PROGRAMFILES%\Microsoft\Exchange Server\V15\Logging` directory.



### CVE-2021-26858 exploitation can be detected via the Exchange log files

- C:\Program Files\Microsoft\Exchange Server\V15\Logging\OABGeneratorLog
- Files should only be downloaded to the %PROGRAMFILES%\Microsoft\Exchange Server\V15\ClientAccess\OAB\Temp directory
  - In case of exploitation, files are downloaded to other directories (UNC or local paths)
- Windows command to search for potential exploitation:

`findstr /snip /c:"Download failed and temporary file" "%PROGRAMFILES%\Microsoft\Exchange Server\V15\Logging\OABGeneratorLog\*.log"`



### CVE-2021-26857 exploitation can be detected via the Windows Application event logs

- Exploitation of this deserialization bug will create Application events with the following properties:
  - Source: MSExchange Unified Messaging
  - EntryType: Error
  - Event Message Contains: System.InvalidCastException
- Following is PowerShell command to query the Application Event Log for these log entries:

`Get-EventLog -LogName Application -Source "MSExchange Unified Messaging" -EntryType Error | Where-Object { $_.Message -like "*System.InvalidCastException*" }`




### CVE-2021-27065 exploitation can be detected via the following Exchange log files

- C:\Program Files\Microsoft\Exchange Server\V15\Logging\ECP\Server

> All Set-<AppName>VirtualDirectory properties should never contain script. InternalUrl and ExternalUrl should only be valid Uris.

- Following is a PowerShell command to search for potential exploitation:

`Select-String -Path "$env:PROGRAMFILES\Microsoft\Exchange Server\V15\Logging\ECP\Server\*.log" -Pattern 'Set-.+VirtualDirectory'`



