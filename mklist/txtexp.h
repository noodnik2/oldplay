/*	Revision:	2
*/

/*

	txtexp.h				TXTEXP C Header
					Copyright (C) 1994, Contahal Ltd.

*/

#ifndef	TXTEXP_H_INC
#define	TXTEXP_H_INC

/*
	#includes
*/

/*
	#defines
*/
#define	TXTEXPMAX_BUFSIZE	256
#define	TXTEXPMAX_TOKENS	128

#define	TXTEXPE_SUCCESS		0
#define	TXTEXPE_TOOMANYTOKENS	1
#define	TXTEXPE_OVERFLOW	2

/*
	typedefs
*/
typedef int TXTEXPE ;
typedef struct tagTXTEXP {
	char buf[TXTEXPMAX_BUFSIZE] ;
	char *tokens[TXTEXPMAX_TOKENS] ;
	int ntokens ;
	int bufptr ;
} TXTEXP ;

/*
	function decls
*/
TXTEXPE txtexp_errno(void) ;
int txtexp_init(void) ;
int txtexp_fini(void) ;

int txtexp_ctor(TXTEXP *ptxtexp) ;
int txtexp_dtor(TXTEXP *ptxtexp) ;
int txtexp_addtoken(TXTEXP *ptxtexp, char *token, int len) ;

/* #ifndef TXTEXP_H_INC */
#endif

/* end of txtexp.h */
