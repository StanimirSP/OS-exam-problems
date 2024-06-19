#!/bin/bash

grep -E "^[+-]?[0-9]+$" | sed -E 's/^([+-]?)0*(.)/\1\2/g' | sed 's/+//' | while read num; do
	sum=0
	for i in $(echo $num | grep -E -o "[0-9]+" | sed -E "s/(.)/\1 /g"); do
		((sum+=i))
	done
	echo "$sum $num"
done | sort -k 1,1rn -k 2,2n | head -n 1 | cut -d' ' -f2
