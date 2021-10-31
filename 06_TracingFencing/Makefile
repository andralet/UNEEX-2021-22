all: move protect.so

test: run test_strace test_PROTECT

protect.so: protect.c
	cc -shared protect.c -o protect.so -ldl

run: move cleandst
	@make source.txt -s
	@echo "Testing program running correct..."
	@./move source.txt destination.txt
	@if cmp -s source_backup.txt destination.txt; \
		then if test -e source.txt; then echo "FAULT."; else echo "DONE."; fi \
		else echo "FAULT."; \
	fi

test_strace: move
	@make source.txt -s
	@echo "Testing strace injections..."
	@ if ./move 2>&1 | cmp - test1.out; \
		then ./move 2> /dev/null; if test $$? -eq 1; \
			then if test -e source.txt; then echo "1 - DONE."; else echo "1 - FAULT."; fi \
			else echo "1 - FAULT."; \
		fi \
		else echo "1 - FAULT."; \
	fi
	@./run_test.sh 2 openat EPERM 3 ./source.txt ./destination.txt test2.out 2
	@./run_test.sh 3 fstat ENOENT 3 ./source.txt ./destination.txt test3.out 3
	@if strace -o/dev/null -efault=brk:error=ENOMEM:when=3 -efault=mmap:error=ENOMEM:when=8+ ./move ./source.txt ./destination.txt 2>&1 | cmp - test4.out; \
		then strace -o/dev/null -efault=brk:error=ENOMEM:when=3 -efault=mmap:error=ENOMEM:when=8+ ./move ./source.txt ./destination.txt 2> /dev/null; if test $$? -eq 4; \
			then if test -e source.txt; then echo "4 - DONE."; else echo "4 - FAULT."; fi \
			else echo "4 - FAULT.";\
		fi \
		else echo "4 - FAULT."; \
	fi
	@./run_test.sh 5 openat EPERM 4 ./source.txt ./destination.txt test5.out 5
	@./run_test.sh 6 write EIO 2 ./source.txt ./destination.txt test6.out 6
	@./run_test.sh 7 read EIO 2 ./source.txt ./destination.txt test7.out 7
	@./run_test.sh 8 fsync EIO 1 ./source.txt ./destination.txt test8.out 8
	@./run_test.sh 9 close EIO 3 ./source.txt ./destination.txt test9.out 9
	@./run_test.sh 10 unlink EIO 1 ./source.txt ./destination.txt test10.out 10
	@if strace -o/dev/null -efault=close:error=EIO:when=4 ./move ./source.txt ./destination.txt 2>&1 | cmp - test11.out; \
		then make source.txt -s; strace -o/dev/null -efault=close:error=EIO:when=4 ./move ./source.txt ./destination.txt 2> /dev/null; if test $$? -eq 11; \
			then echo "11 - DONE."; \
			else echo "11 - FAULT."; \
		fi \
		else echo "11 - FAULT."; \
	fi
	@echo "Over."

test_PROTECT: move protect.so cleandst
	@make source.txt -s
	@make source_PROTECT_file.txt -s
	@echo "Testing PROTECT library version..."
	@LD_PRELOAD="./protect.so" ./move source.txt destination.txt
	@if cmp -s source_backup.txt destination.txt; \
		then if test -e source.txt; then echo "No PROTECT - FAULT."; else echo "No PROTECT - DONE."; fi \
		else echo "No PROTECT - FAULT."; \
	fi
	@LD_PRELOAD="./protect.so" ./move source_PROTECT_file.txt destination_PROTECT_file.txt 2> /dev/null; \
		if (test $$? -eq 10) && cmp -s source_backup.txt destination_PROTECT_file.txt; \
			then if test -e source_PROTECT_file.txt; then echo "PROTECT - DONE."; else echo "PROTECT - FAULT."; fi \
			else echo "PROTECT - FAULT."; \
		fi
	@echo "Over."

source.txt:
	cp source_backup.txt source.txt

source_PROTECT_file.txt:
	cp source_backup.txt source_PROTECT_file.txt

cleandst:
	rm -f destination.txt destination_PROTECT_file.txt

clean: cleandst
	rm -f move protect.so source.txt source_PROTECT_file.txt
