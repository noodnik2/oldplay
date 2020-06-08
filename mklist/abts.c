/*	Revision:	94
*/

#define		DEBUG_LEVEL		0

/*

	abts.c				Ascii Buffer Token Stream
					Copyright (C) 1994, Contahal Ltd.

*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<malloc.h>
#include	<memory.h>

#include	"abts.h"

/*
	local #defines
*/
#define	INESC		(pabts->atsstate.inesc)
#define	QCH		(pabts->atsstate.qch)
#define	INLIT		(pabts->atsstate.inlit)
#define	INCMT		(pabts->atsstate.incmt)
#define	INTPA		(pabts->atsstate.ptpa)

#define	ISQUOTECH(c) (((c) == '"') || ((c) == '\''))
#define	ISSPACE(c) ((c)<= 32)
#define	ISBRKCH(c) !(			\
     (((c)>= 'a') && ((c)<= 'z'))	\
  || (((c)>= 'A') && ((c)<= 'Z'))	\
  || (((c)>= '0') && ((c)<= '9'))	\
  || ((c) == '_')			\
)

/*
	local typedefs
*/

/*
	internal data
*/
static ABTSE lclerrno= ABTSE_SUCCESS ;
static int lcldbglvl= DEBUG_LEVEL ;

static ATSTPA abtstpaltgtarg[]= {
	{	"=",	0		},		/* <<=, >>=	*/
	{	0,	0		},		/* >=, <=	*/
} ;

static ATSTPA abtstpaarg[]= {
	{	"<=",	abtstpaltgtarg     	},	/*  <<, <=	*/
	{	">=",	abtstpaltgtarg     	},	/*  >>, >=	*/
	{	"=",	0     			},	/*  == 		*/
	{	"=",	0	     		},	/*  != 		*/
	{	"=",	0      			},	/*  ^= 		*/
	{	"=",	0	    		},	/*  ~= 		*/
	{	"=&",	0			},	/*  &=, &&	*/
	{	"=|",	0      			},	/*  |=, ||	*/
	{	"=-",	0      			},	/*  -=, --	*/
	{	"=+",	0      			},	/*  +=, ++	*/
	{	"=",	0      			},	/*  *=		*/
	{	"=",	0      			},	/*  /= 		*/
	{	"=",	0      			},	/*  %= 		*/
} ;

ATSTPA abtstpa= {	"<>=!^~&|-+*/%",	abtstpaarg	} 	;

/*
	public data
*/

/*
	internal function decls
*/
static int abts0_ctor(ABTS *pabts) ;
static int abts0_getch(ABTS *pabts) ;
static int abts0_ungetch(ABTS *pabts) ;
static int abts0_iscmt(ABTS *pabts, int ch) ;
static int abts0_outcmt(ABTS *pabts, int cc) ;
static int abts0_uncmt(ABTS *pabts, int ct) ;
static int abts0_getcmt(ABTS *pabts, int ct) ;
static int abts0_getcmttail(ABTS *pabts, int ct) ;
static int abts0_getlittail(ABTS *pabts, char *token, int *plen) ;
static int abts0_gettok(ABTS *pabts, char *token, int *plen) ;
static int abts0_getctoktail(ABTS *pabts, char *token, int *plen) ;

/*
	entry points
*/
int abts_init() { return(0) ; }
int abts_fini() { return(0) ; }
ABTSE abts_errno() { return(lclerrno) ; }

int abts_setdebug(int level)
{
	lcldbglvl= level ;
	return(0) ;
}

ABTS *abts_new()
{
	ABTS *pabts ;
	pabts= (ABTS *) malloc(sizeof(ABTS)) ;
	if (pabts == (ABTS *) NULL) {
		lclerrno= ABTSE_NOMEMORY ;
		return((ABTS *) NULL) ;
	}
	abts0_ctor(pabts) ;
	if (lcldbglvl) printf("abts open: pabts=0x%lX\n", (long)pabts) ;
	return(pabts) ;
}

int abts_delete(ABTS *pabts)
{
	if (lcldbglvl) printf("abts close: pabts=0x%lX\n", (long)pabts) ;
	free((void *) pabts) ;
	return(0) ;
}

int abts_ctor(ABTS *pabts)
{
	return(abts0_ctor(pabts)) ;
}

int abts_dtor(ABTS *pabts)
{
	return(0) ;
}

int abts_setcmtproc(ABTS *pabts, ABTSCMTPROC pfcmtproc)
{
	pabts->pfcmtproc= pfcmtproc ;
	return(0) ;
}

int abts_getatsstate(ABTS *pabts, ATSSTATE *patsstate)
{
	memcpy(
		(void *) patsstate,
		(void *) &pabts->atsstate,
		sizeof(ATSSTATE)
	) ;
	return(0) ;
}

int abts_setatsstate(ABTS *pabts, ATSSTATE atsstate)
{
	memcpy(
		(void *) &pabts->atsstate,
		(void *) &atsstate,
		sizeof(ATSSTATE)
	) ;
	return(0) ;
}

int abts_setbuf(ABTS *pabts, char *puserbuf, int len)
{
	if (lcldbglvl) {
		printf("abts setbuf: pabts=0x%lX len=%d\n", (long)pabts, len) ;
	}
	pabts->puserbuf= puserbuf ;
	pabts->userbuf_len= len ;
	pabts->userbuf_ptr= 0 ;
	return(0) ;
}

char *abts_getbuf(ABTS *pabts)
{
	return(pabts->puserbuf) ;
}

int abts_getatspos(ABTS *pabts, ATSPOS *patspos)
{
	if (pabts->puserbuf == (char *) NULL) {
		lclerrno= ABTSE_NOTEXT ;
		return(-1) ;
	}
	patspos->sid= -1 ;
	patspos->pos= (long) pabts->userbuf_ptr ;
	return(0) ;
}

int abts_setatspos(ABTS *pabts, ATSPOS atspos)
{
	int newptr ;

	if (pabts->puserbuf == (char *) NULL) {
		lclerrno= ABTSE_NOTEXT ;
		return(-1) ;
	}
	newptr= (int) atspos.pos ;
	if ((newptr> (pabts->userbuf_len)) || (newptr< 0)) {
		lclerrno= ABTSE_BADPOS ;
		return(-1) ;
	}
	pabts->userbuf_ptr= newptr ;
	return(0) ;
}

/*
	rc= abts_gettok(ABTS *pabts, char *token, int *plen) ;

	Gets the next token from the input text.
	Must be made to skip comments (routing them
	to a routine that knows how to deal with them,
	such as copying them to the output file, etc..)

*/
int abts_gettok(ABTS *pabts, char *token, int *plen)
{
	int rc ;

	if (lcldbglvl) printf(
		"abts gettok: in (INLIT=%d INCMT=%d QCH=%d)\n",
		INLIT, INCMT, QCH
	) ;

	(*plen)= 0 ;
	token[0]= '\0' ;
	if (INLIT) {
		if (lcldbglvl) printf("abts gettok: 0getlittail\n") ;
		rc= abts0_getlittail(pabts, token, plen) ;
	}
	else {
		rc= 0 ;
		if (INCMT) {
			if (lcldbglvl) printf("abts gettok: 0getcmttail\n") ;
			rc= abts0_getcmttail(pabts, INCMT) ;
		}
		if (rc == 0) {
			if (lcldbglvl) printf("abts gettok: 0gettok\n") ;
			rc= abts0_gettok(pabts, token, plen) ;
		}
	}

	if (rc == 0) {
		if ((*plen)> 0) {	/* we should have a token by now... */
			if (lcldbglvl)
			printf("abts gettok: token='%s'\n", token) ;
			if ((*plen)>= ABTSMAX_TOKENLENGTH) {
				rc= -1 ;
				lclerrno= ABTSE_TOOLONG ;
				if (lcldbglvl)
				printf("abts gettok: TOO LONG!\n") ;
			}
		}
		else {			/* no token!  error or EOS	*/
			lclerrno= ABTSE_EOS ;
			rc= -1 ;
			if (lcldbglvl) printf("abts gettok: EOS(2)\n") ;
		}
	}	

	if (lcldbglvl) printf(
		"abts gettok: out (INLIT=%d INCMT=%d QCH=%d rc=%d errno=0x%04X, len=%d, tok='%s')\n",
		INLIT, INCMT, QCH, rc, lclerrno, (*plen), token
	) ;

	return(rc) ;
}

int abts_flush(ABTS *pabts)
{
	return(abts0_ctor(pabts)) ;
}

/*
	internal functions
*/

int abts0_gettok(ABTS *pabts, char *token, int *plen)
{
	int ch, nextcmt ;
	char *p ;

					/* loop across input */
	while((ch= abts0_getch(pabts))>= 0) {
		/* if token is empty */
		if ((*plen)<= 0) {
			/* skip any initial comments */
			INCMT= abts0_iscmt(pabts, ch) ;
			if (lcldbglvl)
			printf("abts iscmt?: INCMT=%d\n", INCMT) ;
			if (INCMT) {
				if (abts0_getcmt(pabts, INCMT)) {
					return(-1) ;
				}
				continue ;
			}
			/* check for quoted literals */
			INLIT= ISQUOTECH(ch) ;
			if (INLIT) {
				INESC= 0 ;		   /* not in escape  */
				QCH= ch ;		   /* "quote char"   */
				token[(*plen)++]= (char) ch ; /* starts/ends */
				if (lcldbglvl)
				printf("abts gettok: quotechar='%c'\n", ch) ;
				if (abts0_getlittail(pabts, token, plen)) {
					return(-1) ;
				}
				break ;
		 	}
			if (ISBRKCH(ch)) {
				if ((*plen)< ABTSMAX_TOKENLENGTH) {
					token[(*plen)++]= (char) ch ;
				}
				if (!ISSPACE(ch)) {
					p= strchr(abtstpa.szbc, ch) ;
					if (p && abtstpa.pnexttpa) {
						INTPA= abtstpa.pnexttpa
						     + (p - abtstpa.szbc) ;
						if (abts0_getctoktail(
							pabts, token, plen
						)) {
							return(-1) ;
						}
					}
				}
				break ;
			}
		}
		else {			/* have token already */
			/* if find comment, end token immediately */
			nextcmt= abts0_iscmt(pabts, ch) ;
			if (nextcmt) {
				abts0_uncmt(pabts, nextcmt) ;
				break ;
			}
			if (ISBRKCH(ch)) {
				abts0_ungetch(pabts) ;	/* unget delim	*/
				break ;
			}
		}

		/* just a character: add to current token */
		if ((*plen)< ABTSMAX_TOKENLENGTH) {
			token[(*plen)++]= (char) ch ;
		}
	}

	token[(*plen)]= '\0' ;			/* make it ASCIZ */
	return(0) ;				/* return OK */
}

static int abts0_getctoktail(ABTS *pabts, char *token, int *plen)
{
	int ch ;
	char *p0, *p ;

	while(/* CONSTCOND */ 1) {			/* gather token */
		p0= INTPA->szbc ;
		if (!p0) break ;
		if ((ch= abts0_getch(pabts))< 0) break ;	/* EOS */
		p= strchr(p0, ch) ;
		if (!p) {
			abts0_ungetch(pabts) ;
			break ;
		}
		if ((*plen)< ABTSMAX_TOKENLENGTH) {
			token[(*plen)++]= (char) ch ;
		}
		INTPA= INTPA->pnexttpa ;
		if (!INTPA) break ;
		INTPA+= (p - p0) ;
	}
	token[(*plen)]= '\0' ;
	INTPA= 0 ;
	return(0) ;
}

static int abts0_getcmt(ABTS *pabts, int ct)
{
	abts0_outcmt(pabts, '/') ;
	abts0_outcmt(pabts, ct) ;
	return(abts0_getcmttail(pabts, ct)) ;
}

static int abts0_getlittail(ABTS *pabts, char *token, int *plen)
{
	int ch ;

	while(/* CONSTCOND */ 1) { /* gather literal */
		if ((ch= abts0_getch(pabts))< 0) {
			token[(*plen)]= '\0' ;
			if (lcldbglvl)
			printf("abts getlittail: EOS(1)\n") ;
			return(0) ;
		}
		if ((*plen)< ABTSMAX_TOKENLENGTH) {
			token[(*plen)++]= (char) ch ;
		}
		if ((ch == QCH) && !INESC) break ;
		INESC= (ch == '\\') ;
	}
	token[(*plen)]= '\0' ;
	INLIT= 0 ;
	return(0) ;
}

static int abts0_ctor(ABTS *pabts)
{
	memset((void *) pabts, '\0', sizeof(ABTS)) ;
	return(0) ;
}

static int abts0_uncmt(ABTS *pabts, int ct)
{
	/* all comment ids are two bytes, so don't need to look at 'ct' */
	if (abts0_ungetch(pabts)< 0) return(-1) ;
	if (abts0_ungetch(pabts)< 0) return(-1) ;
	return(0) ;
}

/*
	ct= abts0_iscmt(ABTS *pabts, int ch) ;

	ch is the "current" char in the input stream (already read).
	returns comment-type char iff this is the beginning of a comment
		(in which case reads past start of comment)
	returns 0 if this is not beginning of comment
		(in which case leaves input stream "as is")
*/
static int abts0_iscmt(ABTS *pabts, int ch)
{
	int ch2 ;

	if (ch != '/') return(0) ;			/* not comment */
	ch2= abts0_getch(pabts) ;
	if ((ch2 != '*') && (ch2 != '/')) {
		abts0_ungetch(pabts) ;			/* not comment */
		return(0) ;
	}
	return(ch2) ;				/* return comment-type */
}

static int abts0_getcmttail(ABTS *pabts, int ct)
{
	int ch ;

	if (ct == '*') {			/* old 'C style' comment */
		while(/*CONSTCOND*/1) {
			if ((ch= abts0_getch(pabts))< 0) {
				break ;
			}
			abts0_outcmt(pabts, ch) ;
			if (ch != '*') continue ;
			if ((ch= abts0_getch(pabts)) != '/') {
				abts0_ungetch(pabts) ;
				continue ;
			}
			abts0_outcmt(pabts, ch) ;
			INCMT= 0 ;
			break ;
		}
	}
	else if (ct == '/') {		/* new 'C++ style' comment */
		while(/*CONSTCOND*/1) {
			if ((ch= abts0_getch(pabts))< 0) {
				break ;
			}
			if (ch< ' ') {
				abts0_ungetch(pabts) ;
				INCMT= 0 ;
				break ;
			}
			abts0_outcmt(pabts, ch) ;
		}
	}
	else {
		lclerrno= ABTSE_INTERNAL ;	/* bad param to this rtn */
		return(-1) ;
	}
	return(0) ;
}

static int abts0_getch(ABTS *pabts)
{
	int ch ;

	if (pabts->puserbuf == (char *) NULL) {
		lclerrno= ABTSE_NOTEXT ;
		return(-1) ;
	}
	if (pabts->userbuf_ptr>= pabts->userbuf_len) {
		lclerrno= ABTSE_EOS ;
		return(-1) ;
	}
	ch= (int) ((unsigned char *) pabts->puserbuf)[pabts->userbuf_ptr++] ;
	return(ch) ;
}

static int abts0_ungetch(ABTS *pabts)
{
	if (pabts->userbuf_ptr<= 0) {
		lclerrno= ABTSE_INTERNAL ;	/* should never happen */
		return(-1) ;
	}
	pabts->userbuf_ptr-- ;
	return(0) ;
}

static int abts0_outcmt(ABTS *pabts, int cc)
{
	if (lcldbglvl) printf("abts0_outcmt: cc='%c'\n", cc) ;
	if (pabts->pfcmtproc) pabts->pfcmtproc(cc) ;
	return(0) ;
}

/* end of abts.c */
