#!/bin/bash

[ $# -eq 0 ] && exit 0
N=10
if [ "$1" = "-n" ]; then
	if [ "$#" -lt 2 ] || { ! echo "$2" | grep -E -q "^[0-9]+$"; }; then
		echo "error: non-negative integer expected after -n option" >&2
		exit 1
	fi
	N=$2
	shift 2
fi

while [ $# -ne 0 ]; do
	tail -n $N "$1" | awk -v idf=$(echo "$1" | sed -E "s/^(.*)\.log$/\1/") \
						'{ printf("%s %s %s", $1, $2, idf); $1=$2=""; print substr($0, 2); }'
	shift
done | sort -t ' ' -k 1,2
