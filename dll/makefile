#

app:	main.o libdll.so
	$(CC) -o $@ $< -L. -ldll

dll.o:	dll.h
	$(CC) -nostartfiles -fPIC -g -c $*.c

main.o:
	$(CC) -g -c $*.c

libdll.so:	dll.o
	$(CC) -nostartfiles -g -shared -Wl,-soname,libdll.so -o libdll.so dll.o

clean:
	rm -f main.o dll.o libdll.so app

#
