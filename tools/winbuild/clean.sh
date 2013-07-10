#!/bin/bash

if [ "x$1" == "xlinux" ] ; then
	cat build.last | grep win  && echo "**** Cleaning Windows build" && make clean|| true
else
	cat build.last | grep linux && echo "**** Cleaning Linux build" && make clean || true
fi
