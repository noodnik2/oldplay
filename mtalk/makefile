#	Revision:		1
#

#
#
#

UTLDIR=../libutl
CFGDIR=../config
DISTCGI_DIR=$(HOME)/website/cgi-bin
DISTSYS_DIR=/usr/local/bin

#
#
#

MTALK=mtalk
MTALK_OBJ=mtalk.o

COPTS=-I $(CFGDIR) -I $(UTLDIR)

PRODUCTS=$(MTALK)
COPY_SCRIPTS=mtnotify mtcheck

all:	$(PRODUCTS)

dist:	$(PRODUCTS)
	cp $(PRODUCTS) $(DISTSYS_DIR)
	cp $(COPY_SCRIPTS) $(DISTSYS_DIR)

clean:
	rm -f $(PRODUCTS) *.o core

.c.o:
	cc -c $(COPTS) $*.c

$(MTALK):	$(MTALK_OBJ)
	cc -o $@ $(MTALK_OBJ) -L$(UTLDIR) -lutl

#
#
#

mtalk.o:	mtalk.h

# end of makefile
