#define	MSRC_Revision		6
#define	MSRC_DEBUG		0

/*

	inis.c				

*/


/*
	#includes
*/
#include	"compiler.h"
#include	<stdio.h>
#include	"inis.h"


/*
	internal static data
*/

static unsigned long lineno= 0L ;


/*
	internal functions
*/

static INISPARSE_RC parseMultiple(
	FILE *fpInput,
	struct inisSection *paSections,
	int nSections
) ;

static INIS_BOOL parseFileSection(
	FILE *fpInput,
	char *pszRecord,
	struct inisSection *pSection,
	ISTRUCPARSE_RC *pprc
) ;

static INIS_BOOL getNextFileSection(FILE *fpInput, char *pszRecord) ;
static INIS_BOOL isFileComment(char *pszRecord) ;
static INIS_BOOL isFileSection(char *pszRecord, char *pszSection) ;

static struct inisSection *findSectionInList(
	char *pszSectionName,
	struct inisSection *piSections,
	int nSections
) ;

static INIS_BOOL getLineFromFile(
	FILE *fpInput,
	char *pszRecord,
	char **ppStartOfLine
) ;


/*
	API entry points
*/

INISPARSE_RC inis_parseSingle(
	char *pszFileName,
	char *pszSectionName,
	void *pStruc,
	struct istrucField *paFields,
	int nFields
) {
	struct inisSection sa[1] ;

	sa[0].pStruc= pStruc ;
	sa[0].pszSectionName= pszSectionName ;
	sa[0].paFields= paFields ;
	sa[0].nFields= nFields ;

	return(inis_parseMultiple(pszFileName, sa, 1)) ;
}

INISPARSE_RC inis_parseMultiple(
	char *pszFileName,
	struct inisSection *paSections,
	int nSections
) {
	FILE *fpInput ;
	INISPARSE_RC prc ;

	lineno= 0L ;

	if (!pszFileName) {
		return(INISPARSERC_NOFILE) ;
	}

	fpInput= fopen(pszFileName, "r") ;
	if (!fpInput) {
		return(INISPARSERC_NOFILE) ;
	}

	prc= parseMultiple(fpInput, paSections, nSections) ;

	fclose(fpInput) ;
	return(prc) ;
}

unsigned long inis_lineno()
{
	return(lineno) ;
}

/*
	internal functions
*/


/*
	static INISPARSE_RC parseMultiple() ;

	logic:
		eof= getNextFileSection(s) ;
		while(!eof) {
			if (wantsection) eof= parseFileSection(s) ;
			else 		 eof= getNextFileSection(s) ;
		}

*/

static INISPARSE_RC parseMultiple(
	FILE *fpInput,
	struct inisSection *paSections,
	int nSections
) {
	INIS_BOOL bEof ;
	char szRecord[INISMAX_RECORDSIZE+1] ;
	struct inisSection *pSection ;
	ISTRUCPARSE_RC prc ;

	bEof= !getNextFileSection(fpInput, szRecord) ;
	while(!bEof) {
		pSection= findSectionInList(
			szRecord,
			paSections,
			nSections
		) ;
		if (!pSection) {
			bEof= !getNextFileSection(fpInput, szRecord) ;
			continue ;
		}
		bEof= parseFileSection(fpInput, szRecord, pSection, &prc) ;
		if (prc != ISTRUCPARSERC_SUCCESS) {
			return(INISPARSERC_ISTRUCPARSERCBASE + prc) ;
		}
	}
	return(INISPARSERC_SUCCESS) ;
}

static INIS_BOOL parseFileSection(
	FILE *fpInput,
	char *pszRecord,
	struct inisSection *pSection,
	ISTRUCPARSE_RC *pprc
) {
	char *pStartOfLine ;
	ISTRUCPARSE_RC prc ;
	INIS_BOOL bEof ;

	if (!pSection) {
		(*pprc)= INISPARSERC_NOSTRUCTURE ;
		return(INISBOOL_FALSE) ;
	}

	istruc_parseInit(pSection->paFields, pSection->nFields) ;

	bEof= INISBOOL_TRUE ;
	(*pprc)= ISTRUCPARSERC_SUCCESS ;
	while(getLineFromFile(fpInput, pszRecord, &pStartOfLine)) {
		if (isFileComment(pStartOfLine)) continue ;
		if (isFileSection(pStartOfLine, pszRecord)) {
			bEof= INISBOOL_FALSE ;
			break ;
		}
		prc= istruc_parse(pSection->pStruc, pszRecord) ;
		/*
			NOTE:	perhaps there should be an option that
				would cause parse errors such as this
				to be ignored here...
		*/
		if (prc != ISTRUCPARSERC_SUCCESS) {
			(*pprc)= prc ;
			bEof= INISBOOL_FALSE ;
			break ;
		}
	}

	istruc_parseFini() ;
	return(bEof) ;
}

static INIS_BOOL getNextFileSection(FILE *fpInput, char *pszRecord)
{
	char *pStartOfLine ;

	while(getLineFromFile(fpInput, pszRecord, &pStartOfLine)) {
		if (isFileComment(pStartOfLine)) continue ;
		if (!isFileSection(pStartOfLine, pszRecord)) continue ;
		return(INISBOOL_TRUE) ;
		/*NOTREACHED*/
	}
	return(INISBOOL_FALSE) ;
}

/*
	'pszRecord' must point of left-justified record
*/
static INIS_BOOL isFileComment(char *pszRecord)
{
	char c ;

	c= pszRecord[0] ;
	if ((c == '\0') || (c == ';') || (c == '#')) {
		return(INISBOOL_TRUE) ;
	}
	return(INISBOOL_FALSE) ;
}

/*
	'pszRecord' must point of left-justified record
*/
static INIS_BOOL isFileSection(char *pszRecord, char *pszSection)
{
	char c ;
	char *pStart, *pEnd, *pIn, *pOut ;

	/*
		identify line as section name
	*/
	c= pszRecord[0] ;
	if (c != '[') return(INISBOOL_FALSE) ;
	pStart= pszRecord + 1 ;
	for (pEnd= pStart; pEnd[0] != '\0'; pEnd++) {
		if (pEnd[0] == ']') break ;
	}
	if (pEnd[0] != ']') return(INISBOOL_FALSE) ;	/* invalid; ignore */

	/*
		copy section name to "*pszSection"
	*/
	for (
		pIn= pStart, pOut= pszRecord ;
		pIn< pEnd ;
		pIn++, pOut++
	) {
		pOut[0]= pIn[0] ;
	}
	pOut[0]= '\0' ;
	return(INISBOOL_TRUE) ;
}

#if 0
	UNUSED
static char *pszStrip(char *pszLine)
{
	char c, *p ;

	for (p= pszLine; (c= p[i]) != '\0'; p++) {
		if (!isspace(c)) break ;
	}
	return(p) ;
}
#endif

/*
	assumes that "pszSectionName" is not null, and
	that "piSections" is not null, and points to an
	array that contains at least "nSections" entries.
*/
static struct inisSection *findSectionInList(
	char *pszSectionName,
	struct inisSection *piSections,
	int nSections
) {
	int i ;
	char *pszWantSectionName ;

	for (i= 0; i< nSections; i++) {
		pszWantSectionName= piSections[i].pszSectionName ;
		if (!pszWantSectionName) continue ;
		if (strcmp(pszSectionName, pszWantSectionName) == 0) {
			return(piSections + i) ;
		}
	}
	return(0) ;
}

static INIS_BOOL getLineFromFile(
	FILE *fpInput,
	char *pszRecord,
	char **ppStartOfLine
) {
	char *pStartOfLine ;

	if (!fgets(pszRecord, INISMAX_RECORDSIZE, fpInput)) {
		return(INISBOOL_FALSE) ;
	}
	pszRecord[strlen(pszRecord)-1]= '\0' ;

	for (
		pStartOfLine= pszRecord;
		pStartOfLine[0] != '\0';
		pStartOfLine++
	) {
		if (!isspace(pStartOfLine[0])) break ;
	}

	(*ppStartOfLine)= pStartOfLine ;
	lineno++ ;
	return(INISBOOL_TRUE) ;
}

/* end of inis.c */
