
PD_OBJ= pdaxeq.o pdcmde.o pdcmdm.o pdcolor.o pderrmsg.o \
	pderror.o pderrst.o pdflush.o pdgetch.o \
	pdgetech.o pdtty.o pdinit.o pdmdcmap.o pdmdcolo.o \
	pdmddbg.o pdmdtty.o pdmdhi.o pdmdinit.o pdmdqwh.o \
	pdmdspen.o pdmdwin.o pdterm.o pdwrstty.o \
	termio.o wrstty.o

pdtest:	pdtest.o $(PD_OBJ)
	cc -o pdtest pdtest.o $(PD_OBJ)

pdtest.o:	compiler.h pd.h

termio.o:	compiler.h

wrstty.o:	compiler.h

pdaxeq.o:	compiler.h pd.h pderr.h 

pdcmde.o:	compiler.h pd.h pderr.h 

pdcmdm.o:	compiler.h pd.h pderr.h 

pdcolor.o:	pd.h pdcolor.h 

pderrmsg.o:	pd.h pdcolor.h 

pderror.o:	pd.h pderr.h 

pderrst.o:	compiler.h pd.h 

pdflush.o:	compiler.h 

pdgetch.o:	compiler.h pd.h pderr.h termsig.h 

pdgetech.o:	compiler.h pd.h pdgetech.h 

pdtty.o:	compiler.h pd.h pderr.h pdgetech.h

pdinit.o:	compiler.h pd.h 

pdmdcmap.o:	compiler.h 

pdmdcolo.o:	compiler.h pd.h setdisp.h 

pdmdspen.o:	compiler.h pd.h

pdmddbg.o:	compiler.h pd.h 

pdmdtty.o:	compiler.h tty.h 

# pdmdhi.o:	compiler.h dosfiles.h 

pdmdinit.o:	compiler.h pd.h 

pdmdqwh.o:	compiler.h 

pdmdwin.o:	

pdterm.o:	compiler.h pd.h pderr.h 

pdwrstty.o:	compiler.h pd.h 

