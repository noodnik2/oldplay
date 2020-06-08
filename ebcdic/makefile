PROG=ebcdic
DESTDIR= /usr/local/bin
DESTGRP=bin

CFLAGS= -O

.c.o:
	cc -c ${CFLAGS} $*.c
	- mv -f $(@F) $(@D)

PROG_HDR= \
../compiler/compiler.h \
../ebcdic/ebcdic.h

PROG_SRC= \
../getopt/clgetopt.c \
../ebcdic/ebchelp.c \
../ebcdic/ebcdic.c \
../ebcdic/ebcmain.c

PROG_OBJ= \
../getopt/clgetopt.o \
../ebcdic/ebchelp.o \
../ebcdic/ebcdic.o \
../ebcdic/ebcmain.o

${PROG}: ${PROG_OBJ}
	cc -o $@ ${CFLAGS} ${PROG_OBJ}

install:	${PROG}
	cp ${PROG} ${DESTDIR}
	strip ${DESTDIR}/${PROG}
	chmod 775 ${DESTDIR}/${PROG}
	chgrp ${DESTGRP} ${DESTDIR}/${PROG}

clean:
	rm -f *.o core ${PROG}

${PROG_OBJ}: ${PROG_SRC} ${PROG_HDR}

