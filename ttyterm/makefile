SHELL=/bin/sh
PROG=ttyterm
DESTDIR=/usr/local/bin
DGRP=bin

PROG_OBJ= term.o tcmd.o termfile.o termfio.o clgetopt.o uterm.o ttyio.o

CFLAGS= -O

.c.o:
	cc -c ${CFLAGS} $*.c

${PROG}: ${PROG_OBJ}
	cc -o ${PROG} ${CFLAGS} ${PROG_OBJ}
	@echo $@: created

install: ${PROG}
	cp ${PROG} ${DESTDIR}
	strip ${DESTDIR}/${PROG}
	chmod 775 ${DESTDIR}/${PROG}
	chgrp ${DGRP} ${DESTDIR}/${PROG}
	@echo $@: ${DESTDIR}/${PROG}

clean:
	rm -f ${PROG} *.o core

clgetopt.o:	compiler.h clgetopt.h 

tcmd.o:		compiler.h term.h 

term.o:		compiler.h clgetopt.h term.h ttyio.h 

termfile.o:	compiler.h clgetopt.h uterm.h utset.h term.h 

termfio.o:	compiler.h clgetopt.h term.h ttyio.h 

ttyio.o:	compiler.h clgetopt.h uterm.h utset.h ttyio.h 

uterm.o:	compiler.h uterm.h utset.h 

