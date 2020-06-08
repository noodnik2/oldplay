#include	<stdio.h>
#include	"gistok.h"

static int mygetc(void *fpv)
{
	int c ;

	c= getc((FILE *) fpv) ;
	if (c == EOF) return(GISTOKCH_EOF) ;
	return(c) ;
}

static void myungetc(void *fpv, int c)
{
	ungetc(c, (FILE *) fpv) ;
}

int main()
{
	gistok_istream_t ti= { mygetc, myungetc, stdin } ;
	char tok[23] ;
	int i ;

	while(/*CONSTCOND*/1) {
		i= gistok(tok, sizeof(tok), &ti) ;
		printf("i=%d\n", i) ;
		if (i< 0) break ;
		printf("\ttok='%s'\n", tok) ;
	}
}
