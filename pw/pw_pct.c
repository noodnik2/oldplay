#include	<stdio.h>
#include	<sys/time.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"
#include	"pw_pct.h"

extern struct parms p ;

static pwpctl_t pctl ;

void pw_pctinit()
{
	pctl.n= 0 ;
}

void pw_pctstart(pp_p)
pwproc_t *pp_p ;
{
	int i ;

	if (pp_p->pid == 0L) return ;
	if (pp_p->pid == p.mypid) return ;

	for (i= 0; i< pctl.n; i++) {
		if (pctl.pc_a[i].free) break ;
	}
	if (i == pctl.n) {
		if (pctl.n == MAXPROCS) {
			clerror("pct table overflow") ;
			exit(1) ;
		}
		i= pctl.n++ ;
	}
	pctl.pc_a[i].free=  0 ;	
	pctl.pc_a[i].pid=   pp_p->pid ;
	memcpy(pctl.pc_a[i].user, pp_p->user, PWPL_USER) ;
	memcpy(pctl.pc_a[i].name, pp_p->name, PWPL_NAME) ;
	pctl.pc_a[i].stime= (long) time(0) ;
	pctl.pc_a[i].ttime= (long) 0 ;
}

void pw_pctadd(newpp_p, oldpp_p)
pwproc_t *newpp_p, *oldpp_p ;
{
	int i ;

	if ((i= pw_pctfind(newpp_p->pid))< 0) return ;
	pctl.pc_a[i].ttime+= (newpp_p->secs - oldpp_p->secs) ;
}

void pw_pctrpt()
{
	int nactive ;
	int i, first ;
	long x, time1 ;
	double pct ;

	nactive= 0 ;
	first= 1 ;
	time1= (long) time(0) ;
	for (i= 0; i< pctl.n; i++) {
		if (pctl.pc_a[i].ttime == 0L) continue ;
		x= (time1 - pctl.pc_a[i].stime) ;
		pct=  (double) pctl.pc_a[i].ttime ;
		pct/= (double) x ;
		pct*= (double) 100.0e0 ;
		if (first) {
			printf("\n%-10s %5s Proc\n", "user", "pct") ;
			first= 0 ;
		}
		printf(
			"%-10s %5.2f %s\n",
			pctl.pc_a[i].user,
			pct,
			pctl.pc_a[i].name
		) ;	
		nactive++ ;
	}
	printf("--- %d active process(es) ---\n", nactive) ;
}

void pw_pctdel(pp_p)
pwproc_t *pp_p ;
{
	int i, j ;

	if ((i= pw_pctfind(pp_p->pid))< 0) return ;
	pctl.pc_a[i].free= 1 ;
	for (j= i+1; j< pctl.n; j++) {
		if (!pctl.pc_a[j].free) break ;
	}
	if (j == pctl.n) pctl.n= i ;
}

/*
	internal subroutines
*/

static int pw_pctfind(pid)
long pid ;
{
	int i ;

	for (i= 0; i< pctl.n; i++) {
		if (pctl.pc_a[i].pid == pid) {
			if (!pctl.pc_a[i].free) return(i) ;
		}
	}
	return(-1) ;
}
