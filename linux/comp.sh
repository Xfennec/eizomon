#!/bin/bash

# Dirty compile script, until a I write a real Makefile

gcc -I../../hidapi/hidapi/ -I../src/ ../src/eizo_ctrl.c eizomon.c ../../hidapi/linux/libhidapi-hidraw.so -o eizomon
