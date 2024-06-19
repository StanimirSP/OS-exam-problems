#!/bin/bash

[ $# -eq 1 ] || exit 1
[ -d "$1" ] || { echo "error: dir expected" >&2; exit 2; }

find "$1" -name "*.tgz" -type f $(find .lastExecuted -printf "-newer %f\n" 2>/dev/null) 2>/dev/null | \
	grep -E "/[^_]*_report-[0-9]*\.tgz$" | while read file; do
		EXTR_NAME=$(echo "$file" | grep -E -o "[^/]+$" | sed -E "s/(.*)_report-(.*)\.tgz/\1_\2.txt/")
		tar -C ./extracted -x meow.txt -zf "$file" --transform="s/.*/$EXTR_NAME/" 2>/dev/null
	done
touch .lastExecuted
exit 0
