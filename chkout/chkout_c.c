/*

	NOTE:		THIS ROUTINE BBAADDLLYY NEEDS TO BE RE-ORGANIZED!!!
			SEE chkout_r.c AS WELL, COMBINE, ETC...

	chkout_c.c				Check Out a Resource

	Logic:
		Verify arguments.
		Verify <dev>s don't exist in the checked-out list.
		Verify <dev>s exist in the registered resource list.
		Verify user has rights to checkout device.
		Verify that the current values match default values.
		Update the file.

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"
#include	"chk_pw.h"

extern co_parms cops ;
extern co_updf couf ;

extern co_rlist *chk_rrf() ;
extern co_clist *chk_rcf() ;
extern char *chk_recltrim() ;
extern FILE *chk_rcf_open() ;
extern char *malloc() ;

int chkout_c(cocsp)
co_cmd *cocsp ;
{
	co_clist *clp ;
	co_rlist *rlp ;
	co_rlist_entry *rlep ;
	co_deve de, to_de, *newdep, *olddep ;
	char **dnep ;
	int rc, ri, i, n ;


						/* check arguments */
	if ((cocsp->ndevs) == 0) {
		chk_err("arguments required for 'chkout' option") ;
		return(CHK_ERR_BADARG) ;
	}


						/* read files */
	if ((clp= chk_rcf(couf.chkfn)) == (co_clist *)NULL) {
		return(CHK_ERR+3) ;
	}

	if ((rlp= chk_rrf(couf.regfn)) == (co_rlist *)NULL) {
		return(CHK_ERR+3) ;
	}

						/* allocate status arrays */
	i= (2 * sizeof(co_deve) * (cocsp->ndevs)) ;
	if ((newdep= (co_deve *)malloc(i)) == (co_deve *)NULL) {
		chk_err("can't allocate status arrays") ;
		return(CHK_ERR+4) ;
	}
	olddep= newdep + 1 ;

	to_de.uid= cocsp->for_uid ;		/* assign "to status" value */
	to_de.gid= cocsp->for_gid ;
	to_de.mode= cocsp->mode ;

						/* verify not checked out */
	n= (cocsp->ndevs) ;
	dnep= cocsp->dnlist ;
	for (i= 0; i< n; i++) {
		if (chk_list_flname(*dnep, clp->dnlist, clp->nentries)>= 0) {
			chk_dev_err(*dnep, "busy") ;
			return(1) ;
		}
		dnep++ ;
	}
						/* verify registered */
	n= (cocsp->ndevs) ;
	dnep= cocsp->dnlist ;
	for (i= 0; i< n; i++) {
		if ((ri= chk_list_flname(
				*dnep, rlp->dnlist, rlp->nentries))< 0) {
			chk_dev_err(*dnep, "not registered") ;
			return(1) ;
		}
		rlep= rlp->rlep + ri ;
		memcpy((char *)&olddep[i], (char *)&rlep->de, sizeof(co_deve)) ;
		if ((rc= chk_dev_inuse(rlp->rlep + ri, &de))< 0) {
			chk_dev_err(*dnep, "can't find") ;
			return(4) ;
		}
		if (rc != 0) {
			chk_dev_rptstat(*dnep, &de, "in use") ;
			return(3) ;
		}
		if (notauth(rlep, cocsp)) {
			chk_dev_err(*dnep, "not authorized") ;
			return(2) ;
		}
		memcpy((char *)&newdep[i], (char *)&to_de, sizeof(co_deve)) ;
		dnep++ ;
	}

	return(upd_chkfile(cocsp, clp, newdep, olddep)) ;
}

/*
	upd_chkfile(cocsp, clp, newdep, olddep) ;	Update the file

	Logic:
		(trap interrupts: vector to restore routine)
		Lock checked-out list.
		Change <dev>s
		Update checked-out list.
		Unlock checked-out list.
	
	Restore:
		Change <dev>s to default values.
		Restore original checked-out list.
		Unlock checked-out list.

*/

static void rst_chkfile() ;
#define		LCL_RST		(-1)

static int upd_chkfile(cocsp, clp, newdep, olddep)
co_cmd *cocsp ;
co_clist *clp ;
co_deve *newdep, *olddep ;
{
	FILE *ifp, *ofp ;
	co_clist_entry *clep ;
	char **dnep ;
	int i, n ;

	couf.rst_dnlist= (cocsp->dnlist) ;
	couf.rst_dnlist_len= (cocsp->ndevs) ;
	couf.rst_deplist= olddep ;

	chk_trap(rst_chkfile) ;

					/* Lock checked-out list */
	if (chk_lock_mv(couf.chkfn, couf.lock_chkfn)) {
		chk_err("can't lock chkout file") ;
		return(CHK_ERR+10) ;
	}

					/* Change <devs> */
	if (chk_stat_upd(cocsp->dnlist, cocsp->ndevs, newdep)) {
		rst_chkfile(LCL_RST) ;
		chk_err("can't change resource status") ;
		return(CHK_ERR+11) ;
	}

					/* Update checked out list */
	if ((ifp= chk_rcf_open(couf.lock_chkfn)) == (FILE *)NULL) {
		rst_chkfile(LCL_RST) ;
		chk_err("can't open checkout file for update") ;
		return(CHK_ERR+12) ;
	}

	if ((ofp= fopen(couf.temp_chkfn, "w")) == (FILE *)NULL) {
		fclose(ifp) ;
		rst_chkfile(LCL_RST) ;
		chk_err("can't open temp file") ;
		return(CHK_ERR+13) ;
	}

	if (chk_rcf_copyhdr(ifp, ofp, &n)) {
		fclose(ifp) ;
		fclose(ofp) ;
		rst_chkfile(LCL_RST) ;
		chk_err("can't copy checkout file header") ;
		return(CHK_ERR+14) ;
	}

	n= (clp->nentries) ;
	for (i= 0, clep= clp->clep; i< n; i++, clep++) {
		fprintf(
			ofp,
			"%s %s %s\n",
			clep->dname,
			clep->uname,
			clep->timeout
		) ;
	}

	n= (cocsp->ndevs) ;
	for (i= 0, dnep= cocsp->dnlist; i< n; i++, dnep++) {
		fprintf(
			ofp,
			"%s %s %s\n",
			*dnep,
			cocsp->by_uname,
			cocsp->cmd_time
		) ;
	}
	fclose(ofp) ;
	fclose(ifp) ;

						/* Unlock checked-out list */
	chk_lock_mv(couf.temp_chkfn, couf.chkfn) ;
	chk_lock_rm(couf.lock_chkfn) ;

	chk_trap(0) ;
	return(0) ;
}

static void rst_chkfile(sig)
int sig ;
{
	chk_lock_mv(couf.lock_chkfn, couf.chkfn) ;
	chk_stat_upd(couf.rst_dnlist, couf.rst_dnlist_len, couf.rst_deplist) ;
	unlink(couf.temp_chkfn) ;
}

/*

	(int)flag= notauth(rlep, cocsp) ;		Chkout NOT Authorized?


	Returns nonzero iff by_user is NOT authorized to checkout the
	device specified by '*rlep'.

	If there is no group list associated with the "*rlep" device
	entry (from the resource file), then any user is authorized
	to check out the device.

	Else, each entry in the group list is checked against the
	"by_gname" value for the command.  If this fails, the author-
	ization list '*gap' is scanned for each group list entry.  If
	this fails, then the by_gid is checked against the "root_uid".

	If all of the above tests fail, the user is not authorized
	and a nonzero value is returned.

*/
static int notauth(rlep, cocsp)
co_rlist_entry *rlep ;
co_cmd *cocsp ;
{
	register int i ;
	int n ;
	char *glp ;
	char gname[GNAME_SIZE+1] ;
	char glist[sizeof(rlep->glist)] ;
	cpwgauth ga, *gap ;

	/*
		Is there a group list?
		If no, then the user is authorized.
	*/
	if ( (*rlep->glist) == '\0') {
		return(0) ;				/* no: authorized */
	}

	/*
		No, so we must scan the group list.
	*/
	memcpy(glp= glist, rlep->glist, sizeof(glist)) ;
	for (;;) {

		chk_recword(gname, glist, ",") ;
		chk_recrtrim(gname) ;
		glp= chk_recltrim(gname) ;
		if (*glp == '\0') break ;

	/*
		First we ask: is the by_gname listed in the group list?
	*/
		if (strcmp(glp, cocsp->by_gname) == 0) {
			return(0) ;			/* yes: authorized */
		}

	/*
		No, so then we ask: is the user on the authorization list
		for this group from the group list?
	*/
		gap= chk_pw_gauthbygname(&ga, glp) ;
		if (gap == (cpwgauth *)NULL) {
			printf("gr='%s'\n", glp) ;
			chk_err("can't find group authorization") ;
			return(1) ;
		}
		n= gap->nauths ;
		for (i= 0; i< n; i++) {
			if (strcmp(
				cocsp->by_uname,
				gap->uname[i]
			) == 0) {
				return(0) ;		/* authorized */
			}
		}

	/*
		No, try the next entry in the group list, if any.
	*/
	}

	/*
		Not authorized via group list;
		we ask: is this ths user "ROOT"?
	*/
	if ((cocsp->by_gid) == cops.root_uid) {
		return(0) ;				/* authorized */
	} 

	/*
		No, so we fail the user.
	*/
	return(1) ;					/* not authorized */
}
