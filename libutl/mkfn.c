#define	MSRC_Revision		1

/*

	mkfn.c

*/

#include	"compiler.h"
#include	"mkfn.h"


/*
	internal #defines
*/

#if	CD_OPSYS == CD_OPSYS_MSDOS
#define	PATHSEP		'\\'
#endif

#if	CD_OPSYS == CD_OPSYS_UNIX
#define	PATHSEP		'/'
#endif


/*
	entry points
*/

void mkfn_ExtFileName(
	char *pszExtFileName,
	char *pszBaseName,
	char *pszExt
) {
	int i, l, c ;

	strcpy(pszExtFileName, pszBaseName) ;

	l= strlen(pszExtFileName) ;
	for (i= l - 1; i> 0; i--) {
		c= pszExtFileName[i] ;
		if (c == '.') return ;
		if (c == PATHSEP) break ;
	}
	pszExtFileName[l++]= '.' ;
	strcpy(pszExtFileName + l, pszExt) ;
}

/* end of mkfn.c */
