/*


	chkout.c


*/

#include	<stdio.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"chkout.h"
#include	"chk_pw.h"

static void default_values() ;
static void process_values() ;
static void init() ;

co_updf couf ;
co_parms cops ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	co_cmd cocs ;

	default_values(&cocs) ;
	init(argc, argv, &cocs) ;
	process_values(&cocs) ;
	exit(doit(&cocs)) ;
}

static void default_values(cocsp)
co_cmd *cocsp ;
{
					/* initialize co_parms struct */
	strcpy(cops.regid, RFN_ID) ;
	strcpy(cops.chkid, CFN_ID) ;
	strcpy(cops.chkdir, CHK_DIR) ;
	strcpy(cops.basefile, BASE_FN) ;
	cops.root_uid= ROOT_UID ;

					/* initialize co_cmd struct */
	memset((char *)cocsp, '\0', sizeof(co_cmd)) ;
	cocsp->cmd= COC_UNKNOWN ;
	cocsp->for_uname[0]= '\0' ;
	cocsp->for_gname[0]= '\0' ;
	strcpy(cocsp->mspec, DEFAULT_MSPEC) ;
	chk_time(cocsp->cmd_time) ;
}

/*

	(void) process_values(cocsp) ;

	This routine is WAY WAY too long!!!
*/
static void process_values(cocsp)
co_cmd *cocsp ;
{
	char uname[CUN_SIZE+1] ;
	cpwpent ue ;
	cpwgent ge ;
	cpwgauth ga ;
	int i, err ;

	if (chk_pw_cuname(uname) == (char *)NULL) {
		chk_err("can't get user name") ;
		exit(1) ;
	}
	strncpy(cocsp->by_uname, uname, sizeof(cocsp->by_uname)) ;

	if (cocsp->for_uname[0] == '\0') {
		strncpy(cocsp->for_uname, uname, sizeof(cocsp->for_uname)) ;
	}

	if (chk_pw_pebyuname(cocsp->for_uname, &ue) == (cpwpent *)NULL) {
		chk_err("can't find user id") ;
		exit(1) ;
	}

	if (cocsp->for_gname[0] == '\0') {
		ge.gid= ue.gid ;	
		err= (chk_pw_gebygid(cocsp->for_gname, &ge) == (char *)NULL) ;
	}
	else err= (chk_pw_gebygname(cocsp->for_gname, &ge) == (cpwgent *)NULL) ;
	if (err) {
		chk_err("can't find group id") ;
		exit(1) ;
	}

#if 0
	/*
		This deleted because of the following reasoning:

		If I'm Joe Nobody, yet I have authority to checkout
		device X, it's going to become mine (or somebody to
		whom I choose), and I (he) can do a "chgrp" command
		to any group desired.
	*/
	if (ue.gid != ge.gid) {
		if (chk_pw_gauthbygid(&ga, &ge) == (cpwgauth *)NULL) {
			chk_err("can't find group authorization list") ;
			exit(1) ;
		}
		for (i= 0; i< ga.nauths; i++) {
			if (strcmp(ga.uname[i], cocsp->for_uname)==0) {
				break ;
			}
		}
		if (i == ga.nauths) {
			chk_err("group not authorized") ;
			exit(1) ;
		}
	}
#endif

	cocsp->for_uid= (int) ue.uid ;
	cocsp->for_gid= (int) ge.gid ;
	if (sscanf(cocsp->mspec, "%o", &cocsp->mode) != 1) {
		chk_err("invalid 'mode' value") ;
		exit(1) ;
	}

	if (strcmp(cocsp->by_uname, cocsp->for_uname) != 0) {
		if (chk_pw_pebyuname(cocsp->by_uname, &ue) == (cpwpent *)NULL) {
			chk_err("can't find by_user id") ;
			exit(1) ;
		}
		cocsp->by_uid= (int) ue.uid ;
		cocsp->by_gid= (int) ue.gid ;
	}
	else {
		cocsp->by_uid= cocsp->for_uid ;
		cocsp->by_gid= cocsp->for_gid ;
	}

	ge.gid= cocsp->by_gid ;	
	if (chk_pw_gebygid(cocsp->by_gname, &ge) == (char *)NULL) {
		chk_err("can't get by_group") ;
		exit(1) ;
	}

	sprintf(cops.chkfile, CFN_FORMAT, cops.basefile) ;
	sprintf(cops.regfile, RFN_FORMAT, cops.basefile) ;
	sprintf(cops.lock_chkfile, LFN_FORMAT, cops.chkfile) ;
	sprintf(cops.lock_regfile, LFN_FORMAT, cops.regfile) ;

	if (chk_lock_temp(cops.temp_basefile)) {
		chk_err("can't make temp fn") ;
		exit(1) ;
	}
	sprintf(cops.temp_chkfile, CFN_FORMAT, cops.temp_basefile) ;
	sprintf(cops.temp_regfile, RFN_FORMAT, cops.temp_basefile) ;

	chk_fn(cops.chkfile, couf.chkfn) ;
	chk_fn(cops.regfile, couf.regfn) ;
	chk_fn(cops.lock_chkfile, couf.lock_chkfn) ;
	chk_fn(cops.lock_regfile, couf.lock_regfn) ;
	chk_fn(cops.temp_chkfile, couf.temp_chkfn) ;
	chk_fn(cops.temp_regfile, couf.temp_regfn) ;

}

static void init(argc, argv, cocsp)
int argc ;
char *argv[] ;
co_cmd *cocsp ;
{
	int i, ncmd, ndet ;

	clinit(argv) ;
	ndet= ncmd= 0 ;
	while((i= clgetopt(argc, argv, "cg:hlm:qru:z"))> 0) {
		switch(i) {

			case 'h':	cocsp->cmd= COC_USAGE ;
					ncmd++ ;
					break ;

			case 'c':	cocsp->cmd= COC_CHKOUT ;
					ncmd++ ;
					break ;

			case 'r':	cocsp->cmd= COC_RETURN ;
					ncmd++ ;
					break ;

			case 'q':	cocsp->cmd= COC_QUERY ;
					ncmd++ ;
					break ;

			case 'l':	cocsp->cmd= COC_LIST ;
					ncmd++ ;
					break ;

			case 'u':	strncpy(
						cocsp->for_uname,
						cloptarg, 
						sizeof(cocsp->for_uname)
					) ;
					ndet++ ;
					break ;

			case 'g':	strncpy(
						cocsp->for_gname,
						cloptarg, 
						sizeof(cocsp->for_gname)
					) ;
					ndet++ ;
					break ;

			case 'm':	strncpy(
						cocsp->mspec,
						cloptarg, 
						sizeof(cocsp->mspec)
					) ;
					ndet++ ;
					break ;

			case 'z':	cops.debug= 1 ;
					break ;
				
			default:
					exit(1) ;
					break ;

		}

	}	

	if (ncmd> 1) {
		chk_err("conflicting options") ;
		exit(1) ;
	}

	if ((cocsp->ndevs= (argc - cloptind))> 0) {
		cocsp->dnlist= argv + cloptind ;
	}
	else {
		cocsp->dnlist= (char **)NULL ;
	}

	if ((ncmd == 0) && ((cocsp->ndevs) == 0)) {
		cocsp->cmd= COC_USAGE ;
	}

	if ((cocsp->cmd) == COC_UNKNOWN) {
		chk_err("no command specified") ;
		exit(1) ;
	}

	if ((cocsp->cmd) != COC_CHKOUT) {
		if (ndet != 0) {
			chk_err("incompatible options") ;
			exit(1) ;
		}
	}

}

static int doit(cocsp)
co_cmd *cocsp ;
{
	int rc, i ;

	if (cops.debug) {
		printf("cmd= %d\n", cocsp->cmd) ;
		printf("b_uname= %s\n", cocsp->by_uname) ;
		printf("b_gname= %s\n", cocsp->by_gname) ;
		printf("f_uname= %s(%d)\n", cocsp->for_uname, cocsp->for_uid) ;
		printf("f_gname= %s(%d)\n", cocsp->for_gname, cocsp->for_gid) ;
		printf("mspec= %s\n", cocsp->mspec) ;
		for (i= 0; i< cocsp->ndevs; i++) {
			printf("dname= %s\n", cocsp->dnlist[i]) ;
		}
	}

	switch(cocsp->cmd) {
		case COC_CHKOUT: rc= chkout_c(cocsp) ;	break ;
		case COC_RETURN: rc= chkout_r(cocsp) ;	break ;
		case COC_QUERY:  rc= chkout_q(cocsp) ;	break ;
		case COC_LIST:	 rc= chkout_l(cocsp) ;	break ;
		case COC_USAGE:  rc= chkout_u(cocsp) ;	break ;
	}

	return(rc) ;
}
