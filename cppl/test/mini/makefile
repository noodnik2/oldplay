
ROOT_DIR=../../..
CPPL_DIR=$(ROOT_DIR)/cppl
CPPL_LIB=$(CPPL_DIR)/cppl.a

.SUFFIXES:	.cpp

.cpp.o .c.o:
	g++ -c -I $(CPPL_DIR) $<

all:	wrtest wrtest2

clean:
	rm -f wrtest wrtest.o wrtest2 wrtest2.o wrap.o

new:	clean
	make

wrtest:	wrtest.o
	g++ -o wrtest wrtest.o $(CPPL_LIB)

wrtest2:	wrtest2.o wrap.o
	g++ -o $@ wrtest2.o wrap.o $(CPPL_LIB)

#
wrtest2.o:	wrap.h

wrap.o:		wrap.h

# end of makefile for iniacc test
