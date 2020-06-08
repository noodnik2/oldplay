#include	<stdio.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"

static void pwp_diff() ;

void pw_cmpp_init(initpl_p)
pwlist_t *initpl_p ;
{
	int i, n ;

	pw_pctinit() ;
	n= initpl_p->n ;
	for (i= 0; i< n; i++) {
		pw_pctstart(&initpl_p->pp_a[i]) ;
	}
}

void pw_cmpp_rpt()
{
	pw_pctrpt() ;
}

void pw_cmpp_fini()
{
}

void pw_cmpp(newpl_p, oldpl_p) 
pwlist_t *newpl_p, *oldpl_p ;
{
	int i, n ;

	n= newpl_p->n ;
	for (i= 0; i< n; i++) {
		pwp_diff(&newpl_p->pp_a[i], oldpl_p) ;
	}

	n= oldpl_p->n ;
	for (i= 0; i< n; i++) {
		if (!oldpl_p->pp_a[i].free) {
			if (oldpl_p->pp_a[i].pid != 0L) {
				pw_entry("exit", &oldpl_p->pp_a[i], NULL) ;
			}
			pw_pctdel(&oldpl_p->pp_a[i]) ;
		}
	}
}

static void pwp_diff(pp_p, pl_p)
pwproc_t *pp_p ;
pwlist_t *pl_p ;
{
	int i, n ;
	long pid ;
	pwproc_t *pl_pp_p ;

	n= pl_p->n ;
	pid= pp_p->pid ;
	pl_pp_p= pl_p->pp_a ;
	for (i= 0; i< n; i++) {
		if (pid == pl_pp_p[i].pid) {
			if (pp_p->secs != pl_pp_p[i].secs) {
				pw_entry(NULL, pp_p, pl_pp_p + i) ;
				pw_pctadd(pp_p, pl_pp_p + i) ;
			}
			pl_pp_p[i].free= 1 ;
			return ;
		}
	}
	pw_entry("new", pp_p, NULL) ;
	pw_pctstart(pp_p) ;
}
