#define	Revision      		4

/*

	msflags.c			qad-NewARC21 M_SFLAGS C Source
					Copyright (C) 1995, Contahal Ltd.

*/

/*
	#includes
*/

#include	"msflags.h"
#include	"mstrlst.h"

#include	<string.h>

/*
	entry points
*/

M_SBOOL M_SFlags_Get(const M_SFLAGSMAP *psfm, M_SFLAGS *psf, const char *psz)
{
	M_SFLAGS sfv= 0 ;
	M_SBOOL bOk= M_CTRUE ;
	M_StrLst osl(psz) ;

	for (int i= 0; i< osl.Length(); i++) {
		for (int j= 0; psfm[j].pszName; j++) {
			if (strcmp(psfm[j].pszName, osl[i]) == 0) {
				sfv|= psfm[j].sfValue ;
				break ;
			}
		}
		if (!psfm[j].pszName) bOk= M_CFALSE ;	/* not found!	*/
	}
	(*psf)= sfv ;
	return(bOk) ;
}

/* end of msflags.c */
