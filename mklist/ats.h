/*	Revision:	21
*/

/*

	ats.h				C Header: Ascii Token Streams
					Copyright (C) 1994, Contahal Ltd.

*/

#ifndef	ATS_H_INC
#define	ATS_H_INC

/*
	sub-#includes
*/

/*
	#defines
*/

#define	ATSMAX_TOKENLENGTH	64

#define	ATSST_ABTS		0	/* Ascii Buffer Token Stream	*/
#define	ATSST_AFTS		1	/* Ascii File Token Stream	*/
#define	ATSST_SATS		2	/* Stream of Asci Token Streams	*/

#define	ATSE_SUCCESS  		0	/* everything is ok    		*/
#define	ATSE_NEWS     		0xA01	/* new stream started  		*/
#define	ATSE_EOS      		0xA02	/* normal end of stream		*/
#define	ATSE_EOSINCOMMENT	0xA03	/* EOS was in comment  		*/
#define	ATSE_EOSINLITERAL	0xA04	/* EOS was in literal  		*/
#define	ATSE_EARLYEOS 		0xA05	/* premature EOS       		*/
#define	ATSE_NOMEMORY		0xA06	/* can't allocate mem  		*/
#define	ATSE_BADPOS		0xA07	/* bad position given  		*/
#define	ATSE_BADPARMS		0xA08	/* bad position given  		*/
#define	ATSE_OVERFLOW		0xA09	/* text ovflo; not read		*/
#define	ATSE_TOOLONG		0xA0A	/* token toolong; truncated	*/
#define	ATSE_IOERROR		0xA0B	/* device I/O error    		*/
#define	ATSE_INTERNAL		0xA0C	/* internal error      		*/

/*
	typedefs
*/
typedef int ATSE ;
typedef int (*ATSCMTPROC)(int ch) ;
typedef int (*ATSLINEPROC)(char *pline, int len) ;
typedef void AXTS ;			/* abstract stream object type	*/

typedef struct tagATSPOS {
	int sid ;			/* id of stream			*/
	long pos ;			/* position of stream (offset)	*/
} ATSPOS ;

typedef struct tagATSTPA {
	char *szbc ;			/* break character string	*/
	struct tagATSTPA *pnexttpa ;	/* follow if more chars in tok	*/
} ATSTPA ;

typedef struct tagATSSTATE {
	int inesc ;			/* currently in "escaped char"	*/
	int incmt ;			/* currently inside comment	*/
	int inlit ;			/* currently inside literal	*/
	int qch ;			/* quote character if 'inlit'	*/
	ATSTPA *ptpa ;			/* token parser array		*/
} ATSSTATE ;

typedef struct tagATS {
	int st ;			/* stream type			*/
	AXTS *paxts ;			/* abstract stream object ptr	*/
} ATS ;

/*
	function decls
*/
int ats_ctor(ATS *pats) ;
int ats_dtor(ATS *pats) ;
int ats_errno(void) ;
int ats_set(ATS *pats, int st, AXTS *paxts) ;
int ats_gettok(ATS *pats, char *token, int *plen) ;
int ats_getpos(ATS *pats, ATSPOS *patspos) ;
int ats_setpos(ATS *pats, ATSPOS atspos) ;
int ats_getstate(ATS *pats, ATSSTATE *pstate) ;
int ats_free(ATS *pats) ;
AXTS *ats_getafts(ATS *pats) ;

/* #ifndef ATS_H_INC */
#endif

/* end of ats.h */
