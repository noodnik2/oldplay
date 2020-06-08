/*


*/

#include	<stdio.h>

#include	"../compiler/compiler.h"
#include	"../getopt/clgetopt.h"
#include	"../ebcdic/e2amain.h"

struct e2aopt e2ao ;

void main(argc, argv)
int argc;
char *argv[];
{
	int i;
	int e, a ;

	clinit(argv);
	while( (i= clgetopt(argc, argv, "Dq")) != -1 ) {
		switch(i) {
			case 'D':	e2ao.debug= 1;			break;
			case 'q':	e2ao.quiet= 1;			break;
			default:	exit(1);			break;
		}
	}

	if (cloptind != argc) {
		clerror("use this command as a filter");
		exit(1);
	}

	while( (e= getchar()) != EOF ) {
		a= sebc2asc(e);
		if (e2ao.debug) {
			fprintf(stderr, "e(%d) => a(%d)\n", e, a);
		}
		putchar( a ) ;
	}

	exit(0);
}

