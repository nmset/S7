#!/bin/bash
if [ ! -x 2_makemo.sh ]
then
    echo "Wrong working directory. Please cd to the directory containing this script."
    exit 0
fi

DOMAIN=fr

COMPONENT_NAME=S7
msgfmt $DOMAIN/${COMPONENT_NAME}.po -o $DOMAIN/${COMPONENT_NAME}.mo

exit 0

