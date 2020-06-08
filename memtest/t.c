#include	<malloc.h>

#define	N		300000
#define	S		78

void main()
{
	int i ;
	static void *pp[N] ;

	printf("allocing %d buffers of %d bytes\n", N, S) ;
	for (i= 0; i< N; i++) {
		pp[i]= malloc(S) ;
		if (!pp[i]) printf("failed: %d\n", i) ;
	}
	printf("freeing\n") ;
	for (i= 0; i< N; i++) {
		if (pp[i]) free(pp[i]) ;
	}
}
