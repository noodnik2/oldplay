#include	<stdio.h>

#include	"compiler.h"
#include	"substr.h"

static void movestr() ;

void substr(str, spp)
char *str ;
struct sub_item *spp ;
{
	int i, l, lb, ls, ld ;
	char *base_str, *search_str ;
	char subbuf[SUBVAL_MAXLEN+1] ;
	
	base_str= str ;			lb= strlen(base_str) ;
	search_str= spp->search_str ;	ls= strlen(search_str) ;

	for (i= 0;; i+= l, lb+= l-ls) {
		for (ld= lb - ls;; i++) {
			if (i> ld) return ;
			if (memcmp(base_str+i, search_str, ls) == 0) break ;
		}
		spp->subproc(subbuf, &l, spp->format_str, spp->subval) ;
		movestr(base_str+i+ls, base_str+i+l, lb-i-ls+1) ;
		memcpy(base_str+i, subbuf, l) ;
	}
	/*NOTREACHED*/
}

static void movestr(from_str, to_str, len)
char *from_str, *to_str ;
int len ;
{
	register int i ;
	
	if (len<= 0) return ;
	if (from_str< to_str) {			/* move up: descending */
		for (i= len-1; i>= 0; i--) {
			to_str[i]= from_str[i] ;
		}
	}
	else {					/* move down: ascending */
		for (i= 0; i< len; i++) {
			to_str[i]= from_str[i] ;
		}
	}
}
