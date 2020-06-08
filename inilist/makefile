
CXXFLAGS:=$(CXXFLAGS) -I../cppl
LDFLAGS:=$(LDFLAGS) -L../cppl -lcppl
#LDFLAGS:=$(LDFLAGS) ../cppl/*.o

#LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

all:	conv

test:	test.o
	$(CXX) -o $@ test.o -L../cppl -lcppl

conv:	conv.o
	$(CXX) -o $@ conv.o -L../cppl -lcppl

clean:
	rm -f test.o test
