AC_INIT([main], [1.0], [example@example.com])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_CHECK_LIB([xml2], [xmlParseFile])
AC_CHECK_HEADERS([libxml/parser.h], [], [AC_MSG_ERROR([libxml2 headers not found.])])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
