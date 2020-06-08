#	Revision:	1
#

#
#	makefile.uxv			Makefile for repstr
#					Copyright (C) 1995, by Marty Ross
#
#

DESTDIR=.

PROG=repstr
PROG_OBJ=repstr.o tsub.o clgetopt.o sublist.o
CLEANLIST=$(PROG) $(PROG_OBJ) *.bak *.map
CFLAGS=
LINKADD=
LFLAGS=
CL=

.c.o:
	$(CC) -c $(CFLAGS) $*.c

$(PROG): $(PROG_OBJ)
	cc -o $(PROG) $(PROG_OBJ)
	echo $(PROG) created.

install: $(PROG)
	cp $(PROG) $(DESTDIR)/$(PROG)
	echo $(DESTDIR)/$(PROG) installed.

install.nostrip: $(PROG)
	cp $(PROG) $(DESTDIR)/$(PROG)
	echo $(DESTDIR)/$(PROG) installed.

clean:
	rm -f $(CLEANLIST)
	mv compiler.h compiler.ux0
	diff compiler.uxv compiler.ux0

compiler.h:
	cp compiler.uxv compiler.h

repstr.o:      compiler.h clgetopt.h tsub.h sublist.h

tsub.o:       compiler.h tsub.h sublist.h

clgetopt.o:   compiler.h clgetopt.h sublist.h

sublist.o:    compiler.h sublist.h

# end of makefile for repstr
