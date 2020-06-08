#	Revision:		1
#

#
#	makefile.lnx			Linux makefile for CPPL library
#
#	for Linux
#

PRODUCT=libcppl.a
OBJLIST=mobj.o mcls.o mclstbl.o		\
	mpa.o mpasort.o mpasrch.o	\
	mini.o mstr.o mstrlst.o mpary.o mparysrt.o mthrow.o

all: $(PRODUCT)

clean:
	rm -r -f $(PRODUCT) $(OBJLIST)

$(PRODUCT):	$(OBJLIST)
	rm -f $(PRODUCT)
	ar -r $(PRODUCT) $(OBJLIST)

mstr.o:		mstr.h 

mstrlst.o:	mstrlst.h 

mini.o:		mini.h mstr.h

# end of makefile
