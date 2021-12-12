all:
	rm -f $(top_srcdir)/src/Doc.md
	make $(top_srcdir)/src/Doc.md
	$(DOXYGEN) Doxyfile

$(top_srcdir)/src/Doc.md: $(top_srcdir)/guess
	echo "\\\\\\ @mainpage" > $@
	echo "# Program usage info:" >> $@
	$(top_srcdir)/guess --help | sed -E "s/^( *)-/-\1-/g" | sed -E "s/--([[:alnum:]]*)/- -\1:/g" | sed -E "s/$$/\n/g" >> $@
