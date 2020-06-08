/*	Revision:	24
*/

/*

	abts.h				C Header: Ascii Buffer Token Stream
					Copyright (C) 1994, Contahal Ltd.

*/

#ifndef	ABTS_H_INC
#define	ABTS_H_INC

/*
	sub-#includes
*/
#include	"ats.h"

/*
	#defines
*/

#define	ABTSMAX_TOKENLENGTH	ATSMAX_TOKENLENGTH

#define	ABTSE_SUCCESS  		0		/* everything is ok	*/
#define	ABTSE_NOTEXT	 	1		/* no user text set	*/
#define	ABTSE_INTERNAL		2      		/* internal error	*/

#define	ABTSE_EOS      		ATSE_EOS
#define	ABTSE_EOSINCOMMENT	ATSE_EOSINCOMMENT
#define	ABTSE_EOSINLITERAL	ATSE_EOSINLITERAL
#define	ABTSE_NOMEMORY		ATSE_NOMEMORY
#define	ABTSE_BADPOS		ATSE_BADPOS
#define	ABTSE_OVERFLOW		ATSE_OVERFLOW
#define	ABTSE_TOOLONG		ATSE_TOOLONG

/*
	typedefs
*/

typedef ATSE ABTSE ;
typedef ATSPOS ABTSPOS ;
typedef ATSCMTPROC ABTSCMTPROC ;

typedef struct tagABTS {
	char *puserbuf ;
	int userbuf_len ;
	int userbuf_ptr ;
	ABTSCMTPROC pfcmtproc ;
	ATSSTATE atsstate ;
} ABTS ;

/*
	function decls
*/
ABTSE abts_errno(void) ;
int abts_init(void) ;
int abts_fini(void) ;

int abts_setdebug(int level) ;
ABTS *abts_new(void) ;
int abts_delete(ABTS *pabts) ;
int abts_ctor(ABTS *pabts) ;
int abts_dtor(ABTS *pabts) ;
int abts_setbuf(ABTS *pabts, char *text, int len) ;
char *abts_getbuf(ABTS *pabts) ;
int abts_setcmtproc(ABTS *pabts, ABTSCMTPROC pfcmtproc) ;
int abts_gettok(ABTS *pabts, char *token, int *plen) ;
int abts_getatspos(ABTS *pabts, ATSPOS *pabtspos) ;
int abts_setatspos(ABTS *pabts, ATSPOS abtspos) ;
int abts_getatsstate(ABTS *pabts, ATSSTATE *patsstate) ;
int abts_setatsstate(ABTS *pabts, ATSSTATE atsstate) ;
int abts_flush(ABTS *pabts) ;

/* #ifndef ABTS_H_INC */
#endif

/* end of abts.h */
