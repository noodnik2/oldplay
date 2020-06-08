/*

	chk_dev.c			Device Information and Messages


*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"
#include	"chkout.h"
#include	"chk_pw.h"

void chk_dev_prt(dev)
char *dev ;
{
	printf("%s: ", dev) ;
}

void chk_dev_err(dev, msg)
char *dev, *msg ;
{
	char emsg[256] ;

	sprintf(emsg, "%s: %s", dev, msg) ;
	chk_err(emsg) ;
}

/*

	(int)rc= chk_dev_getstat(dev, dep) ;

	Fills in *dep with the actual status value of
	a specified resource (device).

	Returns:
		(0) status found, in *dep
		(1) not able to get file status
*/
int chk_dev_getstat(dev, dep)
char *dev ;
co_deve *dep ;
{
	struct stat sb ;

	dep->gid= dep->uid= -1 ;
	dep->mode= 0 ;

	if (stat(dev, &sb) != 0) {
		return(1) ;
	}

	dep->uid= (int) sb.st_uid ;
	dep->gid= (int) sb.st_gid ;
	dep->mode= (int) (sb.st_mode & 07777) ;
	return(0) ;
}

/*

	(void) chk_dev_rptstat(dev, dep, msg) ;

	prints actual status of device, along with user-supplied
	message about what sort of status this represents.
*/
void chk_dev_rptstat(dev, dep, msg)
char *dev, *msg ;
co_deve *dep ;
{
	char uname[CUN_SIZE+1] ;
	char gname[CGN_SIZE+1] ;
	char mspec[MSPEC_SIZE+1] ;
	cpwpent pe ;
	cpwgent ge ;

	chk_dev_prt(dev) ;
	printf("%s: ", msg) ;
	pe.uid= dep->uid ;
	if (chk_pw_pebyuid(uname, &pe) == (char *)NULL) {
		sprintf(uname, "%d", dep->uid) ;
	}
	ge.gid= dep->gid ;
	if (chk_pw_gebygid(gname, &ge) == (char *)NULL) {
		sprintf(gname, "%d", dep->gid) ;
	}
	sprintf(mspec, "%03o", dep->mode) ;
	printf("%-8s %-8s %s\n", uname, gname, mspec) ;
}

/*

	(int)rc= chk_dev_inuse(rlep, dep) ;

	Defines an "in use" device by comparing the default values
	of the device (specified in the resource entry *rlep) with
	the actual values on the device.  If the values are not the
	same, the device is "in use". 

	Returns:

		(-1)	error checking the actual values
		(0)	device matches default values
		(1)	device does not have default values

*/
int chk_dev_inuse(rlep, dep)
co_rlist_entry *rlep ;
co_deve *dep ;
{
						/* verify current values */
	if (chk_dev_getstat(rlep->dname, dep)) {
		return(-1) ;			/* don't know */
	}
	if (    ((rlep->de.uid)==(dep->uid))
	     && ((rlep->de.gid)==(dep->gid))
	     && ((rlep->de.mode)==(dep->mode))
	) {
		return(0) ;			/* not in use */
	}
	return(1) ;				/* in use */
}
