/*

	chk_rcf.c			Reads the Checkout File


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

extern co_parms cops ;

extern char *malloc() ;
extern char *chk_recltrim() ;
extern char *chk_recrtrim() ;

static void pcf_err() ;
static void getword() ;

void chk_rcf_fn(infn, outfn)
char *infn, *outfn ;
{
	chk_fn(infn, outfn) ;
}

FILE *chk_rcf_open(fn)
char *fn ;
{
	FILE *fp ;

	if ((fp= fopen(fn, "r")) == (FILE *)NULL) {
		return((FILE *)NULL) ;
	}
	return(fp) ;
}

int chk_rcf_copyhdr(ifp, ofp, cfnep)
FILE *ifp, *ofp ;
int *cfnep ;
{
	char cfbuf[CF_RECSIZE+1] ;
	char cfid[CF_IDSIZE] ;

	for (;;) {
		if (fgets(cfbuf, sizeof(cfbuf), ifp) == (char *)NULL) {
			pcf_err("can't find ID value in") ;
			return(1) ;
		}
		if (ofp != (FILE *)NULL) {
			if (fputs(cfbuf, ofp) == EOF) {
				pcf_err("EOF error writing new checkout file");
				return(1) ;
			}
		}
		chk_frecstrip(cfbuf) ;
		if (idparse(cfbuf, cfid, cfnep)) break ;
	}
	if (strcmp(cfid, cops.chkid) != 0) {
		pcf_err("wrong ID value in") ;
		return(1) ;
	}
	if (((*cfnep)< 0) || ((*cfnep)> CFNE_MAX)) {
		pcf_err("bad ID # in") ;
		return(1) ;
	}
	return(0) ;
}

co_clist *chk_rcf(fn)
char *fn ;
{
	FILE *ifp ;
	char *clbufp ;
	char **dnep ;
	co_clist *clp, *clp0 ;
	co_clist_entry *clep ;
	char cfbuf[CF_RECSIZE] ;
	int i, cfne, cflsize ;

	clp0= (co_clist *)NULL ;
	if ((ifp= chk_rcf_open(fn)) == (FILE *)NULL) {
		pcf_err("can't open") ;
		return(clp0) ;
	}

	if (chk_rcf_copyhdr(ifp, (FILE *)NULL, &cfne)) {
		fclose(ifp) ;
		return(clp0) ;
	}

	cflsize= sizeof(clp[0])
		+ (sizeof(clp->clep[0]) * cfne)
		+ (sizeof(clp->dnlist[0]) * cfne) ;

	if (cops.debug) {
		printf("checkout file takes %d bytes\n", cflsize) ;
	}
	
	if ((clbufp= malloc(cflsize)) == (char *)NULL) {
		fclose(ifp) ;
		pcf_err("can't get memory for") ;
		return(clp0) ;
	}
	memset(clbufp, '\0', cflsize) ;

	clp= (co_clist *)clbufp ;
	clep= (co_clist_entry *)(clp + 1) ;
	dnep= (char **)(clep + cfne) ;
	clp->maxentries= cfne ;
	clp->dnlist= dnep ;
	clp->clep= clep ;

	for (i= 0;;) {
		if (fgets(cfbuf, sizeof(cfbuf), ifp) == (char *)NULL) {
			break ;
		}
		chk_frecstrip(cfbuf) ;		/* strip off \n */
		chk_recrtrim(cfbuf) ;		/* strip right whitespace */
		if (*chk_recltrim(cfbuf)=='\0') {/* strip left whitespace */
			continue ;
		}
		if (i == cfne) {
			fclose(ifp) ;
			pcf_err("overflow in") ;
			return(clp0) ;
		}
		if (chkparse(cfbuf, clep)) {
			fclose(ifp) ;
			pcf_err("invalid line in") ;
			return(clp0) ;
		}
		*dnep= clep->dname ;
		dnep++ ;
		clep++ ;
		i++ ;
	}

	clp->nentries= i ;

	fclose(ifp) ;
	return(clp) ;
}

static int idparse(rec, idp, nep)
char *rec ;
char *idp ;
int *nep ;
{
	char workrec[CF_RECSIZE] ;
	char word[CF_RECSIZE] ;
	int ne ;

	strcpy(workrec, rec) ;

	getword(word, workrec) ;		/* get device name */

	if (word[0]==';' || word[0]=='\0') {	/* skip comments */
		return(0) ;
	}

	idp[0]= '\0' ;				/* set error values */
	(*nep)= (-1) ;

	if (strcmp(word, "id") == 0) {
		getword(word, workrec) ;
		if (strlen(word)< CF_IDSIZE) {
			strcpy(idp, word) ;
			getword(word, workrec) ;
			if (sscanf(word, "%d", &ne) == 1) {
				getword(word, workrec) ;
				if (word[0] != '\0') {
					pcf_err("junk past ID # in") ;	
				}
				else {
					(*nep)= ne ;
				}
			}
		}
	}
	return(1) ;
}

/*

	(int)rc= chkparse(rec, clep) ;

	Parses a record in 'rec' in chkout-file format.
	Returns fields in 'clep'.  

	Current implementation:

		Fills in clep->to_uid with uid of current owner of
		device (this value is currently NOT in the file).

		Also fills in clep->de with CURRENT status value of
		device (again, not currently kept in the chkout file).

*/
static int chkparse(rec, clep)
char *rec ;
co_clist_entry *clep ;
{
	char workrec[CF_RECSIZE] ;
	char word[CF_RECSIZE] ;

	strcpy(workrec, rec) ;

	getword(word, workrec) ;		/* get device name */
	strncpy(clep->dname, word, sizeof(clep->dname)) ;

	getword(word, workrec) ;		/* get user id */
	strncpy(clep->uname, word, sizeof(clep->uname)) ;

						/* get timestamp */
	strncpy(clep->timeout, chk_recltrim(workrec), sizeof(clep->timeout)) ;

						/* rest is junk */

	if (clep->timeout[0] == '\0') return(1) ;

	if (chk_dev_getstat(clep->dname, &clep->de)) {
		clep->dnf= 1 ;			/* device not found */
		clep->to_uid= cops.root_uid ;
	}
	else {
		clep->dnf= 0 ;			/* defice is found */
		clep->to_uid= clep->de.uid ;
	}
	return(0) ;
}

static void pcf_err(msg)
char *msg ;
{
	char emsgbuf[256] ;

	sprintf(emsgbuf, "%s checkout file", msg) ;
	chk_err(emsgbuf) ;
}

static void getword(word, string)
{
	chk_recword(word, string, (char *)NULL) ;
}
