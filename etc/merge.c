#include	<stdio.h>

static void mrg() ;
static FILE *fop() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	if (argc< 3) {
		fprintf(stderr, "specify two filenames\n") ;
		exit(1) ;
	}

	mrg(argv[1], argv[2]) ;
}

static void mrg(fn1, fn2)
char *fn1, *fn2 ;
{
	FILE *fp1, *fp2 ;
	char *s1, buf1[1024] ;
	char *s2, buf2[1024] ;

	fp1= fop(fn1) ;
	fp2= fop(fn2) ;

	do {

		s1= fgets(buf1, sizeof(buf1), fp1) ;
		s2= fgets(buf2, sizeof(buf2), fp2) ;
		printf("<< %s", (s1 != (char *)NULL)? s1: "(past eof)") ;
		printf(">> %s\n", (s2 != (char *)NULL)? s2: "(past eof)") ;

	} while( (s1 != (char *)NULL) || (s2 != (char *)NULL) ) ;

	fclose(fp1) ;
	fclose(fp2) ;

} 

static FILE *fop(fn)
char *fn ;
{
	FILE *fp ;

	if ((fp= fopen(fn, "r")) == (FILE *)NULL) {
		fprintf(stderr, "can't open: %s\n", fn) ;
		exit(1) ;
	}
	return(fp) ;
}
