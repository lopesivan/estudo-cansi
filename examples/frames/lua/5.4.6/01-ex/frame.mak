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

MAKNAM = frame.mak

# ****************************************************************************
# *                                                                          *
# *  Define the Lua library directories in which to search for library files.*
# *                                                                          *
# ****************************************************************************

LUA_VERSION     = $(shell luaenv version-name)
LUA_ROOT        = $(shell luaenv root)
LUA_PREFIX      = $(LUA_ROOT)/versions/$(LUA_VERSION)
LUA_LIBDIR      = $(LUA_PREFIX)/lib
LUA_INCLUDEDIR  = $(LUA_PREFIX)/include

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

SRCFLS = frame_lua.c\
         $(ALGOWC_TOPDIR)/source/list.c \
         $(ALGOWC_TOPDIR)/source/frames.c

# ****************************************************************************
# *                                                                          *
# *  Define the object files.                                                *
# *                                                                          *
# ****************************************************************************

OBJFLS = frame_lua.o\
         $(ALGOWC_TOPDIR)/source/list.o \
         $(ALGOWC_TOPDIR)/source/frames.o

# ****************************************************************************
# *                                                                          *
# *  Define the executable.                                                  *
# *                                                                          *
# ****************************************************************************

EXE    = frame.so

# ****************************************************************************
# *                                                                          *
# *  Define the compile and link options.                                    *
# *                                                                          *
# ****************************************************************************

CC     = gcc
LL     = gcc
CFLAGS = -Wall -fPIC
LFLAGS = -shared
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

test: $(EXE)
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LUA_LIBDIR) ldd frame.so

run: $(EXE)
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(LUA_LIBDIR) lua test_frame.lua

clean:
	-rm $(EXE)
	-rm $(OBJFLS)

# DO NOT DELETE THIS LINE -- make depend depends on it.
