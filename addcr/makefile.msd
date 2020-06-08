#	Revision:      		2
#

#
#
#

DESTDIR=c:\bin

PROG=addcr.exe
PROG_OBJ=addcr.obj clgetopt.obj dskcache.obj
CLEANLIST=$(PROG) $(PROG_OBJ) *.bak *.map
CFLAGS=
LINKADD=setargv.obj
LFLAGS=/noe /STACK:10240
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

addcr.obj:	compiler.h clgetopt.h 

clgetopt.obj:	compiler.h clgetopt.h 

dskcache.obj:	compiler.h 

# end of makefile for addcr.exe
