#define	MSRC_Revision		63
#define	MSRC_Version		1

/*

	mklist.c			Make list by applying expression to db
					Copyright (C) 1996, Marty Ross

*/


/*
	#includes
*/

#include	"compiler.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<memory.h>
#include	<string.h>
#include	<time.h>

#include	"clgetopt.h"
#include	"mklist.h"
#include	"db.h"
#include	"pe.h"
#include	"tv.h"
#include	"wc.h"
#include	"output.h"


/*
	internal #defines
*/
#define	MKLISREV_MAJOR(r)	((r) / 100)
#define	MKLISREV_MINOR(r)	((r) % 100)


/*
	internal function prototypes
*/

static void init(int argc, char *argv[]) ;
static MKLIST_E mklist() ;
static void finiall() ;
static void signon() ;
static void syntax() ;
static DB_RC processEntry(struct db_Row *, DBRS_RC) ;
static int findParseVar(char *pszVname, int *pvid) ;
static int getEvalVarLong(int vid, long *plval) ;
static void badArgFatalError(char *pMessage) ;
static char *pszGetValue(char *pszName, int iLenName, void *pSpecial) ;
static MKLIST_E parseExp(
	struct pe_ParseArgs *ppexp,
	unsigned char *paucExprProg,
	char *pszExpression
) ;
static void deliverListItem(
	long lExprValue,
	long lSortValue,
	const void *pUserLine,
	void *pExtra
) ;
static MKLIST_BOOL getLongOpt(
	long *plValue,
	const char *pcszValueText
) ;

/*
	internal data
*/

static struct mklist_parms mlp ;
static struct pe_ParseArgs pexp_select ;
static struct pe_ParseArgs pexp_sort ;
static char szExpressionVar[MKLISTMAX_EXPRESSION_VARS][DBMAX_COLNAMELEN+1] ;
static int nExpressionVars ;
static long lBigN= 1L ;
static long lLittleN= 1L ;
static struct db_Row *pglobal_dbr ;

/*
	entry points
*/

void main(int argc, char *argv[])
{
	MKLIST_E mke ;

	init(argc, argv) ;
	mke= mklist() ;
	finiall() ;
	exit(mke) ;
	/*NOTREACHED*/
}


/*
	start of internal functions
*/

static void init(int argc, char *argv[])
{
	int i ;

	memset(&mlp, '\0', sizeof(struct mklist_parms)) ;
	strcpy(mlp.szDBFileName, "mklist.db") ;
	strcpy(mlp.szSubstitute, "%@%") ;
	strcpy(mlp.szSortExpression, "_n") ;

	clinit(argv) ;
	while((i= clgetopt(argc, argv, "f:hO:o:qR:s:z"))> 0) {

		switch(i) {
			case 'f':
				strcpy(mlp.szDBFileName, cloptarg) ;
				break ;
			case 'O':
				mlp.bReverseSort= MKLISTBOOL_TRUE ;
				strcpy(mlp.szSortExpression, cloptarg) ;
				break ;	
			case 'o':
				mlp.bReverseSort= MKLISTBOOL_FALSE ;
				strcpy(mlp.szSortExpression, cloptarg) ;
				break ;	
			case 'R':
				if (!getLongOpt(&mlp.lRandSeed, cloptarg)) {
					badArgFatalError("invalid seed") ;
					/*NOTREACHED*/
				}
				mlp.bUserSeed= MKLISTBOOL_TRUE ;
				break ;
			case 's':
				strcpy(mlp.szSubstitute, cloptarg) ;
				break ;
			case 'h':
				mlp.bHelp= MKLISTBOOL_TRUE ;
				break ;
			case 'q':
				mlp.bQuiet= MKLISTBOOL_TRUE ;
				break ;
			case 'z':
				mlp.iDebugLevel++ ;
				break ;
			default:
				badArgFatalError(0) ;
				/*NOTREACHED*/
				break ;
		}

	}

	if (!mlp.bQuiet) {
		signon() ;
	}

	if (mlp.bHelp) {
		syntax() ;
		exit(MKLISTE_SUCCESS) ;
		/*NOTREACHED*/
	}

	if (cloptind != (argc - 1)) {
		if (mlp.iDebugLevel> 0) {
			fprintf(stderr, "argc=%d\n", argc) ;
		}
		badArgFatalError("invalid number of arguments") ;
		/*NOTREACHED*/
	}

	strcpy(mlp.szExpression, argv[cloptind]) ;

	srand(
		(unsigned int) (
			mlp.bUserSeed
		      ? (long) mlp.lRandSeed
		      : (long) time(0)
		)
	) ;
}	

static MKLIST_E mklist()
{
	struct output_initparms oip ;
	struct tv_parms tvp ;
	struct db_Process dbp ;
	DB_RC dbrc ;
	unsigned char exprProg[MKLIST_EXPRPROGSIZE] ;
	unsigned char exprSortProg[MKLIST_EXPRPROGSIZE] ;
	MKLIST_E mke ;
	OUTPUT_RC orc ;

	/*
		(1)	Parse the selection expression
	*/
	mke= parseExp(&pexp_select, exprProg, mlp.szExpression) ;
	if (mke != MKLISTE_SUCCESS) return(mke) ;

	/*
		(2)	Parse the sort expression
	*/
	mke= parseExp(&pexp_sort, exprSortProg, mlp.szSortExpression) ;
	if (mke != MKLISTE_SUCCESS) return(mke) ;

	/*
		(3)	Initialize the "output" and the "tv" modules to
			collect output and format the entries, respectively.
	*/
	memset(&oip, '\0', sizeof(struct output_initparms)) ;
	oip.pfnDeliver= deliverListItem ;
	oip.pUserData= &dbp ;
	orc= outputInit(&oip) ;
	if (orc != OUTPUTRC_SUCCESS) return(MKLISTE_CANTOUTPUT) ;

	memset(&tvp, '\0', sizeof(struct tv_parms)) ;
	tvp.pfnLookup= pszGetValue ;
	tv_Init(&tvp) ;

	/*
		(4)	Traverse all of the entries in the "master list"
			of files to be considered, calling the function
			"processEntry" for each one.  This function will
			use the "tv" and "output" modules to format and
			queue the output entries, respectively.
	*/
	dbp.fnProcessRow= processEntry ;
	strcpy(dbp.szDBName, mlp.szDBFileName) ;
	dbrc= dbRead(&dbp) ;

	/*
		(5)	If the "master list" was traversed successfully,
			then output the list we've collected with the
			"output" module.  Prepare the final return code
			according to the success of this operation.
	*/
	mke= MKLISTE_SUCCESS ;
	if (dbrc == DBRC_SUCCESS) {
		orc= outputDumpItems() ;
		if (orc != OUTPUTRC_SUCCESS) {
#if 0
	nu?
			if (!mlp.bQuiet)
#endif
			fprintf(
				stderr,
				"NOTE: outputDumpItems returns %d\n",
				dbrc
			) ;
			mke= MKLISTE_CANTOUTPUT ;
		}
	}

	/*
		(6)	Terminate the operation of the "output" and "tv"
			modules (in reverse order of their initialization).
	*/
	tv_Fini() ;
	outputFini() ;

	/*
		(7)	Return appropriate return codes to caller.
	*/
	if (dbrc != DBRC_SUCCESS) {
#if 0
	nu?
		if (!mlp.bQuiet) 
#endif
		fprintf(stderr, "NOTE: dbRead returns %d\n", dbrc) ;
		return(MKLISTE_IOERROR) ;
	}

	return(mke) ;
}

static void finiall()
{
}

static void signon()
{
	FILE *fp ;

	fp= stderr ;

	fprintf(
		fp,
		"mklist %d.%d.%d - Make List based on criterion\n",
		MSRC_Version,
		MKLISREV_MAJOR(MSRC_Revision),
		MKLISREV_MINOR(MSRC_Revision)
	) ;
	fprintf(fp, "Copyright (C) 1996, Marty Ross\n") ;
	fprintf(fp, "\n") ;
}

static void syntax()
{
	static void *pText[]= {
		"syntax:",
		"\tmklist [options] \"expr\"",
		"options:",
		"\tf fn\t\tuse database contained in 'fn'",
		"\to expr\t\tsort by 'expr' instead of '_n'",
		"\tO expr\t\tsame as 'o', but reverse sort order",
		"\tR seed\t\tuse seed 'seed' instead of 'time(0)'",
		"\ts str\t\tsubstite using 'str' instead of '%@%'",
		"\th\t\tprint this help text",
		"\tq\t\tquiet operation",
		"\tz\t\tincrease general debug level",
		"Special words in expressions:",
		"\t_n\t\toutput item serial number",
		"\t_N\t\titem number in database",
		"\t_rand100\trandom number between 0 and 99",
		"\t_rand\t\trandom number between 0 and _RANDMAX",
		"\t_RANDMAX\tmaximum random number generated by _rand",
		"Special substitution (prefix with '=' for www-encoding):",
		"\t%@%\t\tname of item",
		"\t%*%\t\tlist of all keywords defined for item",
		"\t%x%\t\tvalue of keyword 'x'",
	} ;
#define	N_TEXT	(sizeof(pText) / sizeof(pText[0]))
	int i ;

	for (i= 0; i< N_TEXT; i++) {
		printf("%s\n", pText[i]) ;
	}
}


/*

*/

static MKLIST_E parseExp(
	struct pe_ParseArgs *ppexp,
	unsigned char *paucExprProg,
	char *pszExpression
) {
	PEPARSE_RC peprc ;

	memset(ppexp, '\0', sizeof(struct pe_ParseArgs)) ;
	ppexp->pExprProg= paucExprProg ;
	ppexp->iExprProgSize= MKLIST_EXPRPROGSIZE ;
	ppexp->fnFindVar= findParseVar ;
	peprc= pe_parseExpression(pszExpression, ppexp) ;
	if (peprc != PEPARSERC_SUCCESS) {
		clerror("invalid selection expression") ;
		return(MKLISTE_BADEXPR) ;
	}
	return(MKLISTE_SUCCESS) ;
}

/*

*/

#define	MKLISTGVALLMAX_BUFSIZE		1024

static char *pszGetValueAll(struct db_Row *pdbr)
{
	/*
		NOTE:	The following buffer must be static, as the
			caller of this function receives what we put
			into it indirectly from our return value.
	*/
	static char szStaticBuff[MKLISTGVALLMAX_BUFSIZE+1] ;
	int i, iLenTot, iNCols ;
	int iLenName, iLenValue ;
	char *pszColName, *pszColValue ;

	iLenTot= 0 ;
	iNCols= pdbr->nCols ;
	for (i= 0; i< iNCols; i++) {
		pszColName= pdbr->colArray[i].szName ;
		pszColValue= pdbr->colArray[i].szValue ;
		iLenName= strlen(pszColName) ;
		iLenValue= strlen(pszColValue) ;
		if ((iLenTot+iLenName+iLenValue+6)>= MKLISTGVALLMAX_BUFSIZE) {
			memcpy(szStaticBuff + iLenTot, "...", 3) ;
			iLenTot+= 3 ;
			break ;
		}
		memcpy(szStaticBuff+iLenTot, pszColName, iLenName) ;
		iLenTot+= iLenName ;
		szStaticBuff[iLenTot++]= '=' ;
		szStaticBuff[iLenTot++]= '"' ;
		memcpy(szStaticBuff+iLenTot, pszColValue, iLenValue) ;
		iLenTot+= iLenValue ;
		szStaticBuff[iLenTot++]= '"' ;
		szStaticBuff[iLenTot++]= ' ' ;
	}
	szStaticBuff[iLenTot]= '\0' ;
	return(szStaticBuff) ;
}

static char *pszGetValue0(char *pszName, int iLenName, void *pSpecial)
{
	struct db_Row *pdbr ;
	int i, l, nCols ;
	char *pszColName ;

	if (iLenName == 0) return((char *) NULL) ;

	pdbr= (struct db_Row *) pSpecial ;
	if ((iLenName == 1) && (pszName[0] == '@')) {
		return(pdbr->szName) ;
	}
	if ((iLenName == 1) && (pszName[0] == '*')) {
		return(pszGetValueAll(pdbr)) ;
	}

	nCols= pdbr->nCols ;
	for (i= 0; i< nCols; i++) {
		l= strlen(pszColName= pdbr->colArray[i].szName) ;
		if (l == iLenName) {
			if (memcmp(pszColName, pszName, l) == 0) {
				return(pdbr->colArray[i].szValue) ;
			}
		}
	}

	return((char *) NULL) ;
}

static char *pszGetValue(char *pszName, int iLenName, void *pSpecial)
{
	static char szWCodeBuf[MKLISTMAX_SUBSTITUTE_LEN+1] ;
	char *pszValue ;
	MKLIST_BOOL bWCode ;

	if (iLenName == 0) return((char *) NULL) ;

	bWCode= MKLISTBOOL_FALSE ;
	if (pszName[0] == '=') {		/* WWW-encoding prefix */
		iLenName-- ;
		pszName++ ;
		bWCode= MKLISTBOOL_TRUE ;
	}

	pszValue= pszGetValue0(pszName, iLenName, pSpecial) ;

	if (pszValue && bWCode) {
		wcCode(pszValue, szWCodeBuf, sizeof(szWCodeBuf)) ;
		pszValue= szWCodeBuf ;
	}

	return(pszValue) ;	
}

#define	MKLISTMAX_OUTPUTLINESIZE	2048

static void doOutput(
	struct db_Row *pdbr,
	long lExprValue,
	long lSortValue
) {
	/*
		NOTE:	this buffer doesn't need to be static;
			it's static because of its size--we dont
			want to worry about small stack runtimes.
	*/
	static char szBuff[MKLISTMAX_OUTPUTLINESIZE+1] ;

	tv_Substitute(
		mlp.szSubstitute,
		szBuff,
		MKLISTMAX_OUTPUTLINESIZE,
		(void *) pdbr
	) ;
	outputAddItem(
		lExprValue,
		lSortValue,
		szBuff,
		strlen(szBuff) + 1
	) ;
}

/*
	this routine is called back from within "outputDumpItems()"
*/
static void deliverListItem(
	long lExprValue,
	long lSortValue,
	const void *pUserLine,
	void *pExtra
) {
	if (!lExprValue) return ;	/* this never happens now */
	printf("%s\n", (char *) pUserLine) ;
}

static PEEVAL_RC evalEntry(
	struct pe_ParseArgs *ppexp,
	long *plResult
) {
	struct pe_EvalArgs peeval ;
	long exprStack[MKLIST_EXPRSTACKSIZE] ;

	memset(&peeval, '\0', sizeof(struct pe_EvalArgs)) ;
	peeval.pExprStack= exprStack ;
	peeval.iExprStackSize= MKLIST_EXPRSTACKSIZE ;
	peeval.fnGetVarLong= getEvalVarLong ;
	return(pe_evalExpression(ppexp, &peeval, plResult)) ;
}

#if 0
static void printEntry(struct db_Row *pdbr, DBRS_RC dbrsrc, long lResult)
{
	int i ;

	printf(
		"entry '%s' has %d values, rc=%d\n",
		pdbr->szName,
		pdbr->nCols,
		dbrsrc
	) ;
	for (i= 0; i< pdbr->nCols; i++) {
		printf(
			"\t%d.) %s=%s\n",
			i+1,
			pdbr->colArray[i].szName,
			pdbr->colArray[i].szValue
		) ;
	}
	printf("\tlResult='%ld'\n", lResult) ;
}
#endif

/*
	static DB_RC processEntry(struct db_Row *pdbr, DBRS_RC dbrsrc)

	This function is called back from within "dbRead()",
	once for each row (entry) in the database.
*/
static DB_RC processEntry(struct db_Row *pdbr, DBRS_RC dbrsrc)
{
	long lExprResult, lSortResult ;
	PEEVAL_RC pevrc ;

	/*
		increment global "lBigN" counter;
		this function is called once for each
		entry in the database.
	*/
	lBigN++ ;

	/*
		NOTE:	we should probably ignore any entry for which
			the value "dbrsrc" is not DBRSRC_SUCCESS.
	*/
#if 0
	if (dbrsrc != DBRSRC_SUCCESS) {
		printf("lBigN=%ld dbrsrc=%d\n", lBigN, (int) dbrsrc) ;
	}
#endif

	/*
		Globalize current row data for parse callback
	*/
	pglobal_dbr= pdbr ;

	/*
		evaluate main expression
	*/
	pevrc= evalEntry(&pexp_select, &lExprResult) ;
	if (pevrc != PEEVALRC_SUCCESS) return(DBRC_FAILURE) ;

	/*
		if didn't pass main test, return now
	*/
#if 0
	printEntry(pdbr, dbrsrc, lResult) ;
#endif
	if (!lExprResult) return(DBRC_SUCCESS) ;

	lLittleN++ ;			/* count item passing main test	*/

	/*
		evaluate sort expression
	*/
	pevrc= evalEntry(&pexp_sort, &lSortResult) ;
	if (pevrc != PEEVALRC_SUCCESS) return(DBRC_FAILURE) ;
	if (mlp.bReverseSort) lSortResult= -lSortResult ;

	doOutput(pdbr, lExprResult, lSortResult) ;
	return(DBRC_SUCCESS) ;
}

/*

*/

static void fatalVname(char *pszVname)
{
	clerror("variable name invalid") ;
	exit(MKLISTE_OVERFLOW) ;
	/*NOTREACHED*/
}

#define	PVVIDSYS_LITTLEN	-1
#define	PVVIDSYS_BIGN		-2
#define	PVVIDSYS_RAND		-3
#define	PVVIDSYS_RAND100	-4
#define	PVVIDSYS_RANDMAX	-5

static int findParseVar(char *pszVname, int *pvid)
{
	int sl, id ;

	sl= strlen(pszVname) ;
	if ((sl< 1) || (sl> DBMAX_COLNAMELEN)) {
		fatalVname(pszVname) ;
		/*NOTREACHED*/
	}

	if (pszVname[0] == '_') {
		if (strcmp(pszVname+1, "n") == 0) {
			(*pvid)= PVVIDSYS_LITTLEN ;
			return(1) ;
		}
		if (strcmp(pszVname+1, "N") == 0) {
			(*pvid)= PVVIDSYS_BIGN ;
			return(1) ;
		}
		if (strcmp(pszVname+1, "rand") == 0) {
			(*pvid)= PVVIDSYS_RAND ;
			return(1) ;
		}
		if (strcmp(pszVname+1, "RANDMAX") == 0) {
			(*pvid)= PVVIDSYS_RANDMAX ;
			return(1) ;
		}
		if (strcmp(pszVname+1, "rand100") == 0) {
			(*pvid)= PVVIDSYS_RAND100 ;
			return(1) ;
		}
		return(0) ;
	}

	for (id= 0; id< nExpressionVars; id++) {
		if (strcmp(pszVname, szExpressionVar[id]) == 0) {
			(*pvid)= id ;
			return(1) ;
		}
	}

	id= nExpressionVars ;
	if (id>= MKLISTMAX_EXPRESSION_VARS) {
		clerror("expression contains too many variables") ;
		exit(MKLISTE_OVERFLOW) ;
		/*NOTREACHED*/
	}

	strcpy(szExpressionVar[id], pszVname) ;
	(*pvid)= id ;
	nExpressionVars++ ;

	return(1) ;
}


/*

*/

static int getEvalVarLong(int vid, long *plval)
{
	char *pVarName ;
	long lval, lResult ;
	double d, done, dten, drmax ;
	int i ;

	if (vid< 0) {			/* "system" variable	*/
		switch(vid) {

			case	PVVIDSYS_LITTLEN:
				(*plval)= lLittleN ;
				break ;

			case	PVVIDSYS_BIGN:
				(*plval)= lBigN ;
				break ;

			case	PVVIDSYS_RAND:
				(*plval)= (long) rand() ;
				break ;

			case	PVVIDSYS_RANDMAX:
				(*plval)= (long) RAND_MAX ;
				break ;

			case	PVVIDSYS_RAND100:
				done= (double) 1.0e0 ;
				dten= (double) 10.0e0 ;
				drmax= (double) RAND_MAX ;
				d= (double) rand() ;
				d*= dten ;
				d/= drmax + done ;
				d*= dten ;
				(*plval)= (long) d ;
				break ;

			default:
				return(0) ;	/* "never happens"	*/
				/*NOTREACHED*/
				break ;
		}
		return(1) ;
	}

	pVarName= szExpressionVar[vid] ;

	lResult= 0L ;
	for (i= 0; i< pglobal_dbr->nCols; i++) {
		if (strcmp(pVarName, pglobal_dbr->colArray[i].szName) == 0) {
			if (
				sscanf(
					pglobal_dbr->colArray[i].szValue,
					"%ld",
					&lval
				) == 1
			) {
				lResult= lval ;
			}
			break ;
		}
	}

	(*plval)= lResult ;
	return(1) ;
}


static MKLIST_BOOL getLongOpt(
	long *plValue,
	const char *pcszValueText
) {
	const char *pcszFormat ;
	long lValue ;

	pcszFormat= "%ld" ;
	if (pcszValueText[0] == '0') {
		if (pcszValueText[1] == 'x') {
			pcszFormat= "0x%lX" ;
		}
	}
	if (sscanf(pcszValueText, pcszFormat, &lValue) != 1) {
		return(MKLISTBOOL_FALSE) ;
	}
	(*plValue)= lValue ;
	return(MKLISTBOOL_TRUE) ;
}

static void badArgFatalError(char *pMessage)
{
	if (pMessage) clerror(pMessage) ;
	clerror("use 'h' option to get help") ;
	exit(MKLISTE_BADARGS) ;
	/*NOTREACHED*/
}

/* end of mklist.c */
