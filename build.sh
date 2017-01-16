#!/bin/bash

set -e
set -u

if [ $# -lt 1 ] ; then
    echo "Usage: $0 SRC_DIR"
fi

cd "$1"
mkdir -p build-2
cd build-2
meson.py "$1" || true
ninja
