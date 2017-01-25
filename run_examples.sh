#!/bin/bash

MYDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
"${MYDIR}/build.sh" "${MYDIR}"
cd "${MYDIR}/examples"

"${MYDIR}/build-2/examples/data_files/data_example"
"${MYDIR}/build-2/examples/world/world_example"
