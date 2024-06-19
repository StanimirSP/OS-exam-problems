#!/bin/bash

[ $(id -u) -eq 0 ] || { echo "permissions denied" >&2; exit 1; }

res=$(ps -eo user,rss | tail -n +2 | \
 awk '{cnt[$1]++; sum[$1]+=$2;} END {for(i in cnt) printf("%s %s %s\n", i, cnt[i], sum[i]);}')
# a)
echo "$res"
# b)
while read line; do
	[ $(echo "$line" | cut -d ' ' -f 1) = root ] && continue
	pid=$(ps -u $(echo "$line" | cut -d ' ' -f 1) -o rss,pid | tail -n +2 | sort -n -r | head -n 1 | \
	 	    awk -v cnt=$(echo "$line" | cut -d ' ' -f 2) -v sum=$(echo "$line" | cut -d ' ' -f 3) \
	  		 '$1 > 2*sum/cnt {print $2;}')
	kill $pid 2> /dev/null
	sleep 1
	kill -9 $pid 2> /dev/null
done < <(echo "$res")
exit 0
