#define	Revision		4

/*
	lclprtf2.c		Local "printf" function(s) - extra fns
				Copyright (C) 1995-1996, Pointofsale Ltd.

*/


/*
	#includes
*/

#include	"compiler.h"
#include	<stdarg.h>
#include	<string.h>
#include	<memory.h>

#include	"lclprtf.h"


/*
	internal function templates
*/
static LCLPRTF_BOOL parseoptName(
	char *psOptName,
	int len,
	LCLPRTF_OPTS *pOpt
) ;

/*
	internal structs
*/

struct optVals {
	char *pszOptName ;
	LCLPRTF_OPTS lpOpt ;
} ;
	

/*
	static data
*/

static struct optVals ov[]= {

	{	"a",		LCLPRTFOPTS_APPEND	},
	{	"app",		LCLPRTFOPTS_APPEND	},
	{	"append",	LCLPRTFOPTS_APPEND	},

	{	"f",		LCLPRTFOPTS_FLUSH	},
	{	"fl",		LCLPRTFOPTS_FLUSH	},
	{	"flush",	LCLPRTFOPTS_FLUSH	},

	{	"nop",		LCLPRTFOPTS_NOPFX	},
	{	"nopfx",	LCLPRTFOPTS_NOPFX	},
	{	"noprefix",	LCLPRTFOPTS_NOPFX	},

	{	"t",		LCLPRTFOPTS_TSTAMP	},
	{	"ti",		LCLPRTFOPTS_TSTAMP	},
	{	"time",		LCLPRTFOPTS_TSTAMP	},
	{	"ts",		LCLPRTFOPTS_TSTAMP	},
	{	"tstamp",	LCLPRTFOPTS_TSTAMP	},
	{	"timestamp",	LCLPRTFOPTS_TSTAMP	},

} ;
#define	N_OV	(sizeof(ov) / sizeof(ov[0])) 

/*
	entry points
*/


/*
	LCLPRTF_BOOL lclprtf_parseopt(char *pszOpt, LCLPRTF_OPTS *pOpt)
*/
LCLPRTF_BOOL lclprtf_parseopts(char *pszOptName, LCLPRTF_OPTS *pOpt)
{
	int i ;			/* offset to current option	*/
	int j ;			/* offset to end of current opt	*/
	int k ;			/* offset to next option	*/
	char *p ;
	LCLPRTF_OPTS lpOptAccum ;
	LCLPRTF_OPTS lpOpt ;

	lpOptAccum= LCLPRTFOPTS_NONE ;
	for (i= 0; pszOptName[i] != '\0'; i= k) {
		p= strchr(pszOptName + i, ',') ;
		if (p) {
			j= (int) (p - pszOptName) ;
			k= j + 1 ;
		}
		else {
			k= j= i + strlen(pszOptName + i) ;
		}
		if (!parseoptName(pszOptName + i, j - i, &lpOpt)) {
			return(LCLPRTFBOOL_FALSE) ;
		}
		lpOptAccum|= lpOpt ;
	}
	(*pOpt)= lpOptAccum ; 
	return(LCLPRTFBOOL_TRUE) ;
}


/*
	internal functions
*/

static LCLPRTF_BOOL parseoptName(
	char *psOptName,
	int len,
	LCLPRTF_OPTS *pOpt
) {
	int i ; 

	for (i= 0; i< N_OV; i++) {
		if (strlen(ov[i].pszOptName) != len) continue ;
		if (memcmp(psOptName, ov[i].pszOptName, len) == 0) {
			(*pOpt)= ov[i].lpOpt ;	
			return(LCLPRTFBOOL_TRUE) ;
		}
	}
	
	return(LCLPRTFBOOL_FALSE) ;
}

/* end of lclprtf.c */
