#!/bin/bash
echo "---put    file $1 --------"
curl -T $1 ftp://zte:ecc@192.168.50.90/getrun/ 
echo "---upload file $1 done----"
