/*

	(void) chk_fn(infn, outfn) ;

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

extern co_parms cops ;

void chk_fn(infn, outfn)
char *infn, *outfn ;
{
	char tempfn[FN_SIZE+1] ;

	strcpy(outfn, cops.chkdir) ;
	if (outfn[0] != '\0') {
		strcat(outfn, "/") ;
	}
	strcat(outfn, infn) ;
}
