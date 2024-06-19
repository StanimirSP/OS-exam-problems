#!/bin/bash

[ $# -ge 1 -a $# -le 2 ] || exit 1
[ -d "$1" ] || { echo "first argument not a dir" >&2; exit 2; }

if [ $# -eq 2 ]; then
	echo "$2" | grep -E -q "^[0-9]+$" || { echo "second argument not a number" >&2; exit 3; }
	find "$1" -type f \( -links $2 -o -links +$2 \) 2> /dev/null
else # [ $# -eq 1 ]
	find "$1" -type l ! -exec [ -e '{}' ] \; -print 2> /dev/null
fi
