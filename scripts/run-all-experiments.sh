#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
BUILD_DIR="${ROOT_DIR}/build"

# echo "Build directory: " ${BUILD_DIR}
${SCRIPT_DIR}/make-all.sh

echo "Running foraging experiments."
launch_argos -nc xml/001-xp.xml
launch_argos -nc xml/002-xp.xml
launch_argos -nc xml/003-xp.xml
launch_argos -nc xml/004-xp.xml
launch_argos -nc xml/005-xp.xml
launch_argos -nc xml/006-xp.xml
launch_argos -nc xml/007-xp.xml
launch_argos -nc xml/008-xp.xml
launch_argos -nc xml/009-xp.xml
launch_argos -nc xml/010-xp.xml
launch_argos -nc xml/011-xp.xml
launch_argos -nc xml/012-xp.xml

