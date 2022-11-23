#!/bin/bash

find_cmd=`whereis c_rehash | grep "c_rehash"`
arr=($find_cmd)
path=${arr[1]}
grep_cmd=`cat $path|grep '"$openssl" x509 $x509hash -fingerprint -noout -in "$fname"'`
if [ -n "$grep_cmd" ]; then
	echo "System is Vulnerable! Please fix asap"
else
	echo "System is not Vulnerable!"
fi
