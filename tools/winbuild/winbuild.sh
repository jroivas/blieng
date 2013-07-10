#!/bin/bash

if [ -x "$WINPATH/i686-pc-mingw32-gcc" ] ; then
	make $@
else
	echo ""
	echo "****** Can't build for Windows. ******"
	echo "Please install MXE (http://mxe.cc/) and setup WINPATH to mxe/usr/bin:"
	echo "export WINPATH=/path/to/mxe/usr/bin"
	echo "**************************************"
fi
