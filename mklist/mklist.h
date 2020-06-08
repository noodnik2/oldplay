/*	Revision:		15
*/

#ifndef	MKLIST_H_INC
#define	MKLIST_H_INC

/*

	mklist.h

*/


/*
	#definitions, macros
*/

						/* exit() codes		*/
#define	MKLIST_E			int
#define	MKLISTE_SUCCESS			0
#define	MKLISTE_BADARGS	       		1
#define	MKLISTE_BADEXPR       		2
#define	MKLISTE_OVERFLOW       		3
#define	MKLISTE_IOERROR       		4
#define	MKLISTE_CANTOUTPUT     		5
#define	MKLISTE_FAILURE       		99

#define	MKLIST_BOOL	       		int
#define	MKLISTBOOL_FALSE       		0
#define	MKLISTBOOL_TRUE	       		(!MKLISTBOOL_FALSE)

#define	MKLISTMAX_FILENAME_LEN		255
#define	MKLISTMAX_EXPRESSION_LEN	255
#define	MKLISTMAX_SUBSTITUTE_LEN	255
#define	MKLISTMAX_EXPRESSION_VARS	32

#define	MKLIST_EXPRPROGSIZE		255
#define	MKLIST_EXPRSTACKSIZE		32

#define	MKLISTBYTE0_7(x)		((x) & 255)
#define	MKLISTBYTE8_15(x)		MKLISTBYTE0_7((x)>>8)

/*
	structures, types
*/

struct mklist_parms {
	MKLIST_BOOL bHelp ;
	MKLIST_BOOL bQuiet ;
	MKLIST_BOOL bReverseSort ;
	MKLIST_BOOL bUserSeed ;
	char szDBFileName[MKLISTMAX_FILENAME_LEN+1] ;
	char szSortExpression[MKLISTMAX_EXPRESSION_LEN+1] ;
	char szExpression[MKLISTMAX_EXPRESSION_LEN+1] ;
	char szSubstitute[MKLISTMAX_SUBSTITUTE_LEN+1] ;
	int iDebugLevel ;
	long lRandSeed ;
} ;


/* #ifndef MKLIST_H_INC */
#endif

/* end of mklist.h */
