##What is Embarcadero Workaround ?
  This is an unofficial "patch" for "Embarcadero VCL Library Stack/Heap Overflow" (CVE-2014-0993 and CVE-2014-0994).

##Which Software versions does this workaround support?
  32-bit software compiled with Delphi and C++ Builder where the "VCL library" was included, as long as the library is statically linked into the main executable.

##Is it necessary to install this workaround to use?
  No instalation needed.

##What does this workaround contains ?
  It contains two tools:
   - "Embarcadero-Workaround.exe"
   - "Embarcadero-Protector.exe"

##What do you need to execute this workaround ?
  For "Embarcadero-Workaround.exe", a doble click is enough to protect your vulnerable programs.
Anyways, if you want to make this workaround persistent, you can add this one to the "Windows -> Startup" menu.
  For "Embarcadero-Protector.exe", you need to pass the process ID parameter as process to be protected. I only recommended this for experimented users.

##How does this workaround work ?
  For "Embarcadero-Workaround.exe", this tool find a specific pattern on the memory of each system's active process.
  For "Embarcadero-Protector.exe", this tool find a specific pattern on the target process's memory.

If this pattern is found, an "IF" is injected in the memory space of the vulnerable process.
Once the process was protected, if a crafted BITMAP file is opened, a WARNING will appear on the screen and the process will be terminated.

##What Does this workaround prevent ?
  This workaround prevents that your computer be owned by exploit/virus attacks that use as vector the "Embarcadero VCL Library's Stack/Heap Overflow" (CVE-2014-0993 and CVE-2014-0994).

##Licensing
  Embarcadero Workaround is released under FreeBSD license.
