/*

	chk_rec.c			Record functions


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

extern char *strchr() ;

static char *ltrim() ;
static char *rtrim() ;

char chk_recspace[]= " \t" ;

#if 1
#define	ISWS(c)	(strchr(chk_recspace, c) != (char *)NULL)
#else
#define	ISWS(c)	(((c)==chk_recspace[0]) || ((c)==chk_recspace[1]))
#endif

char *chk_frecstrip(frec)
char *frec ;
{
	int l ;

	l= strlen(frec) ;
	if (frec[l-1]=='\n') {
		frec[--l]= '\0' ;
	}
	return(frec) ;
}

void chk_recword(word, string, ds)
char *word, *string, *ds ;
{
	register char *p ;

	p= ltrim(string) ;
	while((*p) != '\0') {
		if (ds != (char *)NULL) {
			if (strchr(ds, *p) != (char *)NULL) break ;
		}
		else {
			if (ISWS(*p)) break ;
		}
		(*word++)= (*p++) ;
	}
	while((*p) != '\0') (*string++)= (*p++) ;
	(*string)= (*word)= '\0' ;
}

char *chk_recltrim(string)
char *string ;
{
	return(ltrim(string)) ;
}

char *chk_recrtrim(string)
char *string ;
{
	return(rtrim(string)) ;
}

/*
	Internal subroutines
*/

static char *ltrim(string)
char *string ;
{
	while((*string) != '\0' && ISWS(*string)) string++ ;
	return(string) ;
}

static char *rtrim(string)
char *string ;
{
	int c, l ;

	for (l= strlen(string); l> 0; l--) {
		c= string[l-1] ;
		if (!ISWS(c)) break ;
	}
	string[l]= '\0' ;
	return(string) ;
}
