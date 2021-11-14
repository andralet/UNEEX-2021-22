#!/bin/sh

if test $# -ne 4; then echo "Bad arguments for test.sh"; exit 2; fi

IN=$1
OUT=$2
GEN=$3
NAME=$4

# prerare output:
${GEN} ${IN} | sed "s/[[:space:]]*${IN}$//" > ${OUT}

# checks:
if ! ((/usr/bin/echo -ne "${NAME} \"" && cat ${IN} && echo "") | ../rhasher -q | sed "s/\n//" | diff -i - ${OUT} > /dev/null); then
	exit 1;
fi;
