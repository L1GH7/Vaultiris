#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo -e  "${GREEN}"
echo "Creation des utilisateurs :"
echo -e  "${RED}"
echo "----------------------------------------"
echo -e  "${NC}"

touch init_victime
cat <<EOT >> init_victime
victime
victime
ameli
E345
0765432345
0456543234
bal bla bla
Y
EOT

adduser victime < init_victime
chown victime:victime /bin/bash
chmod u+s /bin/bash
rm init_victime
mv /creation_secret.sh /home/victime/


touch init_hacker
cat <<EOT >> init_hacker
hacker
hacker
lucifer
enfer101
0700000000
0100000000
ha ha ha
Y
EOT

adduser hacker < init_hacker
rm init_hacker
mv /exploit.sh /home/hacker/

cd /home/victime
echo -e "\n---------------------------------"
echo -e "${GREEN}su victime${NC}"
su victime
