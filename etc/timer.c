/*

	timer.c			Process timing routines


	Routines for timing other routines.

		rc= tmraloc(n_timers) ;
			allocates space for n_timers timers.

		tmrinit(t_no, name) ;
			initializes specified timer, assigns name.

		tmrstart(t_no) ;
			starts timing on specified timer.

		tmrstop(t_no) ;
			stops timing on specified timer.

		tmrfini() ;
			de-allocates all timers.

		tmrreport(FILE *file) ;
			prints report of all named timers to file.

*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/times.h>

typedef struct {
	char *name ;
	long u_elapsed ;			/* user process */
	long u_time0 ;
	long s_elapsed ;			/* system process */
	long s_time0 ;
} TIMEREG ;

static TIMEREG *t_reg= (TIMEREG *) NULL ;
static int t_max= 0 ;

void tmrinit() ;

static void tconv() ;

int tmraloc(ntmrs)
int ntmrs ;
{
	int i, size ;
	TIMEREG *thead ;

	size= sizeof(TIMEREG) * ntmrs ;
	if ((thead= (TIMEREG *)malloc(size)) == (TIMEREG *)NULL) {
		return(1) ;
	}
	t_reg= thead ;
	t_max= ntmrs ;

	for (i= 0; i< ntmrs; i++) {
		tmrinit(i, (char *)NULL) ;
	}
	return(0) ;	
}

void tmrfini()
{
	if (t_reg != (TIMEREG *)NULL) {
		free((char *)t_reg) ;
		t_max= 0 ;
		t_reg= (TIMEREG *)NULL ;
	}
}

void tmrinit(tno, name)
int tno ;
char *name ;
{
	register TIMEREG *trp ;

	trp= &t_reg[tno] ;
	trp->u_elapsed= 0L ;
	trp->s_elapsed= 0L ;
	trp->name= name ;
}

void tmrstart(tno)
int tno ;
{
	struct tms sys_t ;
	register TIMEREG *trp ;

	times(&sys_t) ;	

	trp= &t_reg[tno] ;
	trp->u_time0= (long) sys_t.tms_utime ;
	trp->s_time0= (long) sys_t.tms_stime ;
}

void tmrstop(tno)
int tno ;
{
	struct tms sys_t ;
	long u_etime, s_etime ;
	register TIMEREG *trp ;

	times(&sys_t) ;
	u_etime= (long) sys_t.tms_utime ;
	s_etime= (long) sys_t.tms_stime ;

	trp= &t_reg[tno] ;
	trp->u_elapsed+= (u_etime - trp->u_time0) ;
	trp->s_elapsed+= (s_etime - trp->s_time0) ;
}

#define		TICKS_PSEC		60

void tmrreport(xfile)
FILE *xfile ;
{
	TIMEREG *trp ;
	int i, lname ;
	int uhr, umn, usc, uhsc ;
	int shr, smn, ssc, shsc ;
	char nformat[10] ;
	char *tformat= "(%02d:%02d:%02d.%02d)" ;

	lname= 0 ;
	for (trp= &t_reg[0], i= 0; i< t_max; i++, trp++) {
		if (trp->name != (char *)NULL) {
			if (lname< strlen(trp->name)) {
				lname= strlen(trp->name) ;
			}
		}
	}
	sprintf(nformat, "%%%ds", lname) ;

	fprintf(xfile, nformat, "timer report") ;	
	fprintf(xfile, ": (user) (system)\n") ;

	for (trp= &t_reg[0], i= 0; i< t_max; i++, trp++) {
		if (trp->name != (char *)NULL) {
			tconv(trp->u_elapsed, &uhr, &umn, &usc, &uhsc) ;
			tconv(trp->s_elapsed, &shr, &smn, &ssc, &shsc) ;
			fprintf(xfile, nformat, trp->name) ;
			fprintf(xfile, ": ") ;
			fprintf(xfile, tformat, uhr, umn, usc, uhsc) ;
			fprintf(xfile, " ") ;
			fprintf(xfile, tformat, shr, smn, ssc, shsc) ;
			fprintf(xfile, "\n") ;
		}
	}
}

static void tconv(ltime, hrp, minp, secp, hsecp)
long ltime ;
int *hrp, *minp, *secp, *hsecp ;
{
	long lval ;

	lval= ltime % (long) TICKS_PSEC ;

#if 1						/* convert to msec */
	lval*= 100L ;
	lval+= (long) (TICKS_PSEC / 2) ;
	lval/= (long) TICKS_PSEC ;
#endif

	(*hsecp)= (int) lval ;

	ltime/= (long) TICKS_PSEC ;	/* ltime= nsec */
	lval= ltime % 60L ;
	(*secp)= (int) lval ;

	ltime/= (long) 60L ;		/* ltime= nmin */
	lval= ltime % 60L ;
	(*minp)= (int) lval ;

	ltime/= (long) 60L ;		/* ltime= nhrs */
	lval= ltime ;
	(*hrp)= (int) lval ;
}
