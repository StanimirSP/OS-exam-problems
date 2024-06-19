#!/bin/bash

[ $# -eq 3 ] || exit 1
[ $(id -u) -eq 0 ] || { echo "permissions denied" >&2; exit 2; }
[ -d "$1" -a -d "$2" ] || { echo "SRC and DST must be directories" >&2; exit 3; }
[ "$(find "$2" -maxdepth 0 -empty)" ] || { echo "error: DST is not empty" >&2; exit 4; }

for f in $(find "$1" -type f -name "*$3*" -printf "%P\n"); do
	# if wildcards in STR should be escaped
	echo "$f" | grep -E -o "[^/]+$" | grep -F -q "$3" || continue
	# -------------------------------------
	dir=
	if echo "$f" | grep -q '/'; then
		dir=$(echo "$f" | sed -E "s/\/[^/]+$//")
		mkdir -p "$2/$dir"
	fi
	mv "$1/$f" "$2/$dir"
done
