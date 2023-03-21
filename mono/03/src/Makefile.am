# Nome da biblioteca
lib_LTLIBRARIES = libsum.la

# Lista de arquivos de origem
libsum_la_SOURCES = \
    sum.c

# Opções de compilação e vinculação
#AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = -Wall -Wextra
AM_LDFLAGS = -shared

if CONFIG_MONO
include mono.mk

CLEANFILES = $(OUT)
endif

# Instalação
#include_HEADERS = sumrary.h

