
INSTDIR=/usr/local/bin
INSTMOD=755

PW_EXE= pw
PW_OBJ=	pw.o pw_doit.o pw_cmpp.o pw_entry.o pw_pct.o \
	pw_getp.o pw_gp_0.o \
	pw_inter.o pw_tio.o \
	clgetopt.o

$(PW_EXE):	$(PW_OBJ)
	cc -o $@ $(PW_OBJ)

install: $(PW_EXE)
	cp $(PW_EXE) $(INSTDIR)
	chmod $(INSTMOD) $(INSTDIR)/$(PW_EXE)	

clean:
	rm -f $(PW_OBJ) $(PW_EXE) core

#
#	dependencies
#

clgetopt.o:	compiler.h clgetopt.h 

pw.o:		compiler.h clgetopt.h pw.h 

pw_cmpp.o:	compiler.h pw.h pw_proc.h 

pw_doit.o:	compiler.h pw.h pw_proc.h pw_inter.h 

pw_entry.o:	compiler.h pw.h pw_proc.h 

pw_getp.o:	compiler.h pw.h pw_proc.h pw_scan.h 

pw_gp_0.o:	compiler.h pw.h pw_proc.h pw_scan.h 

pw_gp_0.o:	compiler.h pw.h pw_proc.h pw_scan.h compiler.h clgetopt.h

pw_inter.o:	compiler.h pw.h pw_inter.h 

pw_pct.o:	compiler.h pw.h pw_proc.h pw_pct.h 

pw_tio.o:	compiler.h 

