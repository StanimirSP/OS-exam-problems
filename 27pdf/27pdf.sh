#!/bin/bash

[ $# -eq 2 ] || exit 1
[ -r "$1" ] || { echo "input file does not exist or not readable" >&2; exit 2; }

sort -r -n -t ',' -k 1,1 "$1" | \
 awk -F ',' 'BEGIN {OFS=FS} {id=$1; $1=""; minId[$0]=id} END {for(i in minId) print minId[i] i}' >"$2"
