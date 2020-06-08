/*

	chk_pw.c


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"chk_pw.h"
#include	"chk_pw0.h"

static cpwgauth *cpauth() ;

/*

	chk_pw_cuname(uname)			Get current user name

*/
char *chk_pw_cuname(uname)
char *uname ;
{
	static char lcl_uname[L_cuserid+1]= "\0" ;

	cuserid(lcl_uname) ;
	if (lcl_uname[0] == '\0') {
		return((char *)NULL) ;
	}
	strncpy(uname, lcl_uname, CUN_SIZE+1) ;
	return(uname) ;
}

/*

	chk_pw_pebyuname(uname, pep)		Get password entry by user name

*/
cpwpent *chk_pw_pebyuname(uname, pep)
char *uname ;
cpwpent *pep ;
{
	struct passwd *pwep ;

	pwep= getpwnam(uname) ;
	if (pwep == (struct passwd *)NULL) {
		return((cpwpent *)NULL) ;
	}
	pep->uid= (int) pwep->pw_uid ;	
	pep->gid= (int) pwep->pw_gid ;
	return(pep) ;
}

/*

	chk_pw_gebygname(gname, gep)		Get group entry by group name

*/
cpwgent *chk_pw_gebygname(gname, gep)
char *gname ;
cpwgent *gep ;
{
	struct group *grep ;

	grep= getgrnam(gname) ;
	if (grep == (struct group *)NULL) {
		return((cpwgent *)NULL) ;
	}
	gep->gid= (int) grep->gr_gid ;
	return(gep) ;
}

/*

	chk_pw_pebyuid(uname, pep)		Get password entry by uid

*/
char *chk_pw_pebyuid(uname, pep)
char *uname ;
cpwpent *pep ;
{
	struct passwd *pwep ;

	pwep= getpwuid(pep->uid) ;
	if (pwep == (struct passwd *)NULL) {
		return((char *)NULL) ;
	}
	strncpy(uname, pwep->pw_name, CUN_SIZE+1) ;
	return(uname) ;
} 

/*

	chk_pw_gebygid(gname, gep)		Get group entry by gid

*/
char *chk_pw_gebygid(gname, gep)
char *gname ;
cpwgent *gep ;
{
	struct group *grep ;

	grep= getgrgid(gep->gid) ;
	if (grep == (struct group *)NULL) {
		return((char *)NULL) ;
	}
	strncpy(gname, grep->gr_name, CGN_SIZE+1) ;
	return(gname) ;
}
	
/*

	chk_pw_gauthbygname(gap, gname)		Get authorization list by gid

*/
cpwgauth *chk_pw_gauthbygname(gap, gname)
cpwgauth *gap ;
char *gname ;
{
	return(cpauth(gap, getgrnam(gname))) ;
}

/*
	chk_pw_gauthbygid(gap, gep)		Get authorization list by gid

*/
cpwgauth *chk_pw_gauthbygid(gap, gep)
cpwgauth *gap ;
cpwgent *gep ;
{
	return(cpauth(gap, getgrgid(gep->gid))) ;
}

/*
	local subroutines
*/

/*

	(cpwgauth *)cpauth(gap, grep) ;

*/

cpwgauth *cpauth(gap, grep)
cpwgauth *gap ;
struct group *grep ;
{
	register int i ;

	if (grep == (struct group *)NULL) {
		return((cpwgauth *)NULL) ;
	}
	for (i= 0; grep->gr_mem[i] != (char *)NULL; i++) {
		if (i>= CGA_MAXAUTHS) {
			chk_err("group authorization list overflow") ;
			return((cpwgauth *)NULL) ;
		}
		strncpy(gap->uname[i], grep->gr_mem[i], CGN_SIZE+1) ;
	}
	gap->nauths= i ;
	return(gap) ;
}
