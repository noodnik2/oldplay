#include	<stdio.h>

#define		MAX_ARGBUF	40
#define		MAX_ARGS	5

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char argbuf[MAX_ARGBUF] ;
	char *args[MAX_ARGS] ;
	int i, j, nargs, rc ;

	for (i= 1; i< argc; i++) {
		rc= parse_lpcmd(
			argv[i],
			&nargs, args, MAX_ARGS,
			argbuf, MAX_ARGBUF
		) ;
		printf("rc=%d, nargs=%d\n", rc, nargs) ;
		for (j= 0; j< nargs; j++) {
			printf("%d: %s\n", j, args[j]) ;
		}
	}

}

void nerr(msg, n)
char *msg ;
int n ;
{
	fprintf(stderr, "tf: %s, code=%d\n", msg, n) ;
}

void mrlp_abort(msg)
char *msg ;
{
	fprintf(stderr, "tf: aborted: %s\n", msg) ;
	exit(1) ;
}

void perr(msg)
char *msg ;
{
	fprintf(stderr, "tf: ") ;
	fflush(stderr) ;
	perror(msg) ;
}
