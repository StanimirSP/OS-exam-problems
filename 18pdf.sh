#!/bin/bash

cut -d ':' -f 1,6 /etc/passwd | \
while read line; do
	dir=$(echo "$line" | cut -d ':' -f 2)
	[ -d "$dir" ] || continue
	find "$dir" -type f -printf "%T@ $(echo "$line" | cut -d ':' -f 1) %p\n" 2> /dev/null
done | sort -n -k 1 | tail -n 1 | cut -d ' ' -f 2-
exit 0
