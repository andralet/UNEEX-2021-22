pkgdata_DATA=ru/LC_MESSAGES/guess.mo

ru/LC_MESSAGES/guess.mo: ru.po
	mkdir -p ru/LC_MESSAGES
	msgfmt -o $@ $<

ru.po: $(top_builddir)/src/guess.c
	xgettext --keyword=_ --add-comments $(top_builddir)/src/guess.c -o messages.pot
	msgmerge --update ru.po messages.pot

clean:
	rm -f ru/LC_MESSAGES/guess.mo messages.pot
