#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
BUILD_DIR="${ROOT_DIR}/build"

echo "Creating makefiles in" $BUILD_DIR

if [ ! -d $BUILD_DIR ]
then
    mkdir ${BUILD_DIR}
fi

(cd ${BUILD_DIR} && cmake -DCMAKE_BUILD_TYPE=Debug ..)

