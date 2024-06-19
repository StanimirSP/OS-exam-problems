#!/bin/bash

escapeSpecial() {
	echo "$1" | sed -E 's/[][\.$()|*+?{}^]/\\&/g'
}

[ $# -eq 3 ] || exit 1
[ -f "$1" -a -r "$1" ] || exit 2
R1=$(egrep "^$(escapeSpecial "$2")=" "$1")

if [ $? -ne 0 ]; then
	echo "key1 not found in the input file" >&2
	exit 3
fi
R1=$(escapeSpecial "$R1")

R2=$(egrep "^$(escapeSpecial "$3")=" "$1") || exit 0
R2=$(escapeSpecial "$R2")
while read i; do
	i=$(escapeSpecial "$i")
	R2=$(echo "$R2" | sed -E "s/ $i( |$)/\1/g" | sed -E "s/(=$i )|(=$i$)/=/g")
done < <(echo "$R1" | sed -E "s/=(.*)$/\1/g" | tr ' ' '\n')

#while read i; do
#	i=$(escapeSpecial "$i")
#	R2=$(echo "$R2" | sed -E "s/ $i( |$)/\1/g" | sed -E "s/(=$i )|(=$i$)/=/g")
#done < <(echo "$R2" | sed -E "s/=(.*)$/\1/g" | tr ' ' '\n' | sort | uniq -d)

sed -E -i "s/^$(escapeSpecial "$3")=.*$/$R2/" "$1"
