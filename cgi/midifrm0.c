#define	MSRC_Revision		4
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

#define	MAX_ENTRIES	10000


/*
	main program
*/

typedef struct {
    char *name ;
    char *val ;
} entry ;


/*
	templates
*/

char *makeword(char *line, char stop) ;
char *fmakeword(FILE *f, char stop, int *len) ;
char x2c(char *what) ;
void unescape_url(char *url) ;
void plustospace(char *str) ;

/*
	internal templates
*/
static void process_form(entry *pentries, int m) ;
static void doquery(char *pszExpression) ;

/*
	main program
*/

void main(int argc, char *argv[])
{
	entry entries[MAX_ENTRIES] ;
	register int x, m= 0 ;
	int cl ;

	printf("Content-type: text/html\n\n") ;
	fflush(stdout) ;

 	if (strcmp(getenv("REQUEST_METHOD"),"POST")) {
        	printf("This script should be referenced with a METHOD of POST.\n") ;
		printf("If you don't understand this, see this ") ;
		printf("<A HREF=\"http://www.ncsa.uiuc.edu/SDG/Software/Mosaic/Docs/fill-out-forms/overview.html\">forms overview</A>.%c",10) ;
        	exit(1) ;
	}
	if (strcmp(
		getenv("CONTENT_TYPE"),
		"application/x-www-form-urlencoded"
	) != 0) {
		printf("This script can only be used to decode form results.\n");
		exit(1) ;
	}
	cl= atoi(getenv("CONTENT_LENGTH")) ;

	for (x= 0; cl && (!feof(stdin)); x++) {
		m= x ;
		entries[x].val= fmakeword(stdin, '&', &cl) ;
		plustospace(entries[x].val) ;
		unescape_url(entries[x].val) ;
		entries[x].name= makeword(entries[x].val, '=') ;
	}

	process_form(entries, m) ;
	exit(0) ;
}

#define	MFMAX_EXPRLEN		4096

static void process_form(entry *pentries, int m)
{
#if	FOR_REAL
		char szExpression[MFMAX_EXPRLEN+1] ;
	int i ;

	for(i= 0; i<= m; i++) {
		if (strcmp(pentries[i].name, "expr1") == 0) {
			doquery(pentries[i].val) ;
			return ;
		}
	}
	printf("<h1>Sorry, this script needs to be updated</h1>\n") ;
#else
	int x ;

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

static void doquery(char *pszExpression)
{
	char szCommandLine[MFMAX_CMDLEN+1] ;
	int src ;

	sprintf(
		szCommandLine,
		"mklist -q \"%s\" 2>&1 | mhl"
		" \"<h1>MIDI Search Results: <i>'%s'</i></h1>\"",
		pszExpression,
		pszExpression
	) ;
	src= system(szCommandLine) ;
	if (src != 0) {
		printf("<h1>Error: %d</h1>\n", src) ;
		printf("<h2>Command='%s'</h2>\n", szCommandLine) ;
		printf("<h3>src=%d</h3>\n", src) ;
	}
}

/* end of midifrm.c */
