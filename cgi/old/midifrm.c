#define	MSRC_Revision		9
#define	FOR_REAL		1

/*
	midifrm.c
*/

/*
	#includes
*/

#include	<stdio.h>
#ifndef NO_STDLIB_H
#include	<stdlib.h>
#else
char *getenv() ;
#endif


/*
	#defines
*/

#define	MAX_QUERY	4096
#define	MAX_ENTRIES	1000


/*
	main program
*/

typedef struct {
	char name[64] ;
	char val[128] ;
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
	char szQueryWork[MAX_QUERY] ;
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

	process_form(pszQuery, entries, x) ;
	exit(0) ;
}

#define	MFMAX_EXPRLEN		4096

#define	FORM_ITEM		int
#define	FORMITEM_NONE		-1
#define	FORMITEM_EXPR1		0
#define	FORMITEM_EXPR2		1
#define	FORMITEM_RTYPE		2
#define	FORMITEM_STYPE		3
#define	FORMITEM_XTYPE		4
#define	NFORMITEMS		5

static char *pszFormItemNames[NFORMITEMS]= {
	"expr1",
	"expr2",
	"rtype",
	"stype",
	"xtype",
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

static void process_form(char *pszQuery, entry *pentries, int n)
{
#if	FOR_REAL
	int i ;
	FORM_ITEM fi ;
	char *pszFormItems[NFORMITEMS] ;

	for (i= 0; i< NFORMITEMS; i++) {
		pszFormItems[i]= "" ;
	}

	for(i= 0; i< n; i++) {
		fi= findFormItem(pentries[i].name) ;
		if (fi != FORMITEM_NONE) {
			pszFormItems[fi]= pentries[i].val ;
		}
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

#define	MFMAX_CMDLEN		(MFMAX_EXPRLEN + 128)

static void doquery(char *pszQuery, char *pszFormItems[NFORMITEMS])
{
	char szCommandLine[MFMAX_CMDLEN+1] ;
	int src ;

	sprintf(
		szCommandLine,
		"midiscr \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
		pszQuery,
		pszFormItems[FORMITEM_EXPR1],
		pszFormItems[FORMITEM_EXPR2],
		pszFormItems[FORMITEM_RTYPE],
		pszFormItems[FORMITEM_STYPE],
		pszFormItems[FORMITEM_XTYPE]
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
		printf("Content-type: text/html\n\n") ;
		fflush(stdout) ;
		bDoneYet= 1 ;
	}
}

/* end of midifrm.c */
