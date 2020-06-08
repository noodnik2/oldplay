#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"
#include	"pw_scan.h"

extern struct opts o ;
extern struct parms p ;

extern pwscan_t *procscan_open_0() ;

void pw_getp(pl_p)
pwlist_t *pl_p ;
{
	pwscan_t *psh_p ;

	pl_p->n= 0 ;
	if ((psh_p= procscan_open_0()) == (pwscan_t *)NULL) {
		clerror("can't find process(es)") ;
		exit(1) ;
	}

	while((pl_p->n)< MAXPROCS) {
		if (procscan_read(psh_p, &pl_p->pp_a[pl_p->n])) break ;
		(pl_p->n)++ ;
	}

	if ((pl_p->n) == 0) {
		clerror("no process(es)") ;
		exit(1) ;
	}

	if ((pl_p->n)>= MAXPROCS) {
		clerror("too many procs") ;
		exit(1) ;
	}

	if (procscan_close_0(psh_p, pl_p->pp_a, pl_p->n)) {
		clerror("can't get process info") ;
		exit(1) ;
	}

}

/*
	internal routines
*/

static int pw_getp_reject(pp_p)
pwproc_t *pp_p ;
{
	if (pp_p->pid == p.mypid) return(1) ;
	if (o.reject_user != (char *)NULL) {
		if (strcmp(pp_p->user, o.reject_user) == 0) return(1) ;
	}
	return(0) ;
}

static int procscan_read(psh_p, pp_p)
pwscan_t *psh_p ;
pwproc_t *pp_p ;
{
	int rc ;

	while(/*CONSTCOND*/1) {
		if (rc= procscan_read_0(psh_p, pp_p)) return(rc) ;
		if (!pw_getp_reject(pp_p)) break ;
	}
	return(0) ;
}
