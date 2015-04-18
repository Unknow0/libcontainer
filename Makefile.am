ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST=README.md

lib_LTLIBRARIES=libcontainer.la
libcontainer_la_SOURCES=\
	src/arraylist.c \
	src/chunked_string.c \
	src/linked_list.c \
	src/chunked_list.c \
	src/hashmap.c \
	src/string.c \
	src/watch.c

pkginclude_HEADERS=container/*.h

EXTRA_PROGRAMS=test
test_SOURCES=src/test.c
test_LDADD=libcontainer.la

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=container.pc

distclean-local:
	-rm -rf Makefile.in aclocal.m4 autom4te.cache/ tools/*
