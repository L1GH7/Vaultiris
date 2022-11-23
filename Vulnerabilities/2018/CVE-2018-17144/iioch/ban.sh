#!/bin/bash

# ban.sh
# shell script to automatically identify and ban vulnerable bitcoin clients

# You need to install jq in order to use this script
command -v jq >/dev/null 2>&1 || { echo >&2 "Please install \"jq\" first. Aborting."; exit 1; }

# Adjust CLIENT variable so it calls bitcoin-cli with the right parameters
# Non standart installations need to add -conf=/PATHtoYOUR/bitcoin.conf -datadir=/PATH/to/YOUR/Datadir/
CLIENT=/usr/local/bin/bitcoin-cli

# Ban time in seconds, 86400 = 1 day, 2592000 = 30 days
BAN_TIME="2592000"

# Temp files
NODES_FILE="`mktemp /tmp/connected-nodes.XXXXXXXXXX`"
BANNED_FILE="`mktemp /tmp/banned-nodes.XXXXXXXXXX`"

# Counter
COUNT=0

# Declaration of array of nodes subversion names.
# Here you can add the nodes subversion or parts of it in order to ban them. Those examples ban everything below version 0.16.3
# Feel free to edit them
declare -a arr=("0.15.1" "0.16.99" "0.15.01" "0.16.2" "0.16.0" "0.14.2" "0.14.99" "0.13.2" "0.15.0.1" "0.9.99" "/bitcore:1.1.2/" "/bitcoinj" "0.16.1" "0.15.99" "0.15.2" "0.14.1" "0.14.0" "0.13.0" "btcwire" "btc-seeder" "/bitnodes.lee/" "/bitnodes.bitcoin-russia.ru:0.0.1f/" "/BitKer/" "bitcoinj" "/bcoin:v1.0.2/")

# Write connected nodes to NODES_FILE
$CLIENT getpeerinfo >$NODES_FILE

# Extract subversion text and the corresponding IP adress
NODES_TO_BAN=`jq -r '.[] | .addr, .subver'  $NODES_FILE`

# Ban clients with the same or partial subversion as in the array
TEMP_COUNT=0
for NODE in ${NODES_TO_BAN[@]}; do
        if [ $TEMP_COUNT -eq 0 ]; then
                IP=$NODE
                TEMP_COUNT=$((TEMP_COUNT + 1))
        else
                SUBVER=$NODE
                TEMP_COUNT=$((TEMP_COUNT - 1))
                for i in "${arr[@]}"
                do
                        if [[ "$SUBVER" == *"$i"* ]]; then
                                 $($CLIENT setban ${IP%:*} "add" ${BAN_TIME})
                                echo Banned client with Subversion: $SUBVER and IP: $IP >> $BANNED_FILE
                                COUNT=$((COUNT + 1))
                        fi
                done
        fi

done
cat $BANNED_FILE
echo Found and banned $COUNT nodes.

rm $NODES_FILE
rm $BANNED_FILE

exit 1
