# CVE-2017-18044 Native POC

This is a dead simple POC for the CVE-2017-18044 command injection vulnerability in Commvault v11 SP5 and older. For metasploit users, a ruby version of the exploit is also bundled with the lastest build. https://www.rapid7.com/db/modules/exploit/windows/misc/commvault_cmd_exec

Usage: comvlt <options>
  
    -h      Usage
    -i      IP Address of host (Default: 127.0.0.1)
    -e      File path of executable
    -a      Executable arguments
