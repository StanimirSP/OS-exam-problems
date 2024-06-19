#!/bin/bash

[ $(id -u) -eq 0 ] || exit 2
[ $# -eq 1 ] || exit 1
{ echo "$1" | egrep '[^0-9]' > /dev/null; } && exit 3

for us in $(cut -d ':' -f 1 /etc/passwd); do
	PROC=$(ps -o pid=,rss= -u $(id -u $us) | awk '{printf("%s %s\n", $1, $2);}' \
			| sort -t ' ' -k 2 -n)
	if [ -z "$PROC" ]; then continue; fi
	sum=$(echo "$PROC" | awk '{ sum+=$2; } END { print sum; }')
	echo $us $sum
	pid=$(echo "$PROC" | tail -n 1 | cut -d ' ' -f 1)
	if [ $sum -gt $1 ]; then
		kill $pid 2>/dev/null
		sleep 2
		kill -9 $pid 2>/dev/null
	fi	
done

exit 0

