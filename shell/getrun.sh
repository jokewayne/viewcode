#!/bin/bash
echo "----------get $1----------"
curl ftp://zte:ecc@192.168.50.90/getrun/$1 -O
chmod +x $1
echo "----------run $1-----------"
./$1
echo "----------done $1-----------"
