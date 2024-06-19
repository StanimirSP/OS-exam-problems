#!/bin/bash

[ $# -eq 2 ] || exit 1
[ -d "$1" ] || { echo "fisrt argument should be a dir" >&2; exit 2; }

MAX=$(find "$1" -maxdepth 1 -type f -printf "%f\n" | grep -E "^vmlinuz-([0-9]+\.){2}[0-9]+-$2$" | \
    sed -E "s/^vmlinuz-(.+)-$2$/\1/" | sort -n -t '.' -k 1,1 -k 2,2 -k 3,3 | tail -n 1)

if [ $MAX ]; then
    echo "vmlinuz-$MAX-$2"
fi
