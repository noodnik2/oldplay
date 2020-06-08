#include	<stdio.h>

static void int_handler() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	int rc ;

	if (argc != 2) {
		fprintf(stderr, "must specify single command\n") ;
		exit(1) ;
	}
	
	tinit(int_handler, 1) ;
	rc= system(argv[1]) ;
	tstop() ;

}	

static void int_handler(int_no)
int int_no ;
{
	char *mesg ;

	switch(int_no) {
		case 2:		mesg= "break" ;		break ;
		case 15:	mesg= "terminate" ;	break ;
		default:	mesg= "unknown" ;	break ;
	}

	tstop() ;
	fprintf(stderr, "program interrupted: %s\n", mesg) ;
	exit(1) ;
}	

