/*	Revision:	1
*/

#include	"browse.h"

#if 0
	#define		RECSIZE	512
#else
#define		RECSIZE		RECSIZEMAX
#endif


/*
	expands a line that contains tabs.  replaces tab characters
	with a number of blanks, from 1 to 7.

	returns nonzero value if output buffer overflow error.
*/
static int xpand(buf, len)
unsigned char *buf ;
int len ;
{
	int j, k, t ;
	unsigned char c, xpbuf[RECSIZE+1] ;

	j= k= t= 0;
	while((k< len) && (j< RECSIZE)) {
		if (t> 0) { c= ' '; t--; }
		else c= buf[k++] ;
		if (c == '\t') t= (8-(j&7)) ;
		else xpbuf[j++]= c ;
	}	
	xpbuf[j++]= '\0' ;
	memcpy(buf, xpbuf, j) ;
	return(j == RECSIZE) ;
}
