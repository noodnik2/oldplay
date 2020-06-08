/*

	pdgetch.c			Get character from RGS/PD input stream
					Copyright (C) 1990, by ==C=R=C==

		c= pdgetch() ;
		int c ;

		reads character from input stream.

		if user aborts RGS/PD mode, a longjmp()
		is taken to the jmp_buf eoferr.

*/

#include	<stdio.h> 
#include	<setjmp.h>
#include	"compiler.h"
#include	"pd.h"
#include	"pderr.h"
#include	"termsig.h"

extern int stopflag ;				/* true when user BREAKs  */
extern jmp_buf eoferr ;				/* user/PD abort vectors  */
extern int pd_ints ;				/* number of user BREAKs  */

static int pdinp_c ;				/* RGS/PD input character */

static int pd_getch_poll() ;			/* RGS/PD interrupt hndlr */
void pd_break() ;				/* handle user/PD BREAK   */

/*
	this routine is called when PD driver wants input.

	it waits for a character, or a user BREAK interrupt.

	if the break handler aborts, a longjmp() is taken
	back to the main pd terminal driver's exit routine.

	never returns a negative value.

*/
int pdgetch()
{
	do {
		termwait(pd_getch_poll) ;		/* wait on network */
		if (stopflag) {				/* if user BREAK,  */
			pd_break() ;			/* handle interupt */
			stopflag= 0 ;			/* only do it once */
		}
	} while(pdinp_c< 0) ;				/* don't return <0 */
	return(pdinp_c) ;				/* return input ch */
}

/*
	this routine is called by the network when there
	may be a character available.  It returns a neg-
	ative value if there is no character available,
	or it returns the character value.
*/
static int pd_getch_poll()
{
	chau xc[2];

	if (iskbch()) {			/* Handle terminal keyboard */
		getkbch(xc);
		if ((xc[0]<= ' ') && (xc[0]!='\0')) {
			termputch((int)xc[0]) ;
		}
	}

	if ((pdinp_c= termgetch())< 0) {
		return(stopflag? 1: 0) ;
	}

	return(1) ;
}

/*
	this routine is called when the PD driver requests input
	and the user has signalled a BREAK condition.
*/
static void pd_break()
{
	if ((++pd_ints)> 3) {		/* if more than 3, */
		longjmp(eoferr, 1) ;	/* then abandon    */
	}
	if (pdbrkc>= 0) {		/* send PD break c */
		termputch(pdbrkc) ;
	}
	else {
		termsig(TSIG_BREAK) ;	/* send BREAK sig. */
	}
}
