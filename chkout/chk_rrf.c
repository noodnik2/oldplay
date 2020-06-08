/*

	chk_rrf.c			Reads the Registration File


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"
#include	"chk_pw.h"

extern co_parms cops ;

extern char *malloc() ;
extern char *chk_recltrim() ;
extern char *chk_recrtrim() ;
static void prf_err() ;
static void regperr_cantget() ;
static void getword() ;

co_rlist *chk_rrf(fn)
char *fn ;
{
	FILE *ifp ;
	char *rlbufp ;
	char **dnep ;
	co_rlist *rlp, *rlp0 ;
	co_rlist_entry *rlep ;
	char rfbuf[RF_RECSIZE] ;
	char rfid[RF_IDSIZE] ;
	int i, rfne, rflsize ;

	rlp0= (co_rlist *)NULL ;
	if ((ifp= fopen(fn, "r")) == (FILE *)NULL) {
		printf("fn='%s'\n", fn ) ;
		prf_err("can't open") ;
		return(rlp0) ;
	}

	for (;;) {
		if (fgets(rfbuf, sizeof(rfbuf), ifp) == (char *)NULL) {
			fclose(ifp) ;
			prf_err("can't find ID value in") ;
			return(rlp0) ;
		}
		chk_frecstrip(rfbuf) ;
		if (idparse(rfbuf, rfid, &rfne)) break ;
	}

	if (strcmp(rfid, cops.regid) != 0) {
		fclose(ifp) ;
		prf_err("wrong ID value in") ;
		return(rlp0) ;
	}

	if ((rfne< 0) || (rfne> RFNE_MAX)) {
		fclose(ifp) ;
		prf_err("bad ID # in") ;
		return(rlp0) ;
	}

	rflsize= sizeof(rlp[0])
		+ (sizeof(rlp->rlep[0]) * rfne)
		+ (sizeof(rlp->dnlist[0]) * rfne) ;
	
	if (cops.debug) {
		printf("resource file takes %d bytes\n", rflsize) ;
	}

	if ((rlbufp= malloc(rflsize)) == (char *)NULL) {
		fclose(ifp) ;
		prf_err("can't get memory for") ;
		return(rlp0) ;
	}

	rlp= (co_rlist *)rlbufp ;
	rlep= (co_rlist_entry *)(rlp + 1) ;
	dnep= (char **)(rlep + rfne) ;
	rlp->maxentries= rfne ;
	rlp->dnlist= dnep ;
	rlp->rlep= rlep ;

	for (i= 0;;) {
		if (fgets(rfbuf, sizeof(rfbuf), ifp) == (char *)NULL) {
			break ;
		}
		chk_frecstrip(rfbuf) ;		/* strip off \n */
		chk_recrtrim(rfbuf) ;		/* strip right whitespace */
		if (*chk_recltrim(rfbuf)=='\0') {/* strip left whitespace */
			continue ;
		}
		if (i == rfne) {
			fclose(ifp) ;
			prf_err("overflow in") ;
			return(rlp0) ;
		}
		if (regparse(rfbuf, rlep)) {
			fclose(ifp) ;
			prf_err("invalid line in") ;
			return(rlp0) ;
		}
		*dnep= rlep->dname ;
		dnep++ ;
		rlep++ ;
		i++ ;
	}

	rlp->nentries= i ;

	fclose(ifp) ;
	return(rlp) ;
}

static int idparse(rec, idp, nep)
char *rec ;
char *idp ;
int *nep ;
{
	char workrec[RF_RECSIZE] ;
	char word[RF_RECSIZE] ;
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
		if (strlen(word)< RF_IDSIZE) {
			strcpy(idp, word) ;
			getword(word, workrec) ;
			if (sscanf(word, "%d", &ne) == 1) {
				getword(word, workrec) ;
				if (word[0] != '\0') {
					prf_err("junk past ID # in") ;	
				}
				else {
					(*nep)= ne ;
				}
			}
		}
	}
	return(1) ;
}

static int regparse(rec, rlep)
char *rec ;
co_rlist_entry *rlep ;
{
	char workrec[RF_RECSIZE] ;
	char word[RF_RECSIZE] ;
	cpwpent pe ;
	cpwgent ge ;
	unsigned mode ;

	strcpy(workrec, rec) ;

	getword(word, workrec) ;		/* get device name */
	strncpy(rlep->dname, word, sizeof(rlep->dname)) ;

	getword(word, workrec) ;		/* get user id */
	strncpy(rlep->uname, word, sizeof(rlep->uname)) ;

	getword(word, workrec) ;		/* get group id */
	strncpy(rlep->gname, word, sizeof(rlep->gname)) ;

	getword(word, workrec) ;		/* get mode spec */
	strncpy(rlep->mspec, word, sizeof(rlep->mspec)) ;

						/* rest is glist */
	strncpy(rlep->glist, chk_recltrim(workrec), sizeof(rlep->glist)) ;

	if (rlep->mspec[0] == '\0') return(1) ;

	if (chk_pw_pebyuname(rlep->uname, &pe) == (cpwpent *)NULL) {
		regperr_cantget("user id", rlep->uname) ;
		return(1) ;
	}
		
	if (chk_pw_gebygname(rlep->gname, &ge) == (cpwgent *)NULL) {
		regperr_cantget("group id", rlep->gname) ;
		return(1) ;
	}

	if (sscanf(rlep->mspec, "%o", &mode) != 1) {
		regperr_cantget("mode", rlep->mspec) ;
		return(1) ;
	}

	rlep->de.uid= pe.uid ;
	rlep->de.gid= ge.gid ;
	rlep->de.mode= mode ;
		
	return(0) ;
}

static void regperr_cantget(what, for_item)
char *what, *for_item ;
{
	char emsg[256] ;

	sprintf(emsg, "can't get %s for '%s'", what, for_item) ;
	prf_err(emsg) ;
}

static void prf_err(msg)
char *msg ;
{
	char emsg[256] ;

	sprintf(emsg, "%s resource file", msg) ;
	chk_err(emsg) ;
}

static void getword(word, string)
{
	chk_recword(word, string, (char *)NULL) ;
}
