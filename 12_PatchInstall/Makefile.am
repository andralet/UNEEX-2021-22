SUBDIRS=po . docs

CFLAGS=-Wall -Wextra -DLOCALE_PATH='"$(localedir)"'
bin_PROGRAMS=guess
guess_SOURCES=src/guess.c src/roman.c src/roman.h src/argp.c src/argp_data.h
man1_MANS=guess.1

%.1: %
	$(HELP2MAN) --no-info --output=$@ ./$<

gitclean:
	git clean -df

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = config.rpath m4/ChangeLog
