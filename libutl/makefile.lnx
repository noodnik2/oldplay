#	Revision:		9
#

#
#	makefile for libutl	Unix Utility library
#				Copyright (C) 1995-1996, Pointofsale Ltd.
#

CFGDIR=../config

LIBUTL=libutl.a

LIBUTL_OBJ=clgetopt.o 			\
	border.o 			\
	lclprtf.o lclprtf2.o		\
	mkfn.o				\
	istruc0.o istruci.o		\
	inis.o

PRODUCTS=$(LIBUTL)
CIFLAGS=-I $(CFGDIR)

.c.o:
	$(CC) -c $(CIFLAGS) $*.c

all: $(PRODUCTS)

new:
	(make clean ; make)

all.dist dist: all

t:	t.o
	cc -o t t.o -L. -lutl

inistest:	inistest.o libutl.a
	cc -o inistest inistest.o -L. -lutl

$(LIBUTL): $(LIBUTL_OBJ)
	rm -f $(LIBUTL)
	ar rs $@ $(LIBUTL_OBJ)

clean:
	rm -f $(PRODUCTS) *.o

clgetopt.o:	clgetopt.h

mkfn.o:		mkfn.h

border.o:	border.h

lclprtf.o:	lclprtf.h

lclprtf2.o:	lclprtf.h

istruc0.o:	istruc.h

istruci.o:	istruc.h

inis.o:		inis.h istruc.h

# end of makefile for libutl
