/*


	NOTE:		THIS ROUTINE BBAADDLLYY NEEDS TO BE RE-ORGANIZED!!!
			SEE chkout_c.c AS WELL, COMBINE, ETC...


	chkout_r.c				Return a Resource

	Logic:
		Verify arguments.
		Verify <dev>s exist in the checked-out list.
		Verify that the current values match checkout values.
		Verify user's authority to return <dev>s.
		(checkout values now==values at time of read of chkout file)
		(trap interrupts: vector to restore routine)
		Lock checked-out list.
		Change <dev>s to default values.
		Update checked-out list.
		Unlock checked-out list.
	
	Restore:
		Change <dev>s to values they had previously.
		Restore original checked-out list.
		Unlock checked-out list.

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

extern co_rlist *chk_rrf() ;
extern co_clist *chk_rcf() ;
extern FILE *chk_rcf_open() ;
extern char *malloc() ;

extern co_parms cops ;
extern co_updf couf ;

int chkout_r(cocsp)
co_cmd *cocsp ;
{
	co_clist *clp ;
	co_rlist *rlp ;
	co_rlist_entry *rlep ;
	co_clist_entry *clep ;
	co_deve *newdep, *olddep ;
	co_nlist nl ;
	char **dnep ;
	int ci, ri, i, n ;

						/* load reg & status files */
	if ((rlp= chk_rrf(couf.regfn)) == (co_rlist *)NULL) {
		return(CHK_ERR+1) ;
	}

	if ((clp= chk_rcf(couf.chkfn)) == (co_clist *)NULL) {
		return(CHK_ERR+1) ;
	}

	if ((cocsp->ndevs) != 0) {
		nl.list= (cocsp->dnlist) ;
		nl.n= (cocsp->ndevs) ;
	}
	else {
		if (getcolist(&nl, cocsp, clp)) {
			chk_err("can't get return list") ;
			return(CHK_ERR+2) ;
		}
		if (nl.n == 0) {
			chk_err("no resource(s) checked out") ;
			return(0) ;
		}
	}
						/* allocate status arrays */
	i= (2 * sizeof(co_deve) * nl.n) ;
	if ((newdep= (co_deve *)malloc(i)) == (co_deve *)NULL) {
		chk_err("can't allocate status arrays") ;
		return(CHK_ERR+2) ;
	}
	olddep= newdep + 1 ;
	
						/* verify return ok */
	n= nl.n ;
	dnep= nl.list ;
	for (i= 0; i< n; i++) {
		if ((ci= chk_list_flname(
			*dnep, clp->dnlist, clp->nentries))< 0) {
			chk_dev_err(*dnep, "not checked out") ;
			return(1) ;
		}
		clep= clp->clep + ci ;

		if ((ri= chk_list_flname(
			*dnep, rlp->dnlist, rlp->nentries))< 0) {
			chk_dev_err(*dnep, "not registered") ;
			return(2) ;
		}
		rlep= rlp->rlep + ri ;

		memcpy((char *)&newdep[i], (char *)&rlep->de, sizeof(co_deve)) ;
		if (chk_dev_getstat(*dnep, &olddep[i])) {
			chk_dev_err(*dnep, "can't find") ;
			return(4) ;
		}

		if (
			(strcmp(cocsp->by_uname, clep->uname) != 0)	
		     &&	(cocsp->by_uid != clep->to_uid)
		     && (cocsp->by_uid != clep->de.uid)
		     && (cocsp->by_uid != rlep->de.uid)
		     && (cocsp->by_uid != cops.root_uid)
		) {
			chk_dev_err(*dnep, "not authorized to return") ;
			return(3) ;
		}
		dnep++ ;
	}

	return(return_resources(&nl, clp, newdep, olddep)) ;
}

/*
	return_resources(nlp, clp, newdep, olddep) ;		Return resources

	Logic:
		(trap interrupts: vector to restore routine)
		Lock checked-out file.
		Change <dev>s
		Update checked-out file.
		Unlock checked-out file.
	
	Restore:
		Change <dev>s to values in list clp.
		Restore original checked-out file.
		Unlock checked-out file.

*/

static void rst_chkfile() ;
#define		LCL_RST		(-1)

static int return_resources(nlp, clp, newdep, olddep)
co_nlist *nlp ;
co_clist *clp ;
co_deve *newdep, *olddep ;
{
	FILE *ifp, *ofp ;
	co_clist_entry *clep ;
	int i, n ;

	couf.rst_dnlist= nlp->list ;
	couf.rst_dnlist_len= nlp->n ;
	couf.rst_deplist= olddep ;

	chk_trap(rst_chkfile) ;

					/* Lock checked-out list */
	if (chk_lock_mv(couf.chkfn, couf.lock_chkfn)) {
		chk_err("can't lock chkout file") ;
		return(CHK_ERR+10) ;
	}

					/* Change <devs> */
	if (chk_stat_upd(nlp->list, nlp->n, newdep)) {
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
		if (chk_list_flname(
			clep->dname,
			nlp->list, 
			nlp->n
		)< 0) {
			fprintf(
				ofp,
				"%s %s %s\n",
				clep->dname,
				clep->uname,
				clep->timeout
			) ;
		}
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

static int getcolist(nlp, cocsp, clp)
co_nlist *nlp ;
co_cmd *cocsp ;
co_clist *clp ;
{
	int i, n, count ;
	co_clist_entry *clep ;
	char **list ;

	n= clp->nentries ;
	count= 0 ;
	clep= clp->clep ;
	for (i= 0; i< clp->nentries; i++) {
		if (cocsp->by_uid == clep->to_uid) count++ ;
		clep++ ;
	}
	nlp->n= count ;
	if (count == 0) return(0) ;

	i= (count * sizeof(nlp->list[0])) ;
	if ( (list= (char **)malloc(i)) == (char **)NULL ) {
		return(1) ;
	}
	count= 0 ;
	clep= clp->clep ;
	for (i= 0; i< clp->nentries; i++) {
		if (cocsp->by_uid == clep->to_uid) {
			list[count++]= clep->dname ;
		}
		clep++ ;
	}
	nlp->list= list ;
	return(0) ;
}
