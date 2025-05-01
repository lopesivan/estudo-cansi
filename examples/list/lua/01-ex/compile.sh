#!/usr/bin/env bash

lua_version=$(luaenv version-name)

R=$lua_version
V=${R%.?}

prefix=$(luaenv root)/${lua_version}
echo $dir_target
INSTALL_BIN=${prefix}/bin
INSTALL_INC=${prefix}/include/lua
INSTALL_LIB=${prefix}/lib
INSTALL_MAN=${prefix}/share/man/man1
INSTALL_LMOD=${prefix}/share/lua/${V}
INSTALL_CMOD=${prefix}/lib/lua/${V}
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include/lua

cat <<EOF
Name: Lua
Description: An Extensible Extension Language
Version: $R
Requires:
Libs: -L${libdir} -llua -lm -ldl
Cflags: -I${includedir}
EOF
