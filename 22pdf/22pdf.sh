#!/bin/bash
[ $# -ge 1 -a $# -le 2 ] || { echo "invalid number of arguments" >&2; exit 1; }
[ -d "$1" ] || { echo "error: 1st arg: dir expected" >&2; exit 2; }
{
	find "$1" -type l -exec [ -e '{}' ] \; -printf "%p -> %l\n" 2>/dev/null
	echo "Broken symlinks: $(find "$1" -type l ! -exec [ -e '{}' ] \; -printf "0\n" 2>/dev/null | wc -l)"
} >>${2:-/dev/stdout}
