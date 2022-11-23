This repo contains an old poc that combines three CVEs (CVE-2017-14954, CVE-2017-18344, CVE-2017-5123).
The poc is a Local Privilege Escalation for Linux Kernel 4.13 (tested on Ubuntu).

The exploit uses an info leak (CVE-2017-14954) to bypass KASLR, an arbitrary read (CVE-2017-18344) to read the kernel memory looking for the `struct cred` for user with uid 1000 (non privileged) and uses the buggy `waitid` system call (CVE-2017-5123) to overwrite the uid value with 0 (root).

I'm not the original author of the three exploits, I just combined them as an exercise.