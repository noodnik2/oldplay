/*	Revision:		4
*/

#ifndef	ISTRUC_H_INC
#define	ISTRUC_H_INC

/*

	istruc.h

*/


/*
	#defines
*/

#define	ISTRUC_BOOL			int
#define	ISTRUCBOOL_FALSE		0
#define	ISTRUCBOOL_TRUE			(!ISTRUCBOOL_FALSE)

#define	ISTRUCPARSE_RC			int
#define	ISTRUCPARSERC_SUCCESS		0
#define	ISTRUCPARSERC_FAILURE		1
#define	ISTRUCPARSERC_NOEQUAL		2
#define	ISTRUCPARSERC_NONAME		3
#define	ISTRUCPARSERC_NOTFOUND		4
#define	ISTRUCPARSERC_BADTYPE		5
#define	ISTRUCPARSERC_BADVALUE		6
#define	ISTRUCPARSERC_OVERFLOW		7
#define	ISTRUCPARSERC_NOSTRUCTURE	9
#define	ISTRUCPARSERC_NOFUNCTION	10
#define	ISTRUCPARSERC_NOTIMPLEMENTED	11

#define	ISTRUCMAX_FIELDNAME_LEN		32


/*
	utility macros
*/

/*
	The following utility macros all relate to a structure
	that the user must put in the macro: "ISTRUCNAME", e.g.

		#undef	ISTRUCNAME
		#define	ISTRUCNAME	struct mystruc
		...

	This is a bit awkward, but it eases the coding of the
	parse structure, and it guarantees that the same structure
	is used througout.
*/

#define	ISTRUC_OFFSETOF(e)		(&((ISTRUCNAME *)0)->e)
#define	ISTRUC_SIZEOF(e)		sizeof(((ISTRUCNAME *)0)->e)

#define	ISTRUCFIELD_ENTRY(n,e,f)	{	\
	n,					\
	ISTRUC_OFFSETOF(e),			\
	ISTRUC_SIZEOF(e),			\
	f					\
}

#define	ISTRUCFIELD_INT(n,e)	ISTRUCFIELD_ENTRY(n,e,istruc0_parseInt)
#define	ISTRUCFIELD_STR(n,e)	ISTRUCFIELD_ENTRY(n,e,istruc0_parseStr)
#define	ISTRUCFIELD_CHR(n,e)	ISTRUCFIELD_ENTRY(n,e,istruc0_parseChr)
#define	ISTRUCFIELD_USER(n,e,f)	ISTRUCFIELD_ENTRY(n,e,f)

/*
	structs, types
*/

typedef ISTRUCPARSE_RC (*fnItemParse_t)(void *pOut, char *pszIn, int iSize) ;

struct istrucField {
	char *pszFieldName ;
	int iOffset ;
	int iSize ;
	fnItemParse_t upFn ;
	int iFieldNameLen ;	/* filled in by ".._parseinit()" */
} ;


/*
	API function prototypes
*/

ISTRUC_BOOL istruc_parseinit(struct istrucField *paFields, int nFields) ;
ISTRUCPARSE_RC istruc_parse(void *pStruc, char *pszLine) ;
void istruc_parsefini() ;


/*
	API internal entry point prototypes
*/

ISTRUCPARSE_RC istruc0_parseInt(void *pItemOut, char *pszIn, int iSize) ;
ISTRUCPARSE_RC istruc0_parseStr(void *pItemOut, char *pszIn, int iSize) ;
ISTRUCPARSE_RC istruc0_parseChr(void *pItemOut, char *pszIn, int iSize) ;

/* #ifndef ISTRUC_H_INC */
#endif

/* end of istruc.h */
