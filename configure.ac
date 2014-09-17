#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([container], [1.0])
AC_CONFIG_AUX_DIR([tools])

LT_INIT
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([src/linked_list.c])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

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
	container.pc
	])
AC_OUTPUT
