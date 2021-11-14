#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([rhasher], [1.0], [test@example.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([src/rhasher.c])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([rhash], [rhash_library_init], [], [AC_MSG_ERROR([Missing librhash])])
AC_CHECK_LIB([readline], [readline], [
	AC_SUBST([LIBS], ["${LIBS} -lreadline"])
	AC_CHECK_HEADERS([readline/readline.h readline/history.h])
	AC_ARG_WITH(
		[readline],
        [AS_HELP_STRING([--without-readline], [do not use readline (default - use, if it is accessible on your system)])],
		[AC_MSG_NOTICE([libreadline usage cancelled])],
        [AC_DEFINE([USE_READLINE], [], [Define if you have readline and want to use it])]
	)
], [])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h errno.h ctype.h rhash.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strchr strerror strstr])

AC_CONFIG_FILES([Makefile test/Makefile])
AC_OUTPUT

