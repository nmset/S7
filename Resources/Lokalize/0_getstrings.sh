#!/bin/bash
if [ ! -x 0_getstrings.sh ]
then
    echo "Wrong working directory. Please cd to the directory containing this script."
    exit 0
fi

COMPONENT_NAME=S7

SRC=../../
DOMAIN=fr
DEST=$DOMAIN/${COMPONENT_NAME}.po
[ ! -d $DOMAIN ] && mkdir $DOMAIN
[ -f $DEST ] && JOIN="-j"
[ -f $DEST ] && cp $DEST $DEST.bak-$(date +%F-%T)
[ ! -f $DEST ] && touch $DEST

xgettext --keyword=_ -d $DOMAIN $JOIN -o $DEST --c++ --from-code=UTF-8 $(find $SRC -type f  -name "*.cpp")
xgettext --keyword=_ -d $DOMAIN -j -o $DEST --c++ --from-code=UTF-8 $(find $SRC -type f -name "*.h")

exit 0
