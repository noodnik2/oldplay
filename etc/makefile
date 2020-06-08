
PRODLIST=filedim hebfilt
INSTDIR=/usr/local/bin

all:	$(PRODLIST)

install:	$(PRODLIST)
	cp $(PRODLIST) $(INSTDIR)

clean:
	rm -f $(PRODLIST) *.o

FILEDIM_PROD=filedim
FILEDIM_OBJLIST=filedim.o clgetopt.o

HEBFILT_PROD=hebfilt
HEBFILT_OBJLIST=hebfilt.o clgetopt.o

$(FILEDIM_PROD):	$(FILEDIM_OBJLIST)
	cc -o $(FILEDIM_PROD) $(FILEDIM_OBJLIST)

$(HEBFILT_PROD):	$(HEBFILT_OBJLIST)
	cc -o $(HEBFILT_PROD) $(HEBFILT_OBJLIST)

add.o:	

bo.o:	

bosend.o:	

br.o:	

brktest.o:	

ce.o:	

clgetopt.o:	compiler.h clgetopt.h 

doit.o:	

filedim.o:	compiler.h clgetopt.h 

hebfilt.o:	compiler.h clgetopt.h 

fl.o:	

fpeek.o:	compiler.h clgetopt.h 

generic.o:	compiler.h clgetopt.h 

hear.o:	

merge.o:	

perror.o:	

ra.o:	

rdb.o:	

rmf.o:	

rr.o:	

rr_c.o:	

s.o:	compiler.h 

surprise.o:	

t.o:	compiler.h 

t2.o:	

t3.o:	

t4.o:	

t5.o:	

timer.o:	

x.o:	

