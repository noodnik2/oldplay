/*

	chk_list.c


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chkout.h"

int chk_list_flname(name, list, listlen)
char *name ;
char *list[] ;
int listlen ;
{
	register int i ;

	for (i= 0; i< listlen; i++) {
		if (strcmp(name, list[i]) == 0) return(i) ;
	}
	return(-1) ;
}

#if 0
static void pflists(rfp, cfp)
co_rlist *rfp ;
co_clist *cfp ;
{
	printf("resource file:\n") ;
	for (i= 0; i< rlp->nentries; i++) {
		printf(
			"%s: \"%s|%s|%s|%s\"\n",
			rlp->dnlist[i],
			rlp->rlep[i].uname,
			rlp->rlep[i].gname,
			rlp->rlep[i].mspec,
			rlp->rlep[i].glist
		) ;
	}

	printf("checkout file:\n") ;
	for (i= 0; i< clp->nentries; i++) {
		printf(
			"%s: \"%s|%s\"\n",
			clp->dnlist[i],
			clp->clep[i].uname,
			clp->clep[i].timeout
		) ;
	}
}
#endif
