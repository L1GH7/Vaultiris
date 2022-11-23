#!/bin/bash

if [ $# -ne 1 ]; 
    then echo "usage: cve2022_24675.sh root_folder"
    exit
fi


shopt -s globstar
for x in $1/**; do
  [[ $(file -b -- $x | grep "Go Build") ]] || continue
  if [[ $(file -b -- $x | grep "not stripped") ]]; then
    if [[ $(readelf -s $x 2>/dev/null | grep "encoding/pem.Decode" -l) ]]; then
      echo $x "uses pem.Decode - may be vulnerable";
    fi;
  else
    echo $x "is stripped";
  fi;
done