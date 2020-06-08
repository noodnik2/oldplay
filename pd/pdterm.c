/*

	pdterm.c:


*/


#include	<setjmp.h>
#include	"compiler.h"

#define		PD_ROOT		1
#include	"pd.h"
#include	"pderr.h"

extern int stopflag ;

jmp_buf eoferr ;			/* jump vector to catch aborts */

/*
	this routine is called by the RGS terminal driver when
	the PD "enter command mode" sequence is received.  
	
	sets a jump vector so that it catches a hard abort
	from within the PD command mode driver.




*/
int pdterm()
{
	char scn[4096] ;
	char errbuf[256] ;

	pdgison= 0 ;				/* not in PD graphics mode */
	pd_ints= 0 ;				/* no PD interrupts rec'd  */

	if (setjmp(eoferr)) {
		if (pdgison) {
			ttyclos(1, scn) ;
			pdgison= 0 ;
		}
		if (stopflag) {
			pderrmsg("graph mode abandoned") ;
		}
		if (pdrc) {
			sprintf(errbuf, "\nPD R(%d);\n", pdrc) ;
			pderrmsg(errbuf) ;
		}
		pdflushbuf() ;
		return( pdrc != PD_OKAY ) ;
	}

	pdcmdmode(scn) ;
	longjmp(eoferr, 1) ;
	/* NOTREACHED */
}
