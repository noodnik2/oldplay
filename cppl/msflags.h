/*	Revision:		6
*/

#ifndef	M_SFLAGS_H_INC
#define	M_SFLAGS_H_INC

/*

	msflags.h			qad-NewARC21 M_SFLAGS C/C++ Header
					Copyright (C) 1995, Contahal Ltd.

*/

/*
	sub-#includes
*/

#include	"musrdef.h"

/*
	typedefs
*/

typedef struct tagM_SFLAGSMAP {
	char *pszName ;
	M_SFLAGS sfValue ;
} M_SFLAGSMAP ;

/*
	function templates
*/
M_SBOOL M_SFlags_Get(const M_SFLAGSMAP *, M_SFLAGS *, const char *) ;

/* #ifndef M_SFLAGS_H_INC */
#endif

/* end of msflags.h */
