#!/bin/bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT_DIR="$( cd "${SCRIPT_DIR}/../" && pwd )"
ARGOS_INCLUDE_DIR=/usr/include/argos2

# Use Gnu find on Macs
# Should probably use a more robust way to ensure that we are actually
# using Gnu find...
if [ -x /opt/local/bin/gfind ]
then
    FIND=/opt/local/bin/gfind
else
    FIND=find
fi

echo "Removing old tags File."
rm -f ${ROOT_DIR}/TAGS

echo "Building tags file."
# ctags -e --recurse=yes --verbose=yes -IREGISTER_CONTROLLER -IREGISTER_LOOP_FUNCTIONS -IREGISTER_QTOPENGL_USER_FUNCTIONS --exclude=CMakeLists.txt ${ROOT_DIR}/controllers ${ROOT_DIR}/loop_functions $ARGOS_INCLUDE_DIR
# ${FIND} ${ROOT_DIR}/controllers ${ROOT_DIR}/loop_functions $ARGOS_INCLUDE_DIR -type f -a -iregex '.*\.\(h\|hpp\|hxx\|c\|cpp\|cxx\)' -print0 | xargs -0 ctags -L- -e --verbose=yes -IREGISTER_CONTROLLER -IREGISTER_LOOP_FUNCTIONS -IREGISTER_QTOPENGL_USER_FUNCTIONS
${FIND} ${ROOT_DIR}/controllers ${ROOT_DIR}/loop_functions $ARGOS_INCLUDE_DIR -type f -a -iregex '.*\.\(h\|hpp\|hxx\|c\|cpp\|cxx\)' -print0 | xargs -0 etags
