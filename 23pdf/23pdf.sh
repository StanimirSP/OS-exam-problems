#!/bin/bash

[ $# -eq 2 ] || exit 1
[ -d "$1" ] || { echo "fisrt argument should be a dir" >&2; exit 2; }

Comp() {
	if [ $1 -ne $4 ]; then
		test $1 -gt $4
		return
	fi
	if [ $2 -ne $5 ]; then
		test $2 -gt $5
		return
	fi
	test $3 -gt $6
	return
}

xmax=0; ymax=0; zmax=0
while read file; do
	x=$(echo "$file" | sed -E "s/^vmlinuz-([0-9]+)\.[0-9]+\.[0-9]+-.*$/\1/")
	y=$(echo "$file" | sed -E "s/^vmlinuz-[0-9]+\.([0-9]+)\.[0-9]+-.*$/\1/")
	z=$(echo "$file" | sed -E "s/^vmlinuz-[0-9]+\.[0-9]+\.([0-9]+)-.*$/\1/")
	if Comp $x $y $z $xmax $ymax $zmax; then
		xmax=$x; ymax=$y; zmax=$z
	fi
done < <(find "$1" -maxdepth 1 -type f -printf "%f\n" | grep -E "^vmlinuz-([0-9]+\.){2}[0-9]+-$2$")

find "$1" -maxdepth 1 -type f -printf "%f\n" | grep -E "^vmlinuz-$xmax.$ymax.$zmax-$2$"
