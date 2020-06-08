
#include	<stdio.h>

void main(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;
	double x, sum ;

	sum= (double) 0.0e0 ;
	for (i= 1; i< argc; i++) {
		if (sscanf(argv[i], "%lf", &x) != 1) {
			fprintf(stderr, "bad number: %s\n", argv[i]) ;
			exit(1) ;
		}
		sum+= x ;
	}
	printf("%lf\n", sum) ;
}
	
