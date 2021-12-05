#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([guess], [0.0.1], [test@example.com])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/guess.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h string.h locale.h unistd.h libintl.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile po/Makefile])
AC_OUTPUT
