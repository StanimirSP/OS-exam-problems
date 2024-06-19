#!/bin/bash

[ $# -eq 1 ] || exit 1
[ $(id -u) -eq 0 ] || { echo "permissions denied" >&2; exit 2; }
# a)
ps -eo user | tail -n +2 | sort | uniq -c | \
 awk -v cnt=$(ps -u "$1" | wc -l) '$1>cnt {print $2}' | grep -v "^$1$"
# b)
AVG=$(ps -eo times | tail -n +2 | awk '{sum+=$1; cnt++;} END {printf("%d\n", sum/cnt);}')
echo $AVG
# c)
for pid in $(ps -u "$1" -o times,pid | tail -n +2 | awk -v avg=$AVG '$1 > 2*avg {print $2}'); do
	kill $pid 2> /dev/null
	sleep 1
	kill -9 $pid 2> /dev/null
done
exit 0
