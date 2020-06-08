
#
#	Makefile for cppl.lib
#

PRODUCT=cppl.lib
OBJLIST=mystr.obj mystrl.obj a21flog.obj iniacc.obj

INCOPTS=-I ../platform

.SUFFIXES:	.cpp
CPP=$(CPP) /AL

.cpp.obj:
	$(CPP) -c $(INCOPTS) $*.cpp

all: $(PRODUCT)

clean:
	for %%X in ($(PRODUCT) $(OBJLIST)) do del %%X

$(PRODUCT):	$(OBJLIST)
	del $(PRODUCT)
	lib $(PRODUCT) $(OBJLIST) ;

mystr.obj:	mystr.h 

mystrl.obj:	mystrl.h 

a21flog.obj:	a21flog.h

iniacc.obj:	iniacc.h listcls.h

# end of makefile.msc
