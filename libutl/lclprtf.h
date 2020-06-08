/*	Revision:		12
*/
  
#ifndef	LCLPRTF_H_INC
#define	LCLPRTF_H_INC

/*

	lclprtf.h			Local "printf" functions
					Copyriht (C) 1995-1996, Pointofsale Ltd.

*/


/*
	sub-#includes
*/
#include	"compiler.h"
#include	<stdio.h>
#include	<time.h>


/*
	#defines
*/

#define	LCLPRTF_MAXLINE_LEN		8192

#define	LCLPRTF_OPTS			unsigned
#define	LCLPRTFOPTS_NONE		0x0000
#define	LCLPRTFOPTS_FLUSH		0x0001
#define	LCLPRTFOPTS_APPEND		0x0002
#define	LCLPRTFOPTS_TSTAMP		0x0004
#define	LCLPRTFOPTS_NOPFX		0x0008

#define	LCLPRTF_BOOL			int
#define	LCLPRTFBOOL_FALSE		0
#define	LCLPRTFBOOL_TRUE		(!LCLPRTFBOOL_FALSE)

/*
	structs, types
*/

struct lclprtf_parms {
	char *pszPFX ;
	FILE *fp ;
	LCLPRTF_OPTS opts ;
	int isMyFile ;
	time_t t0 ;
} ;


/*
	API templates
*/

/*
	basic API
*/
void lclprtf_init() ;
void lclprtf_fini() ;

void lclprtf_usefile(char *pszFileName) ;
char *lclprtf_setpfx(char *pfx) ;
void lclprtf_setopts(LCLPRTF_OPTS) ;
void lclprtf(char *fmt, ...) ;


/*
	extended API
*/
LCLPRTF_BOOL lclprtf_parseopts(char *pszOptName, LCLPRTF_OPTS *pOpt) ;

/* #ifndef LCLPRTF_H_INC */
#endif

/* end of lclprtf.h */
