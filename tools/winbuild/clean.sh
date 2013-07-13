#!/bin/bash

if [ "x$1" == "xlinux" ] ; then
	cat build.last | grep win > /dev/null && echo -e "\n**** Cleaning Windows build\n****\n" && make clean || true
else
	cat build.last | grep linux  > /dev/null && echo -e "\n**** Cleaning Linux build\n****\n" && make clean || true
fi
