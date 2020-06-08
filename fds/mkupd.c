
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
} entry ;

typedef struct {
	char* pszMethod ;
	char* pszReferer ;
	char* pszLength ;
	char* pszType ;
	char* pszQuery ;
	char* pszScript ;
} cgienv ;

typedef struct {
	char szAlt[4096] ;
	char szDesc[10240] ;
} data ;

char *makeword(char *line, char stop);
char *fmakeword(FILE *f, char stop, int *len);
void unescape_url(char *url);
void plustospace(char *str);

static int pbadenv(char*) ;
static int phdr(char* pszType, char* pszRedir) ;
static int perr(char *pszError, char *pszBack) ;
static int doget(cgienv*) ;
static int dopost(cgienv*) ;
static int getstring(char* pszName, char* pszExt, char* pszString, int maxlen) ;
static int getdata(char* pszName, data* pdata) ;
static int putstring(char* pszName, char* pszExt, char* pszString) ;
static int putdata(char* pszName, data* pdata) ;
static int mkfn(char* pszFilename, char* pszName, char* pszExt) ;


/*
*/

int main(int argc, char *argv[])
{
	cgienv env ;
	char szErrmsg[2048] ;

	memset(&env, '\0', sizeof(cgienv)) ;
	env.pszMethod= getenv("REQUEST_METHOD") ;
	env.pszReferer= getenv("HTTP_REFERER") ;
	env.pszType= getenv("CONTENT_TYPE") ;
	env.pszLength= getenv("CONTENT_LENGTH") ;
	env.pszQuery= getenv("QUERY_STRING") ;
	env.pszScript= getenv("SCRIPT_NAME") ;

	if (!env.pszMethod) return(pbadenv("no method")) ;
	if (!env.pszScript) return(pbadenv("no script name")) ;

	if (strcmp(env.pszMethod, "GET") == 0) return(doget(&env)) ;
	if (strcmp(env.pszMethod, "POST") == 0) return(dopost(&env)) ;

	sprintf(szErrmsg, "Bad REQUEST_METHOD: \"%s\"", env.pszMethod) ;
	return(perr(szErrmsg, env.pszReferer)) ;
}


/*
	internal functions
*/

static int doget(cgienv* penv)
{
	data d ;

	if (!penv->pszQuery) return(pbadenv("query string missing")) ;
	if (!penv->pszScript) return(pbadenv("script name missing")) ;

	if (!getdata(penv->pszQuery, &d)) {
		return(perr("can't get data", 0)) ;
	}

	phdr("text/html", 0) ;

	printf("<html>\n") ;
	printf("<head>\n") ;
	printf("<title>%s</title>\n", penv->pszQuery) ;
	printf("</head>\n") ;
	printf("<body>\n") ;

	printf("<form method=\"POST\" action=\"%s\">\n", penv->pszScript) ;

	printf("<center><table>\n") ;

	printf("<input name=\"name\" type=\"hidden\" value=\"%s\">\n", penv->pszQuery) ;
	printf("<input name=\"back\" type=\"hidden\" value=\"%s\">\n", penv->pszReferer) ;

	printf("<tr><td align=center valign=middle>\n") ;
	printf("<input name=\"alt\" type=\"text\" value=\"%s\" size=\"80\"><br>\n", d.szAlt) ;
	printf("</td></tr>\n") ;

	printf("<tr><td align=center valign=middle>\n") ;
	printf("<img src=\"images/%s-m.jpg\">\n", penv->pszQuery) ;
	printf("</td></tr>\n") ;

	printf("<tr><td align=center valign=middle>\n") ;
	printf("<textarea name=\"desc\" rows=\"2\" cols=\"80\">%s</textarea><br>\n", d.szDesc) ;
	printf("</td></tr>\n") ;

	printf("<tr><td align=center valign=middle>\n") ;
	printf("<input type=\"submit\" value=\"Update Info\" align=\"left\">\n") ;
	printf("<input type=\"reset\" value=\"Clear Form\" align=\"right\">\n") ;
	printf("</td></tr>\n") ;

	printf("</table></center>\n") ;

	printf("</form>\n") ;

	printf("</body>\n") ;
	printf("</html>\n") ;

	return(0) ;
}

static int dopost(cgienv* penv)
{
	data d ;
	entry entries[MAX_ENTRIES];
	int x, m, cl ;
	char *pszBack, *pszName, *pszAlt, *pszDesc ;

	/*
		get and check all required CGI environment variables
	*/


	if (!penv->pszLength) return(pbadenv("length missing")) ;
	if (!penv->pszType) return(pbadenv("type missing")) ;

	if (strcmp(penv->pszType, "application/x-www-form-urlencoded")) {
		return(perr("Invalid POST method content type", penv->pszReferer)) ;
	}
    

	/*
		parse and store returned POST data
	*/

	cl= atoi(penv->pszLength) ;

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

	pszBack= 0 ;
	pszName= 0 ;
	pszAlt= 0 ;
	pszDesc= 0 ;
	for(x= 0; x<= m; x++) {
		if (strcmp(entries[x].name, "back") == 0) {
			pszBack= entries[x].val ;
		}
		if (strcmp(entries[x].name, "name") == 0) {
			pszName= entries[x].val ;
		}
		if (strcmp(entries[x].name, "desc") == 0) {
			pszDesc= entries[x].val ;
		}
		if (strcmp(entries[x].name, "alt") == 0) {
			pszAlt= entries[x].val ;
		}
	}
	if (!pszName) {
		return(perr("no 'name' returned in form", 0)) ;
	}
	if (pszAlt) {
		while(isspace(*pszAlt)) {
			pszAlt++ ;
		}
	}
	if (pszDesc) {
		while(isspace(*pszDesc)) {
			pszDesc++ ;
		}
	}


	/*
		update data
	*/

	memset(&d, '\0', sizeof(data)) ;
	if (pszAlt) strncpy(d.szAlt, pszAlt, sizeof(d.szAlt)) ;
	if (pszDesc) strncpy(d.szDesc, pszDesc, sizeof(d.szDesc)) ;

	if (!putdata(pszName, &d)) {
		return(perr("can't update data", 0)) ;
	}

	/*
		if everything went OK, return user to referrer
		of original GET request...
	*/

#if 0
	phdr("text/html", pszBack) ;

	/*
		Most browsers will honor the HTTP "Location:" tag and
		return the user to the "pszBack" page (e.g., the user
		will not see anything that's written at this point).

		However, if the browser doesn't do this, we write a
		nice message that the user will see...
	*/
#else
	phdr("text/html", 0) ;
#endif

	printf("<h1>Update succeeded</h1>\n") ;
	printf("<p>\n") ;
	printf("(Use the &lt;BACK&gt; button on your browser to return)\n") ;

	return(0) ;
}



/*
	internal functions
*/


static int pbadenv(char* pszDetails)
{
	phdr(0, 0) ;
	printf("ERROR: not executed in proper CGI environment\n") ;
	if (pszDetails) printf("(%s)\n", pszDetails) ;
	return(1) ;
}

static int phdr(char* pszType, char* pszRedir)
{
	if (pszType) {
		printf("Content-type: %s\n", pszType) ;
	}
	if (pszRedir) {
		printf("Location: %s\n", pszRedir) ;
	}
	printf("\n") ;
	return(0) ;
}

static int perr(char *pszError, char *pszBack)
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
	return(2) ;
}

static int getdata(char* pszName, data* pdata)
{
	getstring(pszName, "alt", pdata->szAlt, sizeof(pdata->szAlt)) ;
	getstring(pszName, "desc", pdata->szDesc, sizeof(pdata->szDesc)) ;
	return(1) ;
}

static int getstring(char* pszName, char* pszExt, char* pszString, int maxlen)
{
	FILE* fp ;
	char szFilename[1024] ;
	char *p ;
	size_t sRead ;

	if (!mkfn(szFilename, pszName, pszExt)) {
		return(0) ;
	}

	pszString[0]= '\0' ;
	fp= fopen(szFilename, "r") ;
	if (!fp) return(1) ;

	sRead= fread(pszString, maxlen, 1, fp) ;

	fclose(fp) ;

	/* we take the liberty of switching all quotes */
	for (p= pszString; *p; p++) {
		if ((*p) == '"') {
			(*p)= '\'' ;
		}
	}

	return((sRead> 0)? 1: 0) ;
}

static int putdata(char* pszName, data* pdata)
{
	putstring(pszName, "alt", pdata->szAlt) ;
	putstring(pszName, "desc", pdata->szDesc) ;
	return(1) ;
}

static int putstring(char* pszName, char* pszExt, char* pszString)
{
	FILE* fp ;
	char szFilename[1024] ;
	size_t sWriteReq, sWrite ;

	if (!mkfn(szFilename, pszName, pszExt)) {
		return(0) ;
	}

	if (!pszString || !pszString[0]) {
		unlink(szFilename) ;
		return(1) ;
	}

	fp= fopen(szFilename, "w") ;
	if (!fp) return(0) ;

	sWriteReq= (size_t) strlen(pszString) ;
	sWrite= fwrite(pszString, sWriteReq, 1, fp) ;
	fwrite("\n", 1, 1, fp) ;

	fclose(fp) ;

	return((sWrite == sWriteReq)? 1: 0) ;
}

static int mkfn(char* pszFilename, char* pszName, char* pszExt)
{
	sprintf(pszFilename, "%s.%s", pszName, pszExt) ;
	return(1) ;
}

/* end of file */
