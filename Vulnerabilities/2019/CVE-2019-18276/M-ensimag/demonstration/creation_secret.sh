#!/usr/bin/env bash

GREEN='\033[0;32m'
NC='\033[0m'

cd /home/victime
echo "Je suis 'victime', le seul etre capable de lire ce contenu!" > .secret.txt
chmod og-r .secret.txt

echo -e "\n${GREEN}Running${NC}: ls -la\n"
ls -la
cd /home/hacker
echo -e "\n"
echo -e "su hacker (mot de passe est: ${GREEN}hacker) ${NC}"
su hacker
