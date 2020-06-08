#	Revision:		5
#

#
#
#

UTLDIR=../libutl
CFGDIR=../config

#
#
#

MKLIST=mklist
MKLIST_EXE=$(MKLIST)
MKLIST_OBJ=mklist.o db.o pe.o tv.o wc.o	\
	txtexp.o abts.o 		\
	subexpr.o evaluate.o		\
	output.o list.o qsort.o

INSTDIR=$(HOME)/website/cgi-bin

COPTS=-I $(CFGDIR) -I $(UTLDIR)

PRODUCTS_EXE=$(MKLIST_EXE)

all:	$(PRODUCTS_EXE)
	-strip $(PRODUCTS_EXE)

dist: $(PRODUCTS_EXE)
	cp $(PRODUCTS_EXE) $(INSTDIR)

.c.o:
	cc -c $(COPTS) $*.c

$(MKLIST_EXE):	$(MKLIST_OBJ)
	cc -o $@ $(MKLIST_OBJ) -L$(UTLDIR) -lutl

clean:
	rm -f $(PRODUCTS_EXE) *.o core

#
#
#

mklist.o:	mklist.h db.h pe.h tv.h wc.h 	\
		subexpr.h evaluate.h txtexp.h	\
		ats.h abts.h output.h list.h 	\
		qsort.h

db.o:		db.h

pe.o:		pe.h txtexp.h abts.h subexpr.h evaluate.h

tv.o:		tv.h

wc.o:		wc.h

subexpr.o:	subexpr.h

evaluate.o:	evaluate.h subexpr.h

ats.o:		ats.h

abts.o:		abts.h ats.h

txtexp.o:	txtexp.h

output.o:	output.h list.h qsort.h

list.o:		list.h

qsort.o:	qsort.h

# end of makefile
