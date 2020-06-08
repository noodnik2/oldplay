/*

	chk_stat.c				Change Resource Status


*/

#include	<stdio.h>
#include	<sys/stat.h>

#include	"compiler.h"
#include	"chkout.h"

/*

	(int)rc= chk_stat_upd(dnlist, list_len, deplist) ;

	Change the status of the devices in specified in
	device list 'dnlist' to the corresponding status
	values in array deplist[].

	Returns:
		0	File statuses changed.
		1	Error from chmod() function.
		2	Error from chown() function.

*/
int chk_stat_upd(dnlist, list_len, deplist)
char **dnlist ;
int list_len ;
co_deve *deplist ;
{
	int i, rc ;
	char **dnep ;

	dnep= dnlist ;
	for (i= 0; i< list_len; i++) {
		if ((rc= stat_upd(*dnep, deplist)) != 0) {
			return(rc) ;
		}
		dnep++ ;
		deplist++ ;
	}
	return(0) ;
}

#if 0
/*

	(int)rc= chk_stat_rst(clp) ;

	Resets the status of all devices in specified
	checkout list to their status values.

	Returns:
		0	File statuses changed.
		1	Error from chmod() function.
		2	Error from chown() function.

*/
int chk_stat_rst(clp)
co_clist *clp ;
{
	int i, list_len, rc ;
	co_clist_entry *clep ;

	clep= clp->clep ;
	list_len= (clp->nentries) ;
	for (i= 0; i< list_len; i++) {
		if ((rc= stat_upd(clep->dname, &clep->de)) != 0) {
			return(rc) ;
		}
		clep++ ;
	}
	return(0) ;
}
#endif

/*
	local subroutines
*/

static int stat_upd(fn, dep)
char *fn ;
co_deve *dep ;
{
	if (chmod(fn, (mode_t)dep->mode)) {
		return(1) ;
	}
	if (chown(fn, (uid_t)dep->uid, (gid_t)dep->gid)) {
		return(2) ;
	}
	return(0) ;
}
