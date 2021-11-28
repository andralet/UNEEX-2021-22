TESTS=tests
check_PROGRAMS=tests

tests_SOURCES=tests.c
tests_CFLAGS=-I$(top_builddir)/src -fopenmp @CHECK_CFLAGS@
tests_LDADD=$(top_builddir)/libbuf.la @CHECK_LIBS@

.ts.c:
	checkmk $< > $@
