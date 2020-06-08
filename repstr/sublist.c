/*	Revision:	4
*/

#include        "compiler.h"

#include	<stdio.h>
#include	<string.h>
#include        <memory.h>

#if	0
#include	<io.h>
#endif

#include	"sublist.h"

/*
	internal function template
*/
static int mks(TSSTR *psWord, char *psz) ;
static void prompt(char *pszMsg) ;

/*
	entry points
*/


/*
	NOTE:	this routine leaves the
		"substitution list" file open.
*/
int getsublist(SUBLISTITEM *psli, int maxitems, char *pszFileName)
{
        int i, pflag ;
	char szFrom[SUBSTRMAX_LENGTH+1] ;
	char szTo[SUBSTRMAX_LENGTH+1] ;
	FILE *fp ;

	if (!pszFileName) {
		fp= stdin ;
	}
	else {
		fp= fopen(pszFileName, "r") ;
		if (!fp) return(-1) ;
	}

	pflag= isatty(fileno(fp)) ;
	for (i= 0;; i++) {
		if (pflag) prompt("search string (or EOF)") ;
		if (!fgets(szFrom, SUBSTRMAX_LENGTH, fp)) break ;
		if (pflag) prompt("replace string") ;
                if (!fgets(szTo, SUBSTRMAX_LENGTH, fp)) return(-2) ;
		/* we assume '\n' appended to end of every line! */
		szFrom[strlen(szFrom)-1]= '\0' ;
		szTo[strlen(szTo)-1]= '\0' ;
                if (mks(&psli[i].sFrom, szFrom)) return(-3) ;
                if (mks(&psli[i].sTo, szTo)) return(-4) ;
	}	

	return(i) ;
}

void freesublist(SUBLISTITEM *psli, int nitems)
{
	int i ;

	for (i= 0; i< nitems; i++) {
                free(psli[i].sTo.psz) ;
                free(psli[i].sFrom.psz) ;
	}		
}

/*
	static internal routines
*/

static int mks(TSSTR *psWord, char *psz)
{
        psWord->len= strlen(psz) ;
        psWord->psz= (char *) malloc(psWord->len+1) ;
        if (!psWord->psz) return(-2) ;
        memcpy(psWord->psz, psz, psWord->len+1) ;
        return(0) ;
}

static void prompt(char *pszMsg)
{
	fprintf(stderr, "Enter %s:\n", pszMsg) ;
}

/* end of getlist.c */
