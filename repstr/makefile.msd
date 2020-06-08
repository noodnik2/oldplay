#	Revision:	2
#

#
#	makefile.msd			Makefile for REPSTR.EXE under MSDOS
#					Copyright (C) 1995, by Marty Ross
#
#

DESTDIR=.

PROG=repstr.exe
PROG_OBJ=repstr.obj tsub.obj clgetopt.obj sublist.obj
CLEANLIST=$(PROG) $(PROG_OBJ) *.bak *.map
CFLAGS=
LINKADD=setargv.obj
LFLAGS=/noe /STACK:20480
CL=

.c.obj:
	$(CC) /c $(CFLAGS) $*.c

$(PROG): $(PROG_OBJ)
	link $(LFLAGS) $(PROG_OBJ) $(LINKADD), $(PROG), $(PROG_MAP), $(LIBS), $(PROG_DEF) ;
	echo $(PROG) created.

install: $(PROG)
	copy $(PROG) $(DESTDIR)\$(PROG)
	echo $(DESTDIR)\$(PROG) installed.

install.nostrip: $(PROG)
	copy $(PROG) $(DESTDIR)\$(PROG)
	echo $(DESTDIR)\$(PROG) installed.

clean:
	for %%x in ($(CLEANLIST)) do del %%x
	copy compiler.h compiler.ms0
	del compiler.h
	fc compiler.msd compiler.ms0

compiler.h:
	copy compiler.msd compiler.h

repstr.obj:      compiler.h clgetopt.h tsub.h sublist.h

tsub.obj:       compiler.h tsub.h sublist.h

clgetopt.obj:   compiler.h clgetopt.h sublist.h

sublist.obj:    compiler.h sublist.h

# end of makefile for repstr.exe
