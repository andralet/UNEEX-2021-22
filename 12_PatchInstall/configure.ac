#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([guess], [0.0.1], [test@example.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])
# GETTEXT_PACKAGE=guess
# AC_SUBST(GETTEXT_PACKAGE)
# ALL_LINGUAS="ru"
AC_CONFIG_SRCDIR([src/guess.c])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC
AC_CHECK_PROGS([DOXYGEN], [doxygen])
AC_CHECK_PROGS([HELP2MAN], [help2man])
DX_INIT_DOXYGEN([argp])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h string.h locale.h unistd.h libintl.h argp.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile docs/Makefile docs/Doxyfile po/Makefile.in])
AC_OUTPUT
