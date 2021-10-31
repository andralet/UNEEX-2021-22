#!/bin/bash
if strace -o/dev/null -efault=$2:error=$3:when=$4 ./move $5 $6 2>&1 | cmp - $7; \
	then strace -o/dev/null -efault=$2:error=$3:when=$4 ./move $5 $6 2> /dev/null; if test $? -eq $8; \
		then if test -e $5; then echo "$1 - DONE."; else echo "$1 - FAULT."; fi \
		else echo "$1 - FAULT."; \
	fi \
	else echo "$1 - FAULT."; \
fi
