
#include	<stdio.h>

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char c, buf[9120] ;
	int eof, way ;

	eof= 0 ;
	way= 0 ;

	if (argc> 1) {
		if (strcmp(argv[1], "char") == 0) {
			way= 1 ;
		}
		else if (strcmp(argv[1], "string") == 0) {
			way= 0 ;
		}
		else {
			fprintf(stdout, "t4: error: say 'char' or 'string'\n") ;
			exit(1) ;
		}
	}

	if (way == 1) {
		do {
			if (read(0, &c, 1)<= 0) eof= 1 ;
			else {
				fprintf(stdout, "t4: '%c'\n", c) ;
				if (c == '!') break ;
			}
		} while(!eof) ;
	} 

	if (way == 0) {
		do {
			if (gets(buf) == (char *)NULL) eof= 1 ;
			else {
				fprintf(stdout, "t4: '%s'\n", buf) ;
				if (strcmp(buf, "exit") == 0) break ;
			}
		} while(!eof) ;
	}

	if (eof) fprintf(stdout, "t4: premature EOF\n") ;
	else     fprintf(stdout, "t4: done\n") ;

	exit(0) ;
}
