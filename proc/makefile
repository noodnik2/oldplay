
INSTDIR=/usr/local/bin
INSTMOD=755

PD_EXE= pd
PD_OBJ=	pd.o pd_diag.o pdstat.o pdmem.o pdinfo.o pdpr.o \
	rtrim.o timefmt.o print.o \
	clgetopt.o

$(PD_EXE):	$(PD_OBJ)
	cc -o $@ $(PD_OBJ)

install:
	cp $(PD_EXE) $(INSTDIR)
	chmod $(INSTMOD) $(INSTDIR)/$(PD_EXE)	

clean:
	rm -f $(PD_OBJ) $(PD_EXE) core

#
#	dependencies
#

clgetopt.o:	compiler.h clgetopt.h 

pd.o:		compiler.h clgetopt.h pd.h 

pd_diag.o:	compiler.h clgetopt.h pd.h 

pdinfo.o:	compiler.h clgetopt.h pd.h 

pdmem.o:	compiler.h clgetopt.h pd.h 

pdpr.o:		compiler.h clgetopt.h 

pdstat.o:	compiler.h clgetopt.h pd.h 

print.o:	compiler.h 

rtrim.o:	compiler.h 

timefmt.o:	compiler.h 

