# Zoom-dll-hijacking
A dll hijacking vulnerability in zoom meeting &lt; 5.1.3 
<br />
### Affected Windows version
* Windows 7 or less
* Windows Server 2008
* Any other windows system that doesn't have `SHCore.dll` present.

### Usage
* Copy the `dbghelper.dll` and `SHCore.dll` to following location
  `C:\Users\$user\AppData\Roaming\Zoom\bin\`
* Start the zoom application or do any activity like turning on/off camera inside zoom meeting.

 **Note**: You can modify the code of SHCore.dll by opening `shcore dll.sln` project in Visual studio.
 <br/>
  <br/>
 **Reference**: 
 <br/>
 https://blog.0patch.com/2020/07/remote-code-execution-vulnerability-in.html <br/>
 https://www.programmersought.com/article/85545053408/
