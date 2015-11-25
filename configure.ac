#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([utils], [1.0])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([tools])
AC_CONFIG_SRCDIR([src/linked_list.c])

LT_INIT
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AX_PTHREAD([], AC_MSG_ERROR([pthread not found]))
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CC="$PTHREAD_CC"

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset strdup])

AC_CONFIG_FILES([
	Makefile
	utils.pc
	])
AC_OUTPUT
