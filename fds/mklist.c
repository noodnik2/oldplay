
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_ENTRIES 10000

typedef struct {
    char *name;
    char *val;
} entry;

char *makeword(char *line, char stop);
char *fmakeword(FILE *f, char stop, int *len);
char x2c(char *what);
void unescape_url(char *url);
void plustospace(char *str);

static void phdr(char* pszType, char* pszRedir) ;
static void perr(char *pszError, char *pszBack) ;

int main(int argc, char *argv[])
{
	struct stat sb ;
	entry entries[MAX_ENTRIES];
	int x, m, cl, nentries ;
	char *pszLength, *pszMethod, *pszType, *pszReferer ;
	char *pszListname ;
	char szFilename[1024] ;
	char szErrmsg[2048] ;
	FILE* fd ;
    

	/*
		get and check all required CGI environment variables
	*/

	pszMethod= getenv("REQUEST_METHOD") ;
	pszLength= getenv("CONTENT_LENGTH") ;
	pszType= getenv("CONTENT_TYPE") ;
	pszReferer= getenv("HTTP_REFERER") ;

	if (!pszMethod || !pszLength || !pszType || !pszReferer) {
		fprintf(stderr, "ERROR: invalid CGI environment\n") ;
		return(1) ;
	}

	if (strcmp(pszMethod, "POST")) {
		perr("This script should be referenced with a METHOD of POST", pszReferer) ;
		return(2) ;
	}

	if (strcmp(pszType, "application/x-www-form-urlencoded")) {
		perr("This script can only be used to decode form results", pszReferer) ;
        	return(3);
	}
    

	/*
		parse and store returned POST data
	*/

	cl= atoi(pszLength) ;

	for (x= m= 0; cl && !feof(stdin); x++) {
        	m= x ;
        	entries[x].val= fmakeword(stdin, '&', &cl) ;
        	plustospace(entries[x].val) ;
        	unescape_url(entries[x].val) ;
        	entries[x].name= makeword(entries[x].val, '=') ;
	}


	/*
		check returned data for what we need
	*/

	/* ensure list name returned */
	pszListname= 0 ;
	for(x= 0; x<= m; x++) {
		if (strcmp(entries[x].name, "list") == 0) {
			pszListname= entries[x].val ;
			break ;
		}
	}
	if (!pszListname) {
   		perr("No 'list' tag was found in results", pszReferer) ;
		return(4) ;
	}

	/* ensure list name not empty */
	while(isspace(*pszListname)) {
		pszListname++ ;
	}
	if ((*pszListname) == '\0') {
   		perr("Please specify list name", 0) ;
		return(5) ;
	}

	/* must have at least one item in list */
	for(x= 0; x<= m; x++) {
		if (strcmp(entries[x].name, "add") == 0) {
			break ;
		}
	}
	if (x> m) {
   		perr("Please select one or more items", 0) ;
		return(6) ;
	}


	/*
		create output filename
		check that it doesn't yet exist
		check that we can open it for writing
	*/

	sprintf(szFilename, "%s.list", pszListname) ;

	if (stat(szFilename, &sb) == 0) {
		sprintf(szErrmsg, "Use another name; '%s' already exists", szFilename) ;
		perr(szErrmsg, 0) ;
		return(7) ;
	}

	fd= fopen(szFilename, "w") ;
	if (!fd) {
		sprintf(szErrmsg, "Cannot open '%s' for writing", szFilename) ;
		perr(szErrmsg, 0) ;
		return(8) ;
	}


	/*
		write new entries to new list file
	*/

	nentries= 0 ;
	for(x= 0; x<= m; x++) {
		if (strcmp(entries[x].name, "add") == 0) {
			fprintf(fd, "%s\n", entries[x].val) ;
			nentries++ ;
		}
	}
	fclose(fd) ;


	/*
		inform user of results
	*/

	phdr("text/html", 0) ;
	printf("<h2>'%s' Created (%d entries)</h2>\n", szFilename, nentries) ;
	printf("<p>\n") ;
	printf("<h3><a href=\"%s\">Back</a></h3>\n", pszReferer) ;

	return(0) ;
}



/*
	internal functions
*/

static void phdr(char* pszType, char* pszRedir)
{
	if (pszType) {
		printf("Content-type: %s\n", pszType) ;
	}
	if (pszRedir) {
		printf("Location: %s\n", pszRedir) ;
	}
	printf("\n") ;
}

static void perr(char *pszError, char *pszBack)
{
	phdr("text/html", 0) ;

	if (pszError) {
		printf("<h2>Error: %s</h2>\n", pszError) ;
	}
	else {
		printf("<h2>Error</h2>\n") ;
	}

	if (pszBack) {
		printf("<h3><a href=\"%s\">Back</a></h3>\n", pszBack) ;
	}
	else {
		printf("<h3>Press browser's &lt;BACK&gt; button to go back</h3>.\n") ;
	}
}


/* end of file */
