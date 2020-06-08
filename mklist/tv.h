/*	Revision:		3
*/
/*

	tv.h

*/


/*
	#defines
*/

#define	TV_RC	       		int
#define	TVRC_SUCCESS   		0
#define	TVRC_OVERFLOW  		1
#define	TVRC_UNMATCHED 		2


/*
	types, structures
*/

typedef char *(*tv_pfnLookup_t)(char *pszWord, int iLenWord, void *pSpecial) ;

struct tv_parms {
	tv_pfnLookup_t pfnLookup ;
} ;
	

/*
	API functions
*/

TV_RC tv_Init(struct tv_parms *) ;
void tv_Fini() ;
 
TV_RC tv_Substitute(
	char *pszSource,
	char *pszTarget,
	int iMaxTargetLength,
	void *pSpecial
) ;

/* tv.h */
