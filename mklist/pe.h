/*	Revision:		2
*/

#ifndef PE_H_INC
#define	PE_H_INC

/*

	pe.h

*/


/*
	#definitions
*/

#define	PEPARSE_RC			int
#define	PEPARSERC_SUCCESS      		0
#define	PEPARSERC_FAILURE      		1
			 
#define	PEEVAL_RC			int
#define	PEEVALRC_SUCCESS		0
#define	PEEVALRC_FAILURE		1


/*
	structs, types
*/

typedef int (*pe_fnFindParseVar)(char *vname, int *pvid) ;

typedef int (*pe_fnGetEvalVarLong)(int vid, long *plval) ;
typedef int (*pe_fnSetEvalVarLong)(int vid, long lval) ;

struct pe_ParseArgs {
	unsigned char *pExprProg ;
	int iExprProgSize ;
	int iExprProgLen ;
	pe_fnFindParseVar fnFindVar ;
} ;

struct pe_EvalArgs {
	long *pExprStack ;
	int iExprStackSize ;
	pe_fnGetEvalVarLong fnGetVarLong ;
	pe_fnSetEvalVarLong fnSetVarLong ;
} ;


/*
	API entry points
*/

PEPARSE_RC pe_parseExpression(
	char *pszExpression,
	struct pe_ParseArgs *ppexp
) ;

PEEVAL_RC pe_evalExpression(
	struct pe_ParseArgs *ppexp,
	struct pe_EvalArgs *ppeval,
	long *plResult
) ;

/* #ifndef PE_H_INC */
#endif

/* end of pe.h */
