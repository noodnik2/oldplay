#include	<stdio.h>

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char c, buf[9120] ;
	int way ;

	way= 0 ;

	if (argc> 1) {
		if (strcmp(argv[1], "char") == 0) {
			way= 1 ;
		}
		else if (strcmp(argv[1], "string") == 0) {
			way= 0 ;
		}
		else {
			fprintf(stderr, "t3: error: say 'char' or 'string'\n") ;
			exit(1) ;
		}
	}

	if (way == 1) {
		do {
			read(0, &c, 1) ;
			fprintf(stderr, "t3: '%c'\n", c) ;
		} while( c != 'e' ) ;
	} 

	if (way == 0) {
		do {
			gets(buf) ;
			fprintf(stderr, "t3: '%s'\n", buf) ;
		} while(strcmp(buf, "exit") != 0) ;
	}

	fprintf(stderr, "t3: done\n") ;
	exit(0) ;
}
