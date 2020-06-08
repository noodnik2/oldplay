PROG = xdump
OBJECTS = \
	xdump.o seb2asc.o

#comment
$(PROG): $(OBJECTS)
	cc $(OBJECTS) -o $(PROG)
	strip $(PROG)

clean:
	rm *.o xdump 
