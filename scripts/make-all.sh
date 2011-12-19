#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
BUILD_DIR="${ROOT_DIR}/build"

# echo "Build directory: " ${BUILD_DIR}
if [ ! -d ${BUILD_DIR} ]
then
    mkdir ${BUILD_DIR}
fi

# echo "Makefile: ${BUILD_DIR}/Makefile"
if [ ! -f "${BUILD_DIR}/Makefile" ]
then
    ${SCRIPT_DIR}/make-makefiles.sh
fi

echo "Building foraging experiment."
(cd ${BUILD_DIR} && make)

