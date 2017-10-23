#!/bin/bash

set -e
set -u

if [ $# -lt 1 ] ; then
    echo "Usage: $0 SRC_DIR"
    exit 2
fi

cd "$1"
mkdir -p build-2
cd build-2

MESON=$(which meson.py || which meson)
"${MESON}" "$1" || true
ninja
