# $Id: Makefile,v 1.1 2012-11-16 18:04:00-08 - - $

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99
EXECBIN   = hashfn
HASHSRC   = hashfn.c ../code/strhash.c
LISFILES  = hashfn.c ../code/strhash.h ../code/strhash.c \
	    Makefile pspell.perl
LISTING   = Listing.misc.ps
HASHOUT   = hashfn.out

TESTDATA  = 0 9 A Z a z foo bar baz qux \
            quux quuux quuuux quuuuux quuuuuux quuuuuuux quuuuuuuux \
            quuuuuuuuux quuuuuuuuuux quuuuuuuuuuux quuuuuuuuuuuux \
            quuuuuuuuuuuuux quuuuuuuuuuuuuux

all : ${EXECBIN}

% : %.c
	- cid + $<
	- checksource $<
	${GCC} -o $@ ${HASHSRC}

ci : ${LISFILES}
	- checksource ${LISFILES}
	- cid + ${LISFILES}

lis : ${LISFILES} ${HASHOUT}
	mkpspdf ${LISTING} ${LISFILES} ${HASHOUT}

${HASHOUT} : hashfn
	hashfn ${TESTDATA} * >${HASHOUT}
	cat ${HASHOUT}

spotless :
	- rm ${EXECBIN} ${HASHOUT}

