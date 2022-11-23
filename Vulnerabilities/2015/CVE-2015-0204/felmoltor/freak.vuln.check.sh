#!/bin/bash

# Author: Felipe Molina (@felmoltor)
# Date: 03/03/2015
# Summary:
#   This scripts tries to detect if a server is accepting EXPORT cipher suites
#   that could be vulnerable to CVE-2015-0204 a.k.a. FREAK attack. 
#   If your server ends up accepting any Export cipher suites
#   reconfigure your server to reject them, as they could allow an 
#   attacker to decipher comunications with your clients.
#   More information in:
#   * https://www.smacktls.com/
#   * https://freakattack.com/
#   * http://en.wikipedia.org/wiki/Export_of_cryptography_from_the_United_States

###############

###########
# GLOBALS #
###########

NORMAL=$(tput sgr0)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)
RED=$(tput setaf 1)
OUTPUT="$(date +%Y%m%d_%H%M%S)_freak.check.output.csv"

#############
# FUNCTIONS #
#############

function red() { 
    echo -e "$RED$*$NORMAL" 
}
function green() {
    echo -e "$GREEN$*$NORMAL" 
}
function yellow() { 
    echo -e "$YELLOW$*$NORMAL" 
}

function printUsage {
    echo "Usage: $0 <ip[:port] | file_with ip[:port] list >"
}

#############

function isValidIP()
{
    local  ip=$1
    local  valid=0 # Not valid

    if [[ "$ip" != "" ]];then
        if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
            OIFS=$IFS
            IFS='.'
            ip=($ip)
            IFS=$OIFS
            if [[ ${ip[0]} -le 255 && ${ip[1]} -le 255  && ${ip[2]} -le 255 && ${ip[3]} -le 255 ]];then
                valid=1
            fi
        fi
    fi

    echo $valid
}

################

function testExportCiphers(){
    local target=$1
    local domain=$2

    ciphers=$(openssl ciphers -V | grep "EXP-" | awk '{print $3}')
    for cph in $ciphers;do
        echo -n "$domain;$target;$cph;" >> $OUTPUT
        echo -n "$domain ($target): $cph "
        out=$(echo -e "quit\n" | openssl s_client -cipher $cph -connect $target 2>&1)
        fail=$(echo $out | grep -i "alert handshake failure" | wc -l)
        refused=$(echo $out | grep -i "Connection refused" | wc -l)
        if [[ $refused > 0 ]];then
            yellow "UNKNOWN"
            echo "UNKNOWN" >> $OUTPUT
        elif [[ $fail > 0 ]];then
            green "NOT SUPPORTED"
            echo "NOT SUPPORTED" >> $OUTPUT
        else
            red "SUPPORTED"
            echo "SUPPORTED" >> $OUTPUT
        fi
    done
}

################


echo -n "DOMAIN;HOST;CIPHER SUITE;IS SUPPORTED" > $OUTPUT

tocheck=$1
iplist=""
ip=""
port=""

if [[ "$1" == "" ]];then
    printUsage  
    exit 1
fi

ip=$(echo $tocheck|cut -d':' -f1)
port=$(echo $tocheck|cut -d':' -f2)
if [[ $ip == $port ]];then 
    port=443 
fi
validip=$(isValidIP $ip)

# We are receiving a file with a list of IPs and ports
if [[ -f $tocheck ]];then
    iplist=$tocheck
    for ipport in `cat $iplist`;do
        ip=$(echo $ipport|cut -d':' -f1)
        port=$(echo $ipport|cut -d':' -f2)
        if [[ $ip == $port ]];then 
            port=443 
        fi
        validip=$(isValidIP $ip)
        if [[ $validip == 1 ]];then
            testExportCiphers "$ip:$port" $ip
        else
            domain=$ip
            ip=$(dig +short $domain | head -n1)
            validip=$(isValidIP $ip)
            if [[ $validip == 1 ]];then
                testExportCiphers "$ip:$port" $domain
            else
                echo "$ipport is not a valid IP:Port, skipping"
            fi
        fi
    done
elif [[ $validip == 1 ]];then
    # We are receiving just one IP and port
    testExportCiphers "$ip:$port" "$ip"
else
    domain=$ip
    ip=$(dig +short $domain a | head -n1)
    validip=$(isValidIP $ip)
    if [[ $validip == 1 ]];then
        testExportCiphers "$ip:$port" $domain
    else
        echo "The IP:Port, domain or file you provided is incorrect. Try again"
        printUsage
        exit 1
    fi
fi
