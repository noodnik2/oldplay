#define	MSRC_Revision		21

/*

	db.c

*/


/*
	#includes
*/

#include	"compiler.h"

#include	<stdio.h>
#include	<ctype.h>
#include	<memory.h>
#include	<string.h>

#include	"db.h"


/*
	internal macros
*/

#define	COPYFIELD(t,f,l)	copyField(t,f,l,sizeof(t)-1)


/*
	internal function prototypes
*/

static char *pszLStrip(char *pszText) ;
static DB_BOOL readRecord(FILE *fp, char *pszRecord) ;
static DB_BOOL isSectionRecord(char *pszRecord, struct db0_SecSpec *pss) ;
static DB_BOOL isColRecord(char *pszRecord, struct db0_ColSpec *pcs) ;
static DBRS_RC readSection(
	FILE *fp,
	char *pszRecord,
	struct db_Row *pdbr
) ;
static int minLen(int i0, int i1) ;
static void copyField(char *pszTo, const char *pszFrom, int iLen, int iLenMax) ;


/*
	entry points
*/

DB_RC dbRead(struct db_Process *pdbp)
{
	FILE *fp ;
	DB_RC dbrc ;
	DBRS_RC dbrsrc ;
	static struct db_Row dbr ;		/* static because large	*/
	char szRecord[DBMAX0_RECORDLEN+1] ;

	fp= fopen(pdbp->szDBName, "r") ;
	if (!fp) {
		fprintf(stderr, "dbRead: can't open '%s'\n", pdbp->szDBName) ;
		return(DBRC_NOFILE) ;
	}

	dbrc= DBRC_SUCCESS ;
	if (readRecord(fp, szRecord)) {

		do {

			dbrsrc= readSection(fp, szRecord, &dbr) ;
			if (dbrsrc == DBRSRC_NOTSECTION) {
				if (!readRecord(fp, szRecord)) {
					break ;
				}
				continue ;
			}
			dbrc= pdbp->fnProcessRow(&dbr, dbrsrc) ;

		} while(dbrc == DBRC_SUCCESS) ;

	}

	fclose(fp) ;
	return(dbrc) ;
}


/*
	internal functions
*/

static DBRS_RC readSection(
	FILE *fp,
	char *pszRecord,
	struct db_Row *pdbr
) {
	struct db0_SecSpec ss ;
	struct db0_ColSpec cs ;
	struct db_Col *pdbc ;
	int iLen ;

	if (!isSectionRecord(pszRecord, &ss)) {
		return(DBRSRC_NOTSECTION) ;
	}

	memset(pdbr, '\0', sizeof(struct db_Row)) ;
	COPYFIELD(pdbr->szName, ss.pName, ss.iNameLen) ;

	pdbr->nCols= 0 ;
	while(/*CONSTCOND*/1) {
		if (!readRecord(fp, pszRecord)) {
			pszRecord[0]= '\0' ;
			break ;
		}
		if (isSectionRecord(pszRecord, &ss)) {
			break ;
		}
		if (isColRecord(pszRecord, &cs)) {
			if ((pdbr->nCols)>= (DBMAX_NCOLS-1)) {
				return(DBRSRC_OVERFLOW) ;
			}
			pdbc= pdbr->colArray + pdbr->nCols ;
			memset(pdbc, '\0', sizeof(struct db_Col)) ;
			COPYFIELD(pdbc->szName, cs.pName, cs.iNameLen) ;
			COPYFIELD(pdbc->szValue, cs.pValue, cs.iValueLen) ;
			pdbr->nCols++ ;
		}
	}

	return(DBRSRC_SUCCESS) ;
}

static DB_BOOL isCommentRecord(char *pszRecord)
{
	char *pName ;

	pName= pszLStrip(pszRecord) ;
	if (pName[0] == ';') return(DBBOOL_TRUE) ;
	return(DBBOOL_FALSE) ;
}

static DB_BOOL isColRecord(char *pszRecord, struct db0_ColSpec *pcs)
{
	char *p, *pName, *pValue ;
	int iNameLen, iValueLen ;

	if (isCommentRecord(pszRecord)) return(DBBOOL_FALSE) ;

	pName= pszLStrip(pszRecord) ;
	for (p= pName; (*p) && ((*p) != '='); p++) ;

	if (p[0] != '=') return(DBBOOL_FALSE) ;

	for (
		iNameLen= (int) (p - pName);
		iNameLen> 0;
		iNameLen--
	) {
		if (!isspace(pName[iNameLen-1])) break ;
	}
	pcs->pName= pName ;
	pcs->iNameLen= iNameLen ;

	pValue= pszLStrip(p + 1) ;
	for (
		iValueLen= (int) strlen(pValue);
		iValueLen> 0;
		iValueLen--
	) {
		if (!isspace(pValue[iValueLen-1])) break ;
	}
	pcs->pValue= pValue ;
	pcs->iValueLen= iValueLen ;

	return(DBBOOL_TRUE) ;
}

static DB_BOOL isSectionRecord(char *pszRecord, struct db0_SecSpec *pss)
{
	char *pStartRecord, *pStartSectionName, *pEndSectionName ;

	pStartRecord= pszLStrip(pszRecord) ;

	if (pStartRecord[0] != '[') {
		return(DBBOOL_FALSE) ;
	}

	for (
		pEndSectionName= pStartSectionName= pStartRecord + 1;
		(pEndSectionName[0] != '\0') && (pEndSectionName[0] != ']');
		pEndSectionName++
	) ;

	if (pEndSectionName[0] != ']') {
		return(DBBOOL_FALSE) ;
	}

	pss->pName= pStartSectionName ;
	pss->iNameLen= (int) (pEndSectionName - pStartSectionName) ;

	return(DBBOOL_TRUE) ;
}

static DB_BOOL readRecord(FILE *fp, char *pszRecord)
{
	if (!fgets(pszRecord, DBMAX0_RECORDLEN+1, fp)) {
		return(DBBOOL_FALSE) ;
	}
	pszRecord[strlen(pszRecord)-1]= '\0' ;
	return(DBBOOL_TRUE) ;
}

static char *pszLStrip(char *pszText)
{
	char *p ;

	for (p= pszText; (*p) && isspace(*p); p++) ;
	return(p) ;
}

static void copyField(char *pszTo, const char *pszFrom, int iLen, int iMaxLen)
{
	int iLenCopy ;

	if (iLen<= iMaxLen) {
		memcpy(pszTo, pszFrom, iLen) ;
		pszTo[iLen]= '\0' ;
		return ;
	}
		
	iLenCopy= iMaxLen - 3 ;
	memcpy(pszTo, pszFrom, iLenCopy) ;
	memcpy(pszTo + iLenCopy, "...", 4) ;
}

/* end of db.c */
