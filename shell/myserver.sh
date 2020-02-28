#!/bin/bash
while [ TRUE ] ; do
	ret=`nc -l -d 10000`
	echo `date +%F-%T`  ": ${ret}."
	if [[ ${ret} == "exit" ]]; then
		break
	fi
done
echo "byebye"
