# CVE-2021-30853

A simple POC script to test for CVE-2021-30657 affecting MacOS. <br/>
This CVE allows bypass of gatekeeper, notraization and xprotect checks.
## Vulnerability detail

This Vunlerability occurs when you don't define a interpreter( or specify a interpreter that itself is a shell script) in first line(shebang) of the main script of your executable bundle. This will cause `xpcproxy` to invokes `posix_spawnp` to launch the interpreter-less script-based application. This initially errors out (no interpreter → ENOEXEC), but then `posix_spawnp` "recovers" and (re)executes the script ...this time directly via `/bin/sh`.<br/> Later, the AppleSystemPolicy kext intercepts the process launch to ensure its conformant (signed, notarized, etc). But it checks `/bin/sh` without any variable set, so execute the script without any default checks.<br/> Based on thread: https://twitter.com/objective_see/status/1473741597368098819


## Steps to reproduce
* Put your desireable shell script code in payload.sh. Keep in mind to not modify the first line of script (#!/usr/bin/command).
* Execute setup.sh.
* This will generate a bait.dmg that will contain our malicious app bundle.
* Share it to the victim through internet.
* When victim will double click on app icon after mounting dmg, it will execute the payload script without any gatekeeper's checks.

## Affected version

    macOS Big Sur < 11.6

## Technical details
https://objective-see.com/blog/blog_0x6A.html

