#!/bin/bash

[ $(id -u) -eq 0 ] || { echo "permissions denied" >&2; exit 1; }

findProc() {
	while read line; do
		id=$(echo $line | cut -d ':' -f 3)
		[ $id -eq 0 ] && continue
		hdir=$(echo $line | cut -d ':' -f 6)
		if [ -z "$hdir" ] || [ ! -d "$hdir" ] || \
		   [ $(stat --printf "%u" "$hdir") -ne $id ] || sudo -u \#$id [ ! -w "$hdir" ]; then
			ps -u $id -o user,pid,rss,cmd | tail -n +2
		fi
	done < /etc/passwd
}

proc=$(findProc)
[ "$proc" ] || exit 0
echo "$proc"
echo "$proc" | awk -v sumroot=$(ps -o rss | awk '{sum+=$1} END {print sum}') \
 '{sum[$1]+=$3} END {for(us in sum) if(sum[us]>sumroot) printf("%s\n", us);}' | \
 xargs -n 1 -I '{}' killall -KILL -u '{}'
exit 0
