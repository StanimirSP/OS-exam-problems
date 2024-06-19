#!/bin/bash

[ $# -eq 1 -a -d "$1" ] || { echo "error: expected 1 argument which is dir" >&2; exit 1; }
{
	find "$1" -mindepth 3 -maxdepth 3 -type d -printf "0 %f\n" | sort | uniq # to print friend with 0 lines communication
	find "$1" -mindepth 4 -maxdepth 4 -type f -printf "%f %p\n" | \
     while read line; do
		echo "$line" | cut -d' ' -f1 | grep -E -q "^[0-9]{4}(-[0-9]{2}){5}\.txt$" || continue
		date -d "$(echo "$line" | \
			sed -E "s/^(.{10})-(.{2})-(.{2})-(.{2}).*$/\1 \2:\3:\4/")" >/dev/null 2>&1 || continue
		P=$(echo "$line" | cut -d' ' -f2)
		echo "$(cat "$P" | wc -l) $(echo "$P" | awk -F '/' '{print $(NF-1)}')"
	done
} | awk '{sum[$2]+=$1} END {for(fr in sum) printf("%s %s\n", fr, sum[fr])}' | sort -n -k 2 -r | head
