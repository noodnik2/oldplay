
# DESTDIR=$(HOME)/bin

DESTDIR=/usr/local/bin
PROG=xm
DGRP=bin
DACC=755

XM_OBJLIST= xm.o

$(PROG):	$(XM_OBJLIST)
	cc -o $@ $(XM_OBJLIST)

install: $(PROG)
	cp $(PROG) $(DESTDIR)/$(PROG)
	strip $(DESTDIR)/$(PROG)
	chmod $(DACC) $(DESTDIR)/$(PROG)
	chgrp $(DGRP) $(DESTDIR)/$(PROG)
	echo $(DESTDIR)/$(PROG) installed.

clean:
	rm -f $(PROG) core *.o

clgetopt.o:	compiler.h clgetopt.h 

xm.o:		compiler.h 

