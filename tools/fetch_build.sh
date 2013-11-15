#!/bin/bash

if [ -d "tools/build" ] ; then
	cd "tools/build" && git pull
	exit 0
fi
url="$1"
if [ "x$url" == "x" ] ; then
	url="git@blistud.com:/opt/git/build-tools.git"
fi
git clone "$url" tools/build
if [ ! -d "tools/build" ] ; then
	echo "Usage: $0 build-tools.git"
	exit 1
fi
