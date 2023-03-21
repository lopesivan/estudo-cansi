#!/bin/sh

autoreconf -i -v && 
    ./configure && 
    make &&
    make -C src/ run

exit 0
