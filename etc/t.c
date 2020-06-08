#include	<stdio.h>
#include	"compiler.h"

main(argc, argv)
int argc ;
char *argv[] ;
{
	int rc ;

	if (argc< 2) {
		fprintf(stderr, "dummy\n") ;	
		exit(1) ;
	}

	rc= systtycmd(argv[1]) ;

	fprintf(stdout, "stdout: this is message to stdout\n") ;
	fprintf(stderr, "stderr: rc from sys(%s) is %d\n", argv[1], rc) ;
	exit(0) ;
}
