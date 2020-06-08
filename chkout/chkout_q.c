/*

	chkout_q.c	


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

extern co_parms cops ;
extern co_updf couf ;

extern co_rlist *chk_rrf() ;
extern co_clist *chk_rcf() ;

void chk_query() ;
void chk_qbusy() ;
void chk_qavail() ;
void chk_qmsg() ;

/*

	(int)rc= chkout_q(cocsp) ;

	Logic:

		Make sure that <dev>(s) are specified by user.
		For each device in registered list, do chk_queue()

*/
int chkout_q(cocsp)
co_cmd *cocsp ;
{
	co_clist *clp ;
	co_rlist *rlp ;
	int i, n ;

						/* verify not checked out */
	if ((cocsp->ndevs) == 0) {
		chk_err("arguments required for 'query' option") ;
		return(CHK_ERR_BADARG) ;
	}

	if ((rlp= chk_rrf(couf.regfn)) == (co_rlist *)NULL) {
		return(CHK_ERR+1) ;
	}
	if ((clp= chk_rcf(couf.chkfn)) == (co_clist *)NULL) {
		return(CHK_ERR+1) ;
	}

	n= (cocsp->ndevs) ;
	for (i= 0; i< n; i++) {
		chk_query(cocsp->dnlist[i], rlp, clp) ;
	}
	
	return(0) ;
}

/*

	(void) chk_query(dev, rlp, clp) ;	Print device status

	Logic:

		if <dev> not in registered list,
			report "resource not registered"
			return

		if <dev> in checked out list,
			report who & when checked out
			return

		if current values match default values,
			report "resource available"
			return
				
		report current values

*/
void chk_query(dev, rlp, clp)
char *dev ;
co_rlist *rlp ;
co_clist *clp ;
{
	int rc, ri, ci ;
	co_deve de ;
	co_rlist_entry *rlep ;

	ri= chk_list_flname(dev, rlp->dnlist, rlp->nentries) ;
	if (ri< 0) {
#if 0
		chk_qmsg(dev, "not registered") ;
#endif
		return ;
	}	
	rlep= rlp->rlep + ri ;

	ci= chk_list_flname(dev, clp->dnlist, clp->nentries) ;
	if (ci>= 0) {
		chk_qbusy(clp->clep + ci) ;
		return ;
	}

	if ((rc= chk_dev_inuse(rlep, &de))< 0) {
		chk_dev_err(dev, "can't find") ;
		return ;
	}

	if (rc==0) {
		chk_qavail(rlep) ;
		return ;
	}

	chk_dev_rptstat(dev, &de, "in use") ;
}

void chk_qbusy(clep)
co_clist_entry *clep ;
{
	chk_dev_prt(clep->dname) ;
	printf("busy by %-8s %s\n", clep->uname, clep->timeout) ;
}

void chk_qavail(rlep)
co_rlist_entry *rlep ;
{
	chk_dev_prt(rlep->dname) ;
	printf("avail") ;
	if ( (*rlep->glist) != '\0' ) {
		printf(" to members: %s", rlep->glist) ;	
	}
	printf("\n") ;
}

void chk_qmsg(dev, msg)
char *dev, *msg ;
{
	chk_dev_prt(dev) ;
	printf("%s\n", msg) ;
}
