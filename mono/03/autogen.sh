#!/bin/sh

mkdir -p build-aux m4
libtoolize --install --copy # no macOS use: glibtoolize --install --copy
aclocal -I m4
autoconf
automake --add-missing --copy

exit 0
