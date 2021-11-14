TESTS=1_sha1.sh 1_md5.sh 2_sha1.sh 2_md5.sh

%.sh: %.sh.in
	cat $< > $@
	chmod +x $@

clean:
	rm -f *_sha1.sh *_md5.sh *.out *.log *.trs

disclean: clean
