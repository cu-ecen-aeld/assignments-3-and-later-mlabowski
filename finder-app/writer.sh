#!/bin/bash

writefile=$1
writestr=$2

if test $# -lt 2 ; then
    echo "Not enough arguments. Usage: writer.sh writefile writestr"
    exit 1
fi

dirpath=$(dirname "${writefile}")
mkdir -p "${dirpath}"

echo "${writestr}" > "${writefile}"