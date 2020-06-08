/*

	chkout_l.c	


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

extern co_parms cops ;
extern co_updf couf ;

extern co_rlist *chk_rrf() ;
extern co_clist *chk_rcf() ;

static void chk_list() ;

#define		LT_AVAIL	1
#define		LT_BUSY		2
#define		LT_INUSE	4
#define		LT_ALL		(LT_AVAIL|LT_BUSY|LT_INUSE)

/*

	(int)rc= chkout_l(cocsp) ;

	Logic:

		Make sure that arguments are ok, if any,
		For each device in registered list, do chk_list()

*/
int chkout_l(cocsp)
co_cmd *cocsp ;
{
	co_rlist *rlp ;
	co_clist *clp ;
	int i, n, lt ;
	char *larg ;

						/* verify not checked out */
	lt= LT_ALL ;
	n= (cocsp->ndevs) ;
	if (n == 1) {
		n= 0 ;
		larg= cocsp->dnlist[0] ;
		if (strcmp(larg, "all") == 0) lt= LT_ALL ;
		else if (strcmp(larg, "avail") == 0) lt= LT_AVAIL ;
		else if (strcmp(larg, "busy") == 0) lt= LT_BUSY ;
		else if (strcmp(larg, "inuse") == 0) lt= LT_INUSE ;
		else n= (cocsp->ndevs) ;
	}

	if (n != 0) {
		chk_err("invalid specification for 'list' option") ;
		return(CHK_ERR_BADARG) ;
	}

	if ((rlp= chk_rrf(couf.regfn)) == (co_rlist *)NULL) {
		return(CHK_ERR+1) ;
	}
	if ((clp= chk_rcf(couf.chkfn)) == (co_clist *)NULL) {
		return(CHK_ERR+1) ;
	}

	n= (rlp->nentries) ;
	for (i= 0; i< n; i++) {
		chk_list(rlp->dnlist[i], rlp, clp, lt) ;
	}

	return(0) ;
}

/*

	(void) chk_list(dev, rlp, clp, lt) ;	Print device status

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
void chk_list(dev, rlp, clp, lt)
char *dev ;
co_rlist *rlp ;
co_clist *clp ;
int lt ;
{
	int rc, ri, ci ;
	co_deve de ;
	co_rlist_entry *rlep ;

	ri= chk_list_flname(dev, rlp->dnlist, rlp->nentries) ;
	if (ri< 0) {
		if (lt & LT_ALL) {
			chk_qmsg(dev, "not registered") ;
		}
		return ;
	}	
	rlep= rlp->rlep + ri ;

	ci= chk_list_flname(dev, clp->dnlist, clp->nentries) ;
	if (ci>= 0) {
		if (lt & LT_BUSY) {
			chk_qbusy(clp->clep + ci) ;
		}
		return ;
	}

	if ((rc= chk_dev_inuse(rlep, &de))< 0) {
		chk_dev_err(dev, "can't find") ;
		return ;
	}

	if (rc == 0) {
		if (lt & LT_AVAIL) {
			chk_qavail(rlep) ;
		}
		return ;
	}
	
	if (lt & LT_INUSE) {
		chk_dev_rptstat(dev, &de, "in use") ;
	}
}
