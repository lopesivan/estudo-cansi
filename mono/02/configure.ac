# configure.ac

# Process this file with autoconf to produce a configure script.
AC_INIT([sum], [0.02], [ivan@ivanlopes.eng.br],
        [sum], [http://ivanlopes.eng.br/])

# Force autoconf to be at least this version number:
AC_PREREQ([2.68])
LT_PREREQ([2.2])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])

# Enable "automake" to simplify creating makefiles:
AM_INIT_AUTOMAKE([1.11 -Wall -Werror])
# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O

# We use libraries
AC_PROG_RANLIB
AM_PROG_AR
LT_INIT([dlopen])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])
AC_OUTPUT

AC_MSG_RESULT(
[
-------------------------------------------------
${PACKAGE_NAME} Version ${PACKAGE_VERSION}
Prefix: '${prefix}'.
Compiler: '${CC} ${CFLAGS}'
Package features:
Execution:
Now type 'make @<:@<target>@:>@'
where the optional <target> is:
all
- build all binaries
install
- install everything
--------------------------------------------------
])
