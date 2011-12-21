#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
BUILD_DIR="${ROOT_DIR}/build"

# echo "Build directory: " ${BUILD_DIR}
${SCRIPT_DIR}/make-all.sh

echo "Running foraging experiment."
launch_argos -nc xml/foraging-experiment-1.xml
launch_argos -nc xml/foraging-experiment-2.xml
launch_argos -nc xml/foraging-experiment-3.xml

