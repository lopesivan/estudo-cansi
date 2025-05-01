# ****************************************************************************
# *                                                                          *
# *  list.mak                                                                *
# *  ========                                                                *
# *                                                                          *
# ****************************************************************************

# ****************************************************************************
# *                                                                          *
# *  Define the name of the makefile.                                        *
# *                                                                          *
# ****************************************************************************

MAKNAM = list.mak

# ****************************************************************************
# *                                                                          *
# *  Define the Lua library directories in which to search for library files.*
# *                                                                          *
# ****************************************************************************

LUA_ROOT        = $(shell brew --prefix)
LUA_EXEC_PREFIX = $(LUA_ROOT)/opt/lua
LUA_LIBDIR      = $(LUA_EXEC_PREFIX)/lib
LUA_INCLUDEDIR  = $(LUA_EXEC_PREFIX)/include/lua

# ****************************************************************************
# *                                                                          *
# *  Define the directories in which to search for library files.            *
# *                                                                          *
# ****************************************************************************

LIBDRS = -L$(LUA_LIBDIR) -llua -lm -ldl

# ****************************************************************************
# *                                                                          *
# *  Define the directories in which to search for include files.            *
# *                                                                          *
# ****************************************************************************

INCDRS = -I$(ALGOWC_TOPDIR)/include -I$(LUA_INCLUDEDIR)

# ****************************************************************************
# *                                                                          *
# *  Define the library files.                                               *
# *                                                                          *
# ****************************************************************************

LIBFLS =

# ****************************************************************************
# *                                                                          *
# *  Define the source files.                                                *
# *                                                                          *
# ****************************************************************************

SRCFLS = list_lua.c\
         $(ALGOWC_TOPDIR)/source/list.c

# ****************************************************************************
# *                                                                          *
# *  Define the object files.                                                *
# *                                                                          *
# ****************************************************************************

OBJFLS = list_lua.o\
         $(ALGOWC_TOPDIR)/source/list.o

# ****************************************************************************
# *                                                                          *
# *  Define the executable.                                                  *
# *                                                                          *
# ****************************************************************************

EXE    = list.so

# ****************************************************************************
# *                                                                          *
# *  Define the compile and link options.                                    *
# *                                                                          *
# ****************************************************************************

CC     = gcc
LL     = gcc
CFLAGS = -Wall
LFLAGS = -fPIC -shared

# ****************************************************************************
# *                                                                          *
# *  Define the rules.                                                       *
# *                                                                          *
# ****************************************************************************

$(EXE): $(OBJFLS)
	$(LL) $(LFLAGS) -o $@ $(OBJFLS) $(LIBDRS) $(LIBFLS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $(INCDRS) $<

all:
	make -f $(MAKNAM) clean
	make -f $(MAKNAM) depend

depend:
	makedepend $(INCDRS) -f $(MAKNAM) $(SRCFLS)
	make -f $(MAKNAM) $(EXE)

test:
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LUA_LIBDIR) ldd list.so

run:
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LUA_LIBDIR) lua list_test.lua

clean:
	-rm $(EXE)
	-rm $(OBJFLS)

# DO NOT DELETE THIS LINE -- make depend depends on it.
