#define	MSRC_Revision		13
#define	FOR_REAL		1

/*
	midifrm.c


	This is meant to be run as a "nph" CGI script.
	(it generates full HTTP headers)
*/

/*
	#includes
*/

#include	<stdio.h>
#include	<time.h>
#ifndef NO_STDLIB_H
#include	<stdlib.h>
#else
char *getenv() ;
#endif


/*
	#defines
*/

#define	MAX_SEEDTEXTLEN		64

#define	MAX_QUERY		4096
#define	MAX_QUERY2ADD		40

#define	MFMAX_EXPRLEN		4096
#define	MFMAX_CMDLEN		(MFMAX_EXPRLEN + 128)

#define	MAX_ENTRIES		100
#define	MAX_ENTRYNAMELEN	32
#define	MAX_ENTRYVALUELEN	255


/*
	main program
*/

typedef struct {
	char name[MAX_ENTRYNAMELEN+1] ;
	char val[MAX_ENTRYVALUELEN+1] ;
} entry ;


/*
	templates for external routines
*/

char *makeword(char *line, char stop) ;
char *fmakeword(FILE *f, char stop, int *len) ;
char x2c(char *what) ;
void unescape_url(char *url) ;
void plustospace(char *str) ;


/*
	templates for internal routines
*/
static void process_form(char *pszQuery, entry *pentries, int n) ;
static void initstdout4html() ;


/*
	main program
*/

void main(int argc, char *argv[])
{
	entry entries[MAX_ENTRIES] ;
	char szQueryWork[MAX_QUERY + MAX_QUERY2ADD + 1] ;
	char *cl ;
	char *pszQuery ;
	int x ;

	cl= getenv("REQUEST_METHOD") ;
	if (!cl) {
		initstdout4html() ;
		printf("Environment variable \"REQUEST_METHOD\" undefined!\n") ;
		exit(1) ;
	}

 	if (strcmp(cl, "GET")) {
		initstdout4html() ;
        	printf("This script should be referenced with a METHOD of GET.\n") ;
		printf("If you don't understand this, see this ") ;
		printf("<A HREF=\"http://www.ncsa.uiuc.edu/SDG/Software/Mosaic/Docs/fill-out-forms/overview.html\">forms overview</A>.%c",10) ;
        	exit(1) ;
	}

#if 0
	cl= getenv("CONTENT_TYPE") ;
	if (!cl) {
		initstdout4html() ;
		printf("Environment variable \"CONTENT_TYPE\" undefined!\n") ;
		exit(1) ;
	}

	if (strcmp(cl, "application/x-www-form-urlencoded") != 0) {
		initstdout4html() ;
		printf(
			"This script can only be used to decode form results.\n"
		) ;
		exit(1) ;
	}
#endif

	pszQuery= getenv("QUERY_STRING") ;
	if (!pszQuery) {
		initstdout4html() ;
		printf("No query information to decode.\n") ;
        	exit(1) ;
    	}
	if (strlen(pszQuery)>= MAX_QUERY) {
		initstdout4html() ;
		printf("Query string too long.\n") ;
        	exit(1) ;
	}
	strcpy(szQueryWork, pszQuery) ;

	for(x= 0; szQueryWork[0] != '\0'; x++) {
        	getword(entries[x].val, szQueryWork, '&') ;
        	plustospace(entries[x].val) ;
        	unescape_url(entries[x].val) ;
        	getword(entries[x].name, entries[x].val, '=') ;
	}

	/*
		pass to "process_form" a copy of the query that
		can be expanded by up to MAX_QUERY2ADD bytes...
	*/
	strcpy(szQueryWork, pszQuery) ;
	process_form(szQueryWork, entries, x) ;
	exit(0) ;
}

#define	FORM_ITEM		int
#define	FORMITEM_NONE		-1
#define	FORMITEM_EXPR1		0
#define	FORMITEM_EXPR2		1
#define	FORMITEM_RTYPE		2
#define	FORMITEM_STYPE		3
#define	FORMITEM_SEED		4
#define	NFORMITEMS		5

static char *pszFormItemNames[NFORMITEMS]= {
	"expr1",
	"expr2",
	"rtype",
	"stype",
	"seed",
} ;

static FORM_ITEM findFormItem(char *pszItemName)
{
	FORM_ITEM fi ;

	for (fi= 0; fi< NFORMITEMS; fi++) {
		if (strcmp(pszItemName, pszFormItemNames[fi]) == 0) {
			return(fi) ;
		}
	}
	return(FORMITEM_NONE) ;
}

static void doquery(char *pszQuery, char *pszFormItems[NFORMITEMS]) ;

/*
	NOTE:	pszQuery points to a buffer that can be
		expanded by up to MAX_QUERY2ADD bytes.
*/
static void process_form(char *pszQuery, entry *pentries, int n)
{
#if	FOR_REAL
	int i ;
	FORM_ITEM fi ;
	char *pszFormItems[NFORMITEMS] ;
	char szSeedText[MAX_SEEDTEXTLEN+1] ;
	long lSeed ;

	for (i= 0; i< NFORMITEMS; i++) {
		pszFormItems[i]= "" ;
	}

	for(i= 0; i< n; i++) {
		fi= findFormItem(pentries[i].name) ;
		if (fi != FORMITEM_NONE) {
			pszFormItems[fi]= pentries[i].val ;
		}
	}

	/*
		here, we look for the "seed" item.  if it's not
		found, we generate a new seed, and put it both
		the "query" and the "formitems" values.
	*/
	if (!(pszFormItems[FORMITEM_SEED][0])) {
		lSeed= (long) time(0) ;
		lSeed&= 0xFFFFL ;		/* so it's not too long */
		sprintf(szSeedText, "%ld", lSeed) ;
		pszFormItems[FORMITEM_SEED]= szSeedText ;
		strcat(pszQuery, "&") ;
		strcat(pszQuery, "seed=") ;
		strcat(pszQuery, szSeedText) ;
	}

	doquery(pszQuery, pszFormItems) ;
#else
	int x ;

	initstdout4html() ;
	printf("<H1>Query Results</H1>") ;
	printf("You submitted the following name/value pairs:<p>\n") ;
	printf("<ul>\n") ;

	for(x= 0; x <= m; x++) {
		printf(
			"<li> <code>%s = %s</code>\n",
			pentries[x].name,
			pentries[x].val
		) ;
	}
	printf("</ul>\n") ;
#endif
}

static void doquery(char *pszQuery, char *pszFormItems[NFORMITEMS])
{
	char szCommandLine[MFMAX_CMDLEN+1] ;
	int src ;
#if 0
	char *pszPath, szPathLine[MFMAX_PATHLEN+1] ;

	strcpy(szPathLine, "PATH=.") ;
	if ((pszPath= getenv("PATH")) != (char *) NULL) {
		strcat(szPathLine, ":") ;
		strcat(szPathLine, pszPath) ;
	}
	putenv(szPathLine) ;
#endif

	sprintf(
		szCommandLine,
#if 0
		"midiscr \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
#else
		"(PATH=.:$PATH; midiscr \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\")",
#endif
		pszQuery,
		pszFormItems[FORMITEM_EXPR1],
		pszFormItems[FORMITEM_EXPR2],
		pszFormItems[FORMITEM_RTYPE],
		pszFormItems[FORMITEM_STYPE],
		pszFormItems[FORMITEM_SEED]
	) ;
	src= system(szCommandLine) ;
	if (src != 0) {
		initstdout4html() ;
		printf("<h1>Error: %d</h1>\n", src) ;
		printf("<h2>Command='%s'</h2>\n", szCommandLine) ;
		printf("<h3>src=%d</h3>\n", src) ;
	}
}

static void initstdout4html()
{
	static int bDoneYet= 0 ;

	if (!bDoneYet) {
		printf("HTTP/1.0 200\r\n") ;
		printf("Content-type: text/html\r\n") ;
		printf("Pragma: no-cache\r\n") ;
		printf("\r\n") ;
		fflush(stdout) ;
		bDoneYet= 1 ;
	}
}

/* end of midifrm.c */
