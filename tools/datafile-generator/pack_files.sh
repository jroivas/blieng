#!/bin/bash

mydir=$(dirname $0)

if [ ! -d "$1" ] ; then
	echo "Diretory not found: $1"
	exit 1
fi
find "$1" | while read p
do
	$mydir/datafile-generator "$2" "$p" "$p"
done
