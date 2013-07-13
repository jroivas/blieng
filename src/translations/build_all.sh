#!/bin/bash

BASENAME=zombiebli
SOURCENAME=$BASENAME.ts
TARGETNAME=$BASENAME.qm
#langs="fi_FI en_US"
langs=$(ls -d ??_??)

for l in $langs
do
	lrelease "$l/$SOURCENAME"
	mv -f "$l/$TARGETNAME" "$BASENAME.$l.qm"
done
