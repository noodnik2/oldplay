/*	Revision:		2
*/

#ifndef	INIS_H_INC
#define	INIS_H_INC

/*

	inis.h
					Copyright (C) 1996, Pointofsale Ltd.


*/


/*
	sub-#includes
*/
#include	"compiler.h"
#include	"istruc.h"


/*
	#defines
*/

#define	INIS_BOOL			int
#define	INISBOOL_FALSE			0
#define INISBOOL_TRUE			(!INISBOOL_FALSE)

#define	INISMAX_RECORDSIZE		1024

#define	INISPARSE_RC			int
#define	INISPARSERC_SUCCESS		0
#define	INISPARSERC_FAILURE		1
#define	INISPARSERC_NOFILE		2
#define	INISPARSERC_NOSTRUCTURE		3
#define	INISPARSERC_ISTRUCPARSERCBASE	100


/*
	structs, types
*/

struct inisSection {
	void *pStruc ;
	char *pszSectionName ;
	struct istrucField *paFields ;
	int nFields ;
} ;


/*
	API entry points
*/

INISPARSE_RC inis_parseSingle(
	char *pszFileName,
	char *pszSectionName,
	void *pStruc,
	struct istrucField *paFields,
	int nFields
) ;

INISPARSE_RC inis_parseMultiple(
	char *pszFileName,
	struct inisSection *paSections,
	int nSections
) ;

unsigned long inis_lineno() ;

/* #ifndef INIS_H_INC */
#endif

/* end of inis.h */
