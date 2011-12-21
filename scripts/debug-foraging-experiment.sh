#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
BUILD_DIR="${ROOT_DIR}/build"

# echo "Build directory: " ${BUILD_DIR}
${SCRIPT_DIR}/make-all.sh

echo "Setting up the environment."
# Set the resource directory
export ARGOSINSTALLDIR=/usr/share/argos2

# Set the library path
if [ -n "${DYLD_LIBRARY_PATH}" ]
then
    DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:/usr/lib/argos2
else
    DYLD_LIBRARY_PATH=/usr/lib/argos2
fi
export DYLD_LIBRARY_PATH

echo "Running foraging experiment under gdb."
gdb --args /usr/bin/argos -nc ${ROOT_DIR}/xml/foraging.xml

