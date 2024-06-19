#!/bin/bash

MINMAX=$(grep -E "^[+-]?[0-9]+$" | sed -E 's/^([+-]?)0*(.)/\1\2/g' | sed 's/+//' | sort -n | \
		 awk '{ if(FNR==1) print } END { print }')
[ -z "$MINMAX" ] && exit 0
MIN=$(echo "$MINMAX" | head -n 1)
MAX=$(echo "$MINMAX" | tail -n 1)

if [ $((-MIN)) -eq $MAX ]; then
	[ $MAX -ne 0 ] && echo $MIN
	echo $MAX
elif [ $((-MIN)) -gt $MAX ]; then
	echo $MIN
else
	echo $MAX
fi
