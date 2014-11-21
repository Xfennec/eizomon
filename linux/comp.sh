#!/bin/bash

gcc -I../../hidapi/hidapi/ -I../src/ ../src/eizo_ctrl.c eizomon.c ../../hidapi/linux/libhidapi-hidraw.so -o eizomon
