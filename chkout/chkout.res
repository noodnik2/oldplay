;
;	This is the chkout.res "resource" file.
;	Obviously, it can contain comments and blank
;	leading lines.  The data section begins with
;	a line beginning with the word "id", followed
;	by the "ID name" of the file, "resource", and
;	then the maximum number of entries the file can
;	contain.  After the ID line, there follows up
;	to the maximum number of entries, consisting
;	of at least four word values (device name,
;	user id, group id, mode spec), and optionally
;	a list of group id's.  Blank lines may exist
;	in this section of the file as well, but com-
;	ments are not allowed.
;
;	White space is defined as tabs or spaces.
;

id	resource 	100

/dev/term/s00	root	tty	620
/dev/term/s01	root 	tty	620
/dev/term/s02	mdr  	tty	620	usr,bin,tty,other	    	
/dev/term/s03	root 	tty	620
/dev/term/s04	root 	tty	620
/dev/term/s05	root 	tty	620
resource1	mdr	other	644
resource2	mdr	other	644
resource3	mdr	tty  	644
resource4	mdr	bin  	644
resource5	mdr	other	644
