#define	MSRC_Revision		8


/*

	pe.c
					Copyright (C) 1996, Marty Ross

*/

/*
	#includes
*/

#include	"compiler.h"
#include	<string.h>
#include	<ctype.h>
#include	"clgetopt.h"
#include	"pe.h"
#include	"abts.h"
#include	"txtexp.h"
#include	"subexpr.h"
#include	"evaluate.h"


/*
	internal function prototypes
*/

static char *ltrim(char *pszText) ;


/*
	API entry points
*/

PEPARSE_RC pe_parseExpression(
	char *pszExpression,
	struct pe_ParseArgs *ppexp
) {
	int toklen ;
	subexpr_t exp ;
	TXTEXP txtexp ;
	ABTS abts ;
	PEPARSE_RC perc ;
	char token[ABTSMAX_TOKENLENGTH+1] ;

   	/*
		Parse expression into tokens
	*/
	txtexp_ctor(&txtexp) ;
	abts_ctor(&abts) ;
	abts_setbuf(
		&abts,
		pszExpression,
		strlen(pszExpression)
	) ;
	perc= PEPARSERC_SUCCESS ;
	while(abts_gettok(&abts, token, &toklen) == 0) {
		if (!*ltrim(token)) continue ;
		if (txtexp_addtoken(&txtexp, token, toklen)) {
			clerror("too many tokens") ;
			perc= PEPARSERC_FAILURE ;
			break ;
		}
	}
	abts_dtor(&abts) ;
	if (perc != PEPARSERC_SUCCESS) return(perc) ;

	/*
		Compile the expression
	*/

	subexpr_setexstream(ppexp->pExprProg, ppexp->iExprProgSize) ;
	subexpr_setfoundvar(ppexp->fnFindVar) ; /* AFTER ..setexstream!! */
	exp.tok= txtexp.tokens ;
	exp.len= txtexp.ntokens ;
	if (subexpr(exp)) {
		clerror("expression compilation error") ;
		return(PEPARSERC_FAILURE) ;
	}

	ppexp->iExprProgLen= subexpr_exstreamlen() ;
	return(PEPARSERC_SUCCESS) ;
}	

PEEVAL_RC pe_evalExpression(
	struct pe_ParseArgs *ppexp,
	struct pe_EvalArgs *ppeval,
	long *plResult
) {
	long lval ;

	/*
		execute the expression
	*/
	evaluate_setlongstack(ppeval->pExprStack, ppeval->iExprStackSize) ;
	evaluate_setpfngetusrvarlong(ppeval->fnGetVarLong) ;
	evaluate_setpfnsetusrvarlong(ppeval->fnSetVarLong) ;
	if (evaluate_exstream_long(
		ppexp->pExprProg,
		ppexp->iExprProgLen,
		&lval
	)) {
		clerror("expression execution error") ;
		return(PEEVALRC_FAILURE) ;
	}

	(*plResult)= lval ;
	return(PEEVALRC_SUCCESS) ;
}


/*
	internal functions
*/

static char *ltrim(char *pszText)
{
	char *p ;

	for (p= pszText; (*p) && isspace(*p); p++) ;
	return(p) ;
}


/* end of pe.c */
