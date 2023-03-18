#!/bin/sh

aclocal
automake --add-missing --force-missing --copy --foreign
autoreconf

exit 0


