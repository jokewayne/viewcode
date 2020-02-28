#!/bin/bash
SERVERIP=192.168.1.1
while [ TRUE ]; do
	read -p ":" INPUT
	echo $INPUT | nc -w1 ${SERVERIP} 10000
	if [ ${INPUT} == "exit" ] ; then
		break
	fi
done
echo "byebye"
