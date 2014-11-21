#!/bin/bash

# Quick 'n dirty script to run the binary, using "private" hidapi

HIDAPI_LIB="../../hidapi/linux/libhidapi-hidraw.so"

if [ ! -f "$HIDAPI_LIB" ]; then
    echo "ERROR: can't find $HIDAPI_LIB."
    echo "Quick doc for a quick test:"
    echo " - get hidapi (https://github.com/signal11/hidapi)"
    echo " - put it next to eizomon dir (eizomon and hidapi at the same level)"
    echo " - ignore bootstrap / configure"
    echo " - go to hidapi/linux/ directory"
    echo " - make -f Makefile-manual"
    exit 1
fi

if [ ! -f eizomon ]; then
    echo "Please, compile eizomon"
    exit 1
fi

if [ ! -L libhidapi-hidraw.so.0 ]; then
    ln -s "$HIDAPI_LIB" libhidapi-hidraw.so.0
fi

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./eizomon "$@"
