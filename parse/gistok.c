/*

	gistok.c

	Gets token from input stream, delimited by comma.

	(or space plus comma; see "STRICT_COMMA" macro).

*/


/*
	#includes
*/

#include	"gistok.h"


/*
	#defines
*/

#define	STRICT_COMMA		1


/*
	internal functions
*/

static void eatdribble(gistok_istream_t *pti) ;


/*
	entry points
*/

int gistok(char *tok, int toksize, gistok_istream_t *pti)
{
	int i, c, q ;

	if (!pti) return(-1) ;
	if (!pti->getc) return(-1) ;
	if (!pti->ungetc) return(-1) ;

	q= 0 ;
	while(/*CONSTCOND*/1) {
		c= pti->getc(pti->pUser) ;
		if (c == GISTOKCH_EOF) return(-1) ;
		if (!isspace(c)) {
			if ((c == '\'') || (c == '"')) {
				q= c ;
				break ;
			}
			pti->ungetc(pti->pUser, c) ;
			break ;
		}
	}

	if (q) {
		i= 0 ;
		while(/*CONSTCOND*/1) {
			c= pti->getc(pti->pUser) ;
			if (c == GISTOKCH_EOF) break ;
			if (i< (toksize-1)) {
				tok[i++]= (char) c ;
			}
			if (c == q) {
				eatdribble(pti) ;
				break ;
			}
		}
		tok[i]= '\0' ;
		return(i) ;
	}

	i= 0 ;
	while(/*CONSTCOND*/1) {
		c= pti->getc(pti->pUser) ;
		if (c == GISTOKCH_EOF) break ;
		if (c == ',') break ;
		if (c == '\n') break ;
		if (isspace(c)) {
			eatdribble(pti) ;
			break ;
		}
		if (i< (toksize-1)) {
			tok[i++]= (char) c ;
		}
	}
	tok[i]= '\0' ;
	return(i) ;
}


/*
	internal functions
*/

static void eatdribble(gistok_istream_t *pti)
{
	int c ;

	while(/*CONSTCOND*/1) {
		c= pti->getc(pti->pUser) ;
		if (c == GISTOKCH_EOF) break ;
#if	STRICT_COMMA
		/* this section enforces strict comma delimited */
		if (c == ',') break ;
#else
		/* this section enables space delimited as well */
		if (!isspace(c)) {
			if (c != ',') {
				pti->ungetc(pti->pUser, c) ;
			}
			break ;
		}
#endif
	}
}


/* gistok.c */
