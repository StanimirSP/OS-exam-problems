#!/bin/bash

[ $(id -u) -eq 0 ] || exit 1

while read line; do
	HOMEDIR=$(echo "$line" | cut -d ':' -f 6)
	US=$(echo "$line" | cut -d ':' -f 1)
	if [ -z "$HOMEDIR" ] || sudo -u "$US" [ ! -d "$HOMEDIR" -o ! -w "$HOMEDIR" ]; then
		echo "$US"
	fi
done < /etc/passwd
exit 0
