#define	MSRC_Revision		5
#define	MSRC_DEBUG		0

/*

	istruci.c		Structure Initialization: Input
				Copyright (C) 1996, Pointofsale Ltd.

*/


/*
	#includes
*/
#include	<stdio.h>
#include	<ctype.h>

#include	"istruc.h"

/*
	internal function prototypes
*/

static ISTRUCPARSE_RC ulParse(char *pszTextValue, unsigned long *pul) ;
static ISTRUCPARSE_RC sParse(
	char *pszTextValue,
	char *psTextItemOut,
	int iMaxSize,
	int *piOutSize
) ;


/*
	external API data
*/
extern struct istrucField *istruc0_paFields ;
extern int istruc0_nFields ;


/*
	public API entry points
*/

ISTRUCPARSE_RC istruc_parse(void *pStruc, char *pszLine)
{
	int i ;
	int iVarPos ;
	int iVarLen ;
	int iEqualPos ;
	int iFieldIdx ;
	int iOffset ;
	int iSize ;
	char c ;
	char *psFieldName ;
	fnItemParse_t upFn ;

	/*
		ensure that we have a structure to search
	*/
	if (!istruc0_paFields || !istruc0_nFields) {
		return(ISTRUCPARSERC_NOSTRUCTURE) ;
	}

	/*
		search for '='
	*/	
	for (i= 0; (c= pszLine[i]) != '\0'; i++) {
		if (c == '=') break ;
	}

	if (c != '=') {			/* couldn't find '='	*/
		return(ISTRUCPARSERC_NOEQUAL) ;
	}
	iEqualPos= i ;

	/*
		find start of varname
	*/
	for (i= 0; i< iEqualPos; i++) {
		if (!isspace(pszLine[i])) break ;
	}
	if (i == iEqualPos) {		/* no varname		*/
		return(ISTRUCPARSERC_NONAME) ;
	}
	iVarPos= i ;
	psFieldName= pszLine + iVarPos ;

	/*
		find length of varname
	*/
	for (i= iEqualPos - 1; i> 0; i--) {
		if (!isspace(pszLine[i])) break ;
	}
	iVarLen= (i - iVarPos + 1) ;

	/*
		find variable in list
	*/
	for (i= 0; i< istruc0_nFields; i++) {
		if (iVarLen != istruc0_paFields[i].iFieldNameLen) continue ;
		if (
			memcmp(
				istruc0_paFields[i].pszFieldName,
				psFieldName,
				iVarLen
			) != 0
		) continue ;
		/* found it! */
		break ;
	}
	if (i>= istruc0_nFields) {		/* variable not found	*/
		return(ISTRUCPARSERC_NOTFOUND) ;
	}
	iFieldIdx= i ;

	upFn= istruc0_paFields[iFieldIdx].upFn ;	
	if (!upFn) {				/* no function		*/
		return(ISTRUCPARSERC_NOFUNCTION) ;
	}

	iSize= istruc0_paFields[iFieldIdx].iSize ;
	iOffset= istruc0_paFields[iFieldIdx].iOffset ;
	return(
		upFn(
			(void *) (((char *) pStruc) + iOffset),
			pszLine + iEqualPos + 1,
			iSize
		)
	) ;

}

/*
	semi-private API entry points
*/

ISTRUCPARSE_RC istruc0_parseInt(
	void *pItemOut,
	char *pszValueText,
	int iSize
) {
	unsigned long ulValue ;
	ISTRUCPARSE_RC prc ;

	prc= ulParse(pszValueText, &ulValue) ;
	if (prc != ISTRUCPARSERC_SUCCESS) {
		return(prc) ;
	}

	if (iSize == sizeof(unsigned short)) {
		if (ulValue> (unsigned long) 0x0FFFFL) {
			return(ISTRUCPARSERC_OVERFLOW) ;
		}
		*((unsigned short *) &((unsigned char *)pItemOut)[0])= (
			(unsigned short) ulValue
		) ;
		return(ISTRUCPARSERC_SUCCESS) ;
	}

	if (iSize == sizeof(unsigned long)) {
		*((unsigned long *) &((unsigned char *)pItemOut)[0])= (
			(unsigned long) ulValue
		) ;
		return(ISTRUCPARSERC_SUCCESS) ;
	}

	if (iSize == sizeof(unsigned char)) {
		if (ulValue> (unsigned long) 0x00FFL) {
			return(ISTRUCPARSERC_OVERFLOW) ;
		}
		*((unsigned char *) &((unsigned char *)pItemOut)[0])= (
			(unsigned char) ulValue
		) ;
		return(ISTRUCPARSERC_SUCCESS) ;
	}

	/*
		integers of "unknown" size are unimplemented
	*/
	return(ISTRUCPARSERC_NOTIMPLEMENTED) ;
}

ISTRUCPARSE_RC istruc0_parseChr(void *pItemOut, char *pszValueText, int iSize)
{
	int iLen ;
	ISTRUCPARSE_RC prc ;

	prc= sParse(pszValueText, (char *) pItemOut, iSize, &iLen) ;
	if (prc != ISTRUCPARSERC_SUCCESS) {
		return(prc) ;
	}
	return(ISTRUCPARSERC_SUCCESS) ;
}

ISTRUCPARSE_RC istruc0_parseStr(void *pItemOut, char *pszValueText, int iSize)
{
	int iLen ;
	ISTRUCPARSE_RC prc ;

	prc= sParse(pszValueText, (char *) pItemOut, iSize - 1, &iLen) ;
	if (prc != ISTRUCPARSERC_SUCCESS) {
		return(prc) ;
	}
	((char *) pItemOut)[iLen]= '\0' ;
	return(ISTRUCPARSERC_SUCCESS) ;
}

/*
	internal functions
*/

static ISTRUCPARSE_RC sParse(
	char *pszTextValue,
	char *psOutputItem,
	int iSizeMax,
	int *piSizeOutput
) {
	int iLen ;

	iLen= strlen(pszTextValue) ;
	if (iLen> iSizeMax) {
		return(ISTRUCPARSERC_OVERFLOW) ;
	}
	memcpy(psOutputItem, pszTextValue, iLen) ;
	(*piSizeOutput)= iLen ;
	return(ISTRUCPARSERC_SUCCESS) ;
}

static ISTRUCPARSE_RC ulParse(char *pszTextValue, unsigned long *pul)
{
	char *pszInput, *pszFormat ;
	long ul ;
	int l ;

	if (pszTextValue[0] == '\'') {
		l= strlen(pszTextValue) ;
		if (l != 3) return(ISTRUCPARSERC_BADVALUE) ;
		if (pszTextValue[2] != '\'') return(ISTRUCPARSERC_BADVALUE) ;
		(*pul)= (unsigned long) (((unsigned char *)pszTextValue)[1]) ;
		return(ISTRUCPARSERC_SUCCESS) ;
	}

	if ((pszTextValue[0] == '0') && (pszTextValue[1] == 'x')) {
		pszInput= pszTextValue + 2 ;
		pszFormat= "%lX" ;
	}
	else {
		pszInput= pszTextValue ;
		pszFormat= "%lu" ;
	}

	if (sscanf(pszInput, pszFormat, &ul) != 1) {
		return(ISTRUCPARSERC_BADVALUE) ;
	}

	(*pul)= ul ;
	return(ISTRUCPARSERC_SUCCESS) ;
}

/* end of istruci.c */
