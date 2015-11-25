ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS= -Iincludes
EXTRA_DIST=README.md

lib_LTLIBRARIES=libutils.la
libutils_la_SOURCES=\
	src/arraylist.c \
	src/chunked_string.c \
	src/linked_list.c \
	src/chunked_list.c \
	src/hashmap.c \
	src/string.c \
	src/watch.c \
	src/bus.c

pkginclude_HEADERS=includes/utils/*.h

EXTRA_PROGRAMS=test
test_SOURCES=src/test.c
test_LDADD=libutils.la

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=utils.pc

distclean-local:
	-rm -rf Makefile.in aclocal.m4 autom4te.cache/ tools/*
