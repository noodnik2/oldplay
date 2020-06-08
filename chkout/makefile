
SHELL= /bin/sh

CFLAGS= -O

DESTDIR= /usr/local/bin
DESTMOD= 775
DESTGRP= bin
DESTOWN= root

CHKOUT_SRC= chkout.c syntax.c clgetopt.c \
	    chkout_r.c chkout_q.c chkout_c.c chkout_u.c chkout_l.c \
	    chk_err.c chk_rec.c chk_rrf.c chk_rcf.c chk_pw.c \
	    chk_dev.c chk_list.c chk_lock.c chk_time.c chk_stat.c \
	    chk_trap.c chk_fn.c

CHKOUT_OBJ= chkout.o syntax.o clgetopt.o \
	    chkout_r.o chkout_q.o chkout_c.o chkout_u.o chkout_l.o \
	    chk_err.o chk_rec.o chk_rrf.o chk_rcf.o chk_pw.o \
	    chk_dev.o chk_list.o chk_lock.o chk_time.o chk_stat.o \
	    chk_trap.o chk_fn.o

chkout: ${CHKOUT_OBJ}
	cc -o $@ ${CHKOUT_OBJ}

install: chkout
	strip chkout
	mv chkout ${DESTDIR}	
	chmod ${DESTMOD} ${DESTDIR}/chkout
	chgrp ${DESTGRP} ${DESTDIR}/chkout
	chown ${DESTOWN} ${DESTDIR}/chkout
	chmod +s ${DESTDIR}/chkout

chkout.lint: ${CHKOUT_SRC} ${CHKOUT_OBJ}
	     lint ${CHKOUT_SRC} > chkout.lint

clean:
	rm -f chkout ${CHKOUT_OBJ}

chk_dev.o:	compiler.h chkout.h chk_pw.h 

chk_err.o:	compiler.h chkout.h 

chk_fn.o:	compiler.h chkout.h 

chk_list.o:	compiler.h chkout.h 

chk_lock.o:	compiler.h chkout.h 

chk_pw.o:	compiler.h chk_pw.h chk_pw0.h 

chk_rcf.o:	compiler.h chkout.h 

chk_rec.o:	compiler.h chkout.h 

chk_rrf.o:	compiler.h chkout.h chk_pw.h 

chk_stat.o:	compiler.h chkout.h 

chk_time.o:	compiler.h chkout.h 

chk_trap.o:	compiler.h chkout.h 

chkout.o:	compiler.h clgetopt.h chkout.h chk_pw.h 

chkout_c.o:	compiler.h chkout.h chk_pw.h 

chkout_l.o:	compiler.h chkout.h 

chkout_q.o:	compiler.h chkout.h 

chkout_r.o:	compiler.h chkout.h 

chkout_u.o:	compiler.h chkout.h 

clgetopt.o:	compiler.h clgetopt.h 

syntax.o:	compiler.h 

