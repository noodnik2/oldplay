/*

	tstgis.c

	Tests "gistok.c"

*/

#include	<stdio.h>
#include	"gistok.h"

struct rec {
	int ptr ;
	char buf[1024] ;
} ;

static int mygetc(void *fpv)
{
	char c ;
	struct rec *prec ;

	prec= (struct rec *) fpv ;
	c= prec->buf[prec->ptr] ;
	if (c == '\0') return(GISTOKCH_EOF) ;
	prec->ptr++ ;
	return(c) ;
}

static void myungetc(void *fpv, int c)
{
	struct rec *prec ;

	prec= (struct rec *) fpv ;
	if ((prec->ptr)<= 0) return ;
	prec->buf[--(prec->ptr)]= c ;
}

int main()
{
	struct rec r ;
	gistok_istream_t ti= { mygetc, myungetc, &r } ;
	char tok[23] ;
	int i, recno ;

	for (recno= 1; gets(r.buf); recno++) {
		printf("record #%d:\n", recno) ;
		r.ptr= 0 ;
		for (i= 0; gistok(tok, sizeof(tok), &ti)>= 0; i++) {
			printf("\ttok[%d]='%s'\n", i, tok) ;
		}
	}
}

/* end of tstgis.c */
