#!/bin/bash

[ $# -eq 2 ] || exit 1
[ -r "$1" ] || { echo "input file does not exist or not readable" >&2; exit 2; }
[ -d "$2" -a -w "$2" ] || { echo "output dir does not exist or not writable" >&2; exit 3; }
[ "$(find "$2" -maxdepth 0 -empty)" ] || { echo "output dir must be empty" >&2; exit 4; }

getName() {
	echo "$1" | grep -E -o "^[A-Za-z-]+ [A-Za-z-]+"
}

while read line; do
	getName "$line"
done < "$1" | sort | uniq | awk '{printf("%s;%u\n", $0, ++cnt)}' > "${2}/dict.txt"
cnt=1
while read line; do
	grep -E "^$(getName "$line")[^A-Za-z-]" "$1" > "${2}/${cnt}.txt"
	((++cnt))
done < "${2}/dict.txt"
