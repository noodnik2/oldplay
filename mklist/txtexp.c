/*	Revision:	3
*/

/*

	txtexp.c			C Source: Text Expressions
					Copyright (C) 1994, Contahal Ltd.

*/

#include	<malloc.h>
#include	<memory.h>

#include	"txtexp.h"

/*
	local #defines
*/

/*
	local typedefs
*/

/*
	internal data
*/
static TXTEXPE lclerrno= TXTEXPE_SUCCESS ;

/*
	public data
*/

/*
	internal function decls
*/
static int txtexp0_ctor(TXTEXP *ptxtexp) ;
static int txtexp0_dtor(TXTEXP *ptxtexp) ;

/*
	entry points
*/
int txtexp_init() { return(0) ; }
int txtexp_fini() { return(0) ; }
TXTEXPE txtexp_errno() { return(lclerrno) ; }

int txtexp_ctor(TXTEXP *ptxtexp)
{
	return(txtexp0_ctor(ptxtexp)) ;
}

int txtexp_dtor(TXTEXP *ptxtexp)
{
	return(txtexp0_dtor(ptxtexp)) ;
}

int txtexp_addtoken(TXTEXP *ptxtexp, char *token, int len)
{
	int ntokens, bufptr ;
	char *tbuf ;

	ntokens= ptxtexp->ntokens ;
	if (ntokens>= TXTEXPMAX_TOKENS) {
		lclerrno= TXTEXPE_TOOMANYTOKENS ;
		return(-1) ;
	}

	bufptr= ptxtexp->bufptr ;
	if ((bufptr + len)>= TXTEXPMAX_BUFSIZE) {
		lclerrno= TXTEXPE_OVERFLOW ;
		return(-1) ;
	}

	tbuf= ptxtexp->buf + bufptr ;
	memcpy(tbuf, token, len+1) ;
	ptxtexp->tokens[ntokens]= tbuf ;
	ptxtexp->bufptr= bufptr + len + 1 ;
	ptxtexp->ntokens++ ;
	return(0) ;
}

/*
	internal functions
*/

static int txtexp0_ctor(TXTEXP *ptxtexp)
{
	memset((void *) ptxtexp, '\0', sizeof(TXTEXP)) ;
	return(0) ;
}

static int txtexp0_dtor(TXTEXP *ptxtexp)
{
	return(0) ;
}

/* end of txtexp.c */
