#include	<stdio.h>

#include	"compiler.h"
#include	"pw.h"
#include	"pw_proc.h"
#include	"pw_inter.h"

extern struct opts o ;
extern struct parms p ;

static void pw_doit_i() ;
static void pw_doit_ni() ;
static void pw_doit_comp() ;

void pw_doit()
{
	pwlist_t old_l ;

	pw_getp(&old_l) ;
	pw_cmpp_init(&old_l) ;

	if (o.inter)	pw_doit_i(&old_l) ;
	else		pw_doit_ni(&old_l) ;

	pw_cmpp_rpt() ;
	pw_cmpp_fini() ;
}

static void pw_doit_i(old_l_p)
pwlist_t *old_l_p ;
{
	int pwicmd ;

	fprintf(stderr, "Interactive mode entered:\n") ;
	fprintf(stderr, "(use 'q' to quit, 'r' for current report)\n") ;

	pw_inter_init() ;
	while(/*CONSTCOND*/1) {
		pwicmd= pw_inter_getcmd(p.waitsec) ;
		if (pwicmd == PWIC_RPT) pw_cmpp_rpt() ;
		else if (pwicmd == PWIC_EXIT) break ;
		else pw_doit_comp(old_l_p) ;
	}
	pw_inter_fini() ;
}

static void pw_doit_ni(old_l_p)
pwlist_t *old_l_p ;
{
	int i ;

	for (i= 0; i< p.iterations; i++) {
		sleep(p.waitsec) ;
		pw_doit_comp(old_l_p) ;
	}
}

static void pw_doit_comp(old_l_p)
pwlist_t *old_l_p ;
{
	pwlist_t new_l ;

	pw_getp(&new_l) ;
	pw_cmpp(&new_l, old_l_p) ;
	memcpy((char *)old_l_p, (char *)&new_l, sizeof(pwlist_t)) ;
}
