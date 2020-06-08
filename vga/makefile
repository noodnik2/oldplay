

v%.e:	v%.o
	$(CC) -o $@ $? -lvga -lvgagl

v%.s:	v%.e
	cp $< $@
	su -c "(chown root $@; chmod +s $@)"

help:
	@echo *.c
	
clean:
	rm -f *.o *.e *.s
	

