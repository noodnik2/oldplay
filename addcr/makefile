SHELL= /bin/sh
# DESTDIR=${HOME}/bin
DESTDIR=/usr/local/bin
DGRP=bin
DACC= 755

PROG= addcr
CFLAGS= -g

PROG_OBJ= addcr.o clgetopt.o dskcache.o

${PROG}: ${PROG_OBJ}
	cc -o ${PROG} ${CFLAGS} ${PROG_OBJ}
	echo ${PROG} created.

install: ${PROG}
	cp ${PROG} ${DESTDIR}/${PROG}
	strip ${DESTDIR}/${PROG}
	chmod ${DACC} ${DESTDIR}/${PROG}
	chgrp ${DGRP} ${DESTDIR}/${PROG}
	echo ${DESTDIR}/${PROG} installed.

install.nostrip: ${PROG}
	cp ${PROG} ${DESTDIR}/${PROG}
	chmod ${DACC} ${DESTDIR}/${PROG}
	chgrp ${DGRP} ${DESTDIR}/${PROG}
	echo ${DESTDIR}/${PROG} installed.

clean:
	rm -f ${PROG} core *.o

addcr.o:	compiler.h clgetopt.h 

clgetopt.o:	compiler.h clgetopt.h 

dskcache.o:	compiler.h 

