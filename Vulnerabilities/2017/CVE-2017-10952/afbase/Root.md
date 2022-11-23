Foxit Reader command injection(CVE-2017-10951)and file writing Vulnerability(CVE-2017-10952)2017-08-22 00:00:00
ID SSV:96369
Type seebug
Reporter Root
Modified 2017-08-22 00:00:00
Description
A tale about Foxit Reader - Safe Reading mode and other vulnerabilities

Some days ago someone send me the following link, which describes two vulnerabilities in Foxit Reader: http://thehackernews.com/2017/08/two-critical-zero-day-flaws-disclosed.html

These two vulnerabilities are similar to the behavior of Foxit Reader I presented at Appsec Belfast 2017. Unfortunately the recording was never published, so I decided it’s time for a blog post to give some additional information about these vulnerabilities.

First I have to describe the implemented security model in Foxit Reader.
Safe-Reading mode

Foxit Reader implements a one-line defense, the so-called "Safe-Reading mode". It is enabled by default. In case it is enabled it prohibits the execution of scripts and other features, which can harm the security of the end user.
During my presentation I said, that this feature should never ever be disabled.

In case a vulnerability requires a disabled "Safe-Reading mode", Foxit will mostly not patch it. This is true for the two “vulnerabilities” described in the link above.
Note:

Apparently Foxit decided to provide a patch for the two vulnerabilities mentioned in the hackernews blog post.
https://www.zerodayinitiative.com/blog/2017/8/17/busting-myths-in-foxit-reader

Short quote extracted from the Foxit statement:
"Foxit Software is deeply committed to delivering secure PDF products to its customers. Our track record is strong in responding quickly in fixing vulnerabilities […]"

So lets continue talking about my similar findings, one of which is still unfixed.
Execute local file

    Reported: 5.5.2017 to Foxit Security team
    Security bulletin released: 4.7.2017 https://www.foxitsoftware.com/de/support/security-bulletins.php
    Function call: xfa.host.gotoURL
    Reality: Still Unfixed. Not protected by Safe-Reading mode!
    Tested Foxit version: 8.3.1.21155

CVE-2017-10951 is abusing the app.launchURL JavaScript call to execute a local program, without any user interaction. I am using another function with a similar functionality called xfa.host.gotoURL.
By reading the specification it can be seen that normally these functions accept a URL, which is opened in a new browser window. So far so simple.
I assume CVE-2017-10951 used the same URL I did to execute a local program (I am not 100% sure as no exact details are public).

Instead of passing a http/https URL to xfa.host.gotoURL I used the file:/// protocol handler. To execute cmd.exe. The following file:/// URL is enough:

xfa.host.gotoURL("file:///c:/windows/system32/cmd.exe");

One difference between app.launchURL and xfa.host.gotoURL is this one: xfa.host.gotoURL is not protected by the safe reading mode or as I described in my email to the Foxit security team:

The XFA standard defines the xfa.host.gotoURL function call, which
should load an URL. I discovered that this function is not protected by
the Trust Manager, nor does it check the specified protocol.
The following example will open “cmd.exe” without any user interaction:

xfa.host.gotoURL("file:///C:/windows/system32/cmd.exe");

I have no idea why Foxit did not patch my vulnerability but hopefully they do now!
Note: This is not a full “Safe Reading Mode” bypass. This only works for this exact function call!

Have fun with the PoC (it opens cmd.exe and calc.exe. When you close the PDF it will open explorer.exe):

https://drive.google.com/open?id=0B2HQuxIrwJ53a1hNS2pSYTZzdU0

https://www.youtube.com/watch?v=CWu4OHwtzm8
File execution - limitations:

    It is not possible to pass parameters to the executed program. Maybe it is possible via app.launchURL but the text/video does not contain any hint that this is the case.

    When the file:/// protocol handler is pointing to an executable, which is stored on a SMB share, the Windows operating system will trigger a warning box asking the user for confirmation to execute the program.

    In case the handler is pointing to a currently downloaded file (most likely via the web browser), Windows will once again ask the user for confirmation before the program is executed. Downloaded files contain a so-called "Zone Identifier". This identifier contains information about the source of the executable. In case a file is downloaded from a website like example.com, it will contain a Zone Identifier of 3. A ZI of 3 always triggers a warning dialog before the file is executed (note: there are some exceptions to this rule).

I am aware of one possible way to bypass these restrictions but this will require another blog post ;)

Drop a file to the local file system

I reported my finding in 2016 via ZDI in combination with the safe-reading mode bypass: http://www.zerodayinitiative.com/advisories/ZDI-16-396/

Reality: Patched in combination with the Safe-Reading mode bypass in 2016. It is still working with disabled Safe-Reading mode (as intended I assume)

Lets move on the next vulnerability described in the link above.

Once again I used a different function call with the same functionality. I think you can see a pattern ^^.
CVE-2017-10592 is using the this.saveAs function call to drop a file to the local file system. I always used the xfa.host.exportData function to achieve the same functionality. Both function accept a device independent path (the PDF way to define a local path, independent of the operating system) to store a file. As the file path is completely user controlled, the file extension can be chosen freely.

In case of the saveAs function, the stored PDF file itself can be converted to other file types although I do not know if Foxit Reader actually supports this functionality.

The xfa.host.exportData function call exports a XML structure. As it is either really difficult or even impossible to drop a valid executable (as the attacker has no full control of the content of the file), the easiest way to exploit this kind of vulnerability on the Windows operating system is dropping a HTML application (.hta).

A HTML application behaves like a normal HTML file (eg. any characters, which are no valid HTML elements are happily ignored) but it has access to powerful JavaScript API calls, which allow to execute programs with parameters, local file access and more.

All the attacker has to do is embedding a valid script tag inside the PDF structure and ensure that is stored in the created HTA file.

By dropping this kind of file into the startup folder, the attacker just has to wait for the victim to restart his PC. In case the attacker does not want to wait for a restart, he can drop his malicious HTA file and use the before mentioned functionality to immediately execute it (the dropped file does not have a Zone Identifier).

Proof-of-Concept (the PoC stores no real payload in the dropped file):

    Open the PoC in Foxit Reader
    Disable Safe Reading mode
    Restart Foxit Reader
    Open the PDF
    Close it. A file called evilHTA.hta will be dropped on the desktop.

PoC: https://drive.google.com/open?id=0B2HQuxIrwJ53cGxPUndCdWY3T28

In case you are wondering why the onclose event is used, I can tell you a near null exception crashes Foxit Reader.

So this was a short introduction about Foxit Reader and why you should never disable the Safe Reading mode.

But wait… is there a way to bypass the "Safe-Reading mode"?
The following bypass is fixed but maybe it inspires someone to search for new bypasses :)
[+] Fixed: Safe-Reading mode bypass

When I started to play with Foxit Reader I did not read anything about the implemented security and instead just jumped right into it.

I used different functions, which I know could introduce security problems until I tried xfa.host.exportData.
Suddenly my file was dropped without any user interaction. My first reaction was: “WTF? This can’t be real. There should be some security protection in place.”

So I started to research and discovered: I bypassed the safe-reading mode without even realizing it ^^

Basically what I used while researching was XFA. XFA is a XML structure defined in the PDF standard, which defines everything related to forms in PDF.

It allows to define buttons, text boxes and more. Additionally, similar to HTML, you can react to events triggered for each element and the document itself.

This allows you to specify JavaScript, which is executed as soon as the event is fired. A simplified example to understand the concept is provided by corkami: https://raw.githubusercontent.com/corkami/pocs/master/pdf/formevent_js.pdf

In my case I reacted to the “initialized” event for my created button element.

As you can possible guess, this event is fired every time the element is initialized and therefore it fires really early during the parsing of the PDF structure.

And this was all needed to bypass the “Safe-Reading” mode. Apparently the event fired so early that the mode was not initialized or they forgot to apply it for this event too.

