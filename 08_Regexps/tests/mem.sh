#!/bin/bash
if strace -o/dev/null -efault=brk:error=ENOMEM -efault=mmap:error=ENOMEM:when=8+ ../esub "fugsj[gf" "fhsgdfd" "afhdsk" 2>/dev/null; test $? -ne 3; then exit 1; else exit 0; fi
