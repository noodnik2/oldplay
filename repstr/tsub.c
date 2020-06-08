#include	<stdio.h>
#include	"tsub.h"

/*
	function prototypes

*/
static void subst(TSSTR *psLinePart, TSSTR *psNew, int oldlen, int pos) ;
static int index(TSSTR *psWord, TSSTR *psLine) ;
static int movelblk(char *s, char *d, unsigned len) ;

/*
	entry point
*/

int tsub(
        TSSTR *psOldWord,
        TSSTR *psNewWord,
        TSSTR *psLine,
        int *ip,
        int maxlen
) {
        TSSTR sLinePart ;
        int linePos ;
        int wordFoundAt ;
        int newLineLen ;

        linePos= (*ip) ;
        sLinePart.psz= psLine->psz + linePos ;
        sLinePart.len= psLine->len - linePos ;

        wordFoundAt= index(psOldWord, &sLinePart) ;
        if (wordFoundAt< 0) return(0) ;

        newLineLen= (psLine->len + (psNewWord->len - psOldWord->len)) ;
        if (newLineLen>= maxlen) return(-1) ;

        subst(&sLinePart, psNewWord, psOldWord->len, wordFoundAt) ;
        (*ip)= linePos + wordFoundAt + psNewWord->len ;
        psLine->len= newLineLen ;

	return(1) ;
}

/*
	static subroutines
*/

static void subst(TSSTR *psLinePart, TSSTR *psNew, int oldlen, int pos)
{
        char *psz ;

        psz= psLinePart->psz + pos ;
        movelblk(
                psz + oldlen,
                psz + psNew->len,
                psLinePart->len - pos - oldlen + 1   /* include '\0' */
        ) ;
        memcpy(psz, psNew->psz, psNew->len) ;
        psLinePart->len+= (psNew->len - oldlen) ;
}

static int index(TSSTR *psWord, TSSTR *psLine)
{
        int i ;

        char *pszWord=  psWord->psz ;
        char *pszLine=  psLine->psz ;
        int lWord=      psWord->len ;
        int lSearch=    psLine->len - lWord ;

        for (i= 0; i< lSearch; i++) {
                if (memcmp(pszLine + i, pszWord, lWord) == 0) {
                        return(i) ;
                }
        }

        return(-1) ;
}

/* 
	Note:  This routine is supposed to handle overlapping blocks 
	       correctly.  Experiments with Sun 3/50 compiler indicates 
	       that memcpy() does this.  Experiments with IX386 indicates
	       that it does not.
*/
static int movelblk(
	char *source,
	char *dest,
	unsigned length
) {
/*
	Let     source = address of begin of source
		esrc   = address of end of source move (source + length - 1)
		dest   = address of begin of dest
		edst   = address of end of dest move (dest + length - 1)

	Case 1: dest>esrc
		source:  ccccccccccc
		dest:                   cccccccccccc
		
	Case 2: edest<src
		source:                 cccccccccccc
		dest:    cccccccccccc

	Case 3: not case 1 or 2 and (dest<esrc && dest>source)
		source:  cccccccccccc
		dest:             ccccccccccccc

	Case 4: not 1, 2, or 3 and source<edst
		source:           ccccccccccccc
		dest:    cccccccccccc
		(This case probably does not require special handling, but
		 we will take no chances)
*/

	register char *esrc, *edst ;
	int l ;

	if (source==dest) return(1) ;

	while (length) {

		esrc = source + ((int) (length - 1)) ;
		edst = dest   + ((int) (length - 1)) ;

		if (dest>esrc || edst<source) { /* cases 1 and 2        */
			memcpy(dest,source,length) ;
			return(1) ;
		}
		if ((dest> source) && (dest<=esrc)) { /* case 3         */
			l = edst - esrc ;
			memcpy(esrc+1,esrc-l+1,l) ;
			length -= l ;

		}
		else {                          /* case 4               */
			l = source - dest ;
			memcpy(dest,source,l) ;
			dest += l ;
			source += l ;
			length -= l ;
		}
	}
	return(1) ;
}

/* end of tsub.c */
