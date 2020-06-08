#define	MSRC_Revision			1

#include	"compiler.h"

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	"tv.h"


/*
	internal #defines
*/

#define	TVMAX_ENVNAMELEN	255		/* allocated on STACK!!! */

#define	TV_BOOL	       		int
#define	TVBOOL_FALSE   		0
#define	TVBOOL_TRUE   		(!TVBOOL_FALSE)


/*
	forward internal functions
*/
				  
static TV_BOOL appendTarget(
	char *pszSource,
	char *pszTargetFence,
	char **ppszTargetCurrent,
	int iLen
) ;


/*
	internal global data
*/

static struct tv_parms tvp ;


/*
	entry points
*/

TV_RC tv_Init(struct tv_parms *ptvp)
{
	memset(&tvp, '\0', sizeof(struct tv_parms)) ;
	if (ptvp) {
		memcpy(&tvp, ptvp, sizeof(struct tv_parms)) ;
	}
}

void tv_Fini()
{
}

TV_RC tv_Substitute(
	char *pszSource,
	char *pszTarget,
	int iMaxTargetLength,
	void *pSpecial
) {
	char *psz0, *psz1 ;		/* ptrs to % in source	*/
	char *pszSourceCurrent ;
	char *pszTargetCurrent ;
	char *pszTargetFence ;
	char *pszEnv ;
	int iLen ;

	pszTargetFence= pszTarget + iMaxTargetLength ;
	pszTargetCurrent= pszTarget ;
	pszSourceCurrent= pszSource ;

	for ( ; pszSourceCurrent[0] != '\0'; pszSourceCurrent= psz1 + 1) {

		/*
			get first delimiter, if none, we're done
		*/
		psz0= strchr(pszSourceCurrent, '%') ;
		if (!psz0) {
			if (!appendTarget(
				pszSourceCurrent,
				pszTargetFence,
				&pszTargetCurrent,
				strlen(pszSourceCurrent)
			)) {
				pszTargetCurrent[0]= '\0' ;
				return(TVRC_OVERFLOW) ;
			}
			break ;
		}

		/*
			get second delimiter; if none => error
		*/
		psz1= strchr(psz0 + 1, '%') ;
		if (!psz1) {
			pszTargetCurrent[0]= '\0' ;
			return(TVRC_UNMATCHED) ;
		}

		/*
			if stuff before first delimiter, append to target
		*/
		if (psz0> pszSourceCurrent) {
			if (!appendTarget(
				pszSourceCurrent,
				pszTargetFence,
				&pszTargetCurrent,
				psz0 - pszSourceCurrent
			)) {
				pszTargetCurrent[0]= '\0' ;
				return(TVRC_OVERFLOW) ;
			}
		}

		/*
			get variable value,
			append to target iff value exists
		*/
		if (tvp.pfnLookup) {
			pszEnv= tvp.pfnLookup(
				psz0 + 1,
				(int) (psz1-psz0-1),
				pSpecial
			) ;
			if (pszEnv) {
				if (!appendTarget(
					pszEnv,
					pszTargetFence,
					&pszTargetCurrent,
					strlen(pszEnv)
				)) {
					pszTargetCurrent[0]= '\0' ;
					return(TVRC_OVERFLOW) ;
				}
			}
		}
	}

	pszTargetCurrent[0]= '\0' ;
	return(TVRC_SUCCESS) ;
}



/*
	static internal functions
*/

static TV_BOOL appendTarget(
	char *pszSource,
	char *pszTargetFence,
	char **ppszTargetCurrent,
	int iLen
) {
	if (((*ppszTargetCurrent) + iLen)>= pszTargetFence) {
		return(TVBOOL_FALSE) ;		/* failed */
	}
	memcpy((*ppszTargetCurrent), pszSource, iLen) ;
	(*ppszTargetCurrent)+= iLen ;
	return(TVBOOL_TRUE) ;
}	

/* end of tv.c */
