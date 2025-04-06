#!/bin/sh

filesdir=$1
searchstr=$2

if test $# -lt 2 ; then
    echo "Not enough arguments. Usage: finder.sh filesdir searchstr"
    exit 1
fi

if ! test -d "${filesdir}"; then
    echo "${filesdir} does not represent a directory on the filesystem"
    exit 1
fi

number_of_files=$(find "${filesdir}" -type f | wc -l)
number_of_lines=$(grep -r "${searchstr}" "${filesdir}" | wc -l)

echo "The number of files are ${number_of_files} and the number of matching lines are ${number_of_lines}"
