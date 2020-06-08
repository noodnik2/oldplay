

DIST_DIR=$(HOME)/website/cgi-bin

SPL2_EXE=spl2
SPL2_OBJ=spl2.o
MIDIFRM_EXE=nph-midi.cgi
MIDIFRM_OBJ=midifrm.o cgiutl.o
COPY=copy/midiscr copy/mhlzip

PRODUCTS=$(MIDIFRM_EXE) $(SPL2_EXE)
PRODUCT_OBJS=$(MIDIFRM_OBJ) $(SPL2_OBJ)

all:	$(PRODUCTS)

all.dist:	products.dist copy.dist

copy.dist:
	cp $(COPY) $(DIST_DIR)

products.dist dist:	all
	cp $(PRODUCTS) $(DIST_DIR)

shell:
	(cd $(DIST_DIR); sh)

clean:
	rm -f $(PRODUCTS) $(PRODUCT_OBJS) core

$(MIDIFRM_EXE):	$(MIDIFRM_OBJ)
	cc -o $@ $(MIDIFRM_OBJ)

$(SPL2_EXE):	$(SPL2_OBJ)
	cc -o $@ $(SPL2_OBJ)

.c.o:
	cc -c $*.c

# end of makefile
