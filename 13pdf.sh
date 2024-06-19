#!/bin/bash

[ $# -eq 1 ] || exit 1
[ -d  "$1" ] || exit 2

find "$1" -type l ! -exec [ -e '{}' ] \; -print 2> /dev/null
