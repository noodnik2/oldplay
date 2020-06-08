
#include	<stdio.h>

void main(argc, argv)
int argc ;
char *argv[] ;
{
	int i, i0, i1 ;
	short unsigned x ;

	if (sscanf(argv[1], "%d", &i0) !=  1) {
		fprintf(stderr, "bad argv[1]\n") ;
		exit(1) ;
	}

	if (sscanf(argv[2], "%d", &i1) !=  1) {
		fprintf(stderr, "bad argv[2]\n") ;
		exit(1) ;
	}

	for (i= i0; i<= i1; i++) {
		x= (unsigned short) i ;
		printf("%d.)\t%04X\n", i, (int)x) ;
	}
}

