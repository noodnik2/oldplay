#define	Revision		14

/*
	lclprtf.c		Local "printf" function(s)
				Copyright (C) 1995-1996, Pointofsale Ltd.

*/


/*
	#includes
*/

#include	"compiler.h"
#include	<stdarg.h>

#include	"lclprtf.h"


/*
	internal macros
*/
#define	LCLFP	(lp.fp? lp.fp: stdout)


/*
	internal static data
*/

static struct lclprtf_parms lp ;


/*
	internal function forwards
*/

static void flushem() ;


/*
	entry points
*/


/*
	void lclprtf_init()
*/
void lclprtf_init()
{
	memset(&lp, '\0', sizeof(struct lclprtf_parms)) ;
	lp.t0= time(0) ;
}


/*
	char *lclprtf_setpfx(char *pszPFX)

	NOTE:	the "pszPFX" assigned here is only a pointer; the
		item pointed to by "pszPFX" must remain valid for
		the duration of operation...
*/
char *lclprtf_setpfx(char *pszPFX)
{
	char *pszOldPFX ;

	pszOldPFX= lp.pszPFX ;
	lp.pszPFX= pszPFX ;
	return(pszOldPFX) ;
}

/*
	void lclprtf_usefile(char *pszFileName)
*/
void lclprtf_usefile(char *pszFileName)
{
	char *pszMode ;

	if (lp.isMyFile) {
		fclose(lp.fp) ;
		lp.fp= 0 ;
		lp.isMyFile= 0 ;
	}

	if (!pszFileName) return ;

	if (pszFileName == (char *) 1) return ;

	if (pszFileName == (char *) 2) {
		lp.fp= stderr ;
		return ;
	}

	if (pszFileName[0] == '\0') return ;

	pszMode= (lp.opts & LCLPRTFOPTS_APPEND)? "a": "w" ;
	lp.fp= fopen(pszFileName, pszMode) ;

	if (!lp.fp) {
		lclprtf("lclprtf: can't open: %s\n", pszFileName) ;
		return ;
	}
	lp.isMyFile= 1 ;
}


/*
	void lclprtf_setopts(LCLPRTF_OPTS opts)
*/
void lclprtf_setopts(LCLPRTF_OPTS opts)
{
	lp.opts= opts ;
}

/*

	void lclprtf(char *fmt, ...)

	NOTE:	this function is limited to printing lines that are
		shorter than LCLPRTF_MAXLINE_LEN bytes each!!!

*/
void lclprtf(char *fmt, ...)
{
	static char outbuf[LCLPRTF_MAXLINE_LEN+1] ;
	static int col= 0 ;

	int i, c ;
	FILE *fp ;
	va_list args ;

	va_start(args, fmt) ;
	vsprintf(outbuf, fmt, args) ;

	fp= LCLFP ;
	for (i= 0; outbuf[i] != '\0'; i++) {
		if (lp.pszPFX && (col == 0)) {
			if (!(lp.opts & LCLPRTFOPTS_NOPFX)) {
				fprintf(fp, "%s: ", lp.pszPFX) ;
			}
			if (lp.opts & LCLPRTFOPTS_TSTAMP) {
				fprintf(
					fp,
					"%ld: ", 
					(long) ((long) time(0) - (long) lp.t0)
				) ;
			}
		}
		c= outbuf[i] ;
		putc(c, fp) ;
		if (c == '\r') col= 0 ;
		else if (c == '\n') col= 0 ;	/* lf ==> cr/lf	*/
		else col++ ;
	}

	if (lp.opts & LCLPRTFOPTS_FLUSH) {
		flushem() ;
	}
}

/*
	void lclprtf_fini()
*/
void lclprtf_fini()
{
	lclprtf_usefile(0) ;
}


/*
	start of internal functions
*/

static void flushem()
{
	if (lp.fp) {
		fflush(LCLFP) ;
	}
}

/* end of lclprtf.c */
