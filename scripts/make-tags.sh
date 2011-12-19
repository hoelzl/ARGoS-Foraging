#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
ARGOS_INCLUDE_DIR=/usr/include/argos2

echo "Building tags file."
ctags -e --recurse=yes --verbose=yes -IREGISTER_CONTROLLER -IREGISTER_LOOP_FUNCTIONS -IREGISTER_QTOPENGL_USER_FUNCTIONS --exclude=CMakeLists.txt controllers loop_functions $ARGOS_INCLUDE_DIR

