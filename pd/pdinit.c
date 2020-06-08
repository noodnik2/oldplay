#define			DEBUG		1

/*

	pdinit.c			Initialize PD system
						

	rc= pdinit() ;			One-time startup.  PD aborts if
					this returns nonzero value.

	void pdfini() ;			One-time closeout.  Called before
					Stata RGS returns to the opsys.

*/

#include	<stdio.h>
#include	"compiler.h"
#include	"pd.h"

void pdmdclos() ; 

static int tisset = 0 ; 

#if		DEBUG==1
FILE *pdfile ;				/* debug only */
#endif

int pdinit()
{
	if (pdmdinit()) return(1) ; 

#if	DEBUG==1
	if ((pdfile= fopen("pdfile.dbg", "w")) == (FILE *)NULL) {
		pderrmsg("can't open pdfile") ;
	}
#endif

	tisset= 1 ; 
	return(0) ; 
}

int pdfini()
{
	if (tisset==0) return(0) ; 
	pdmdclos() ; 

#if	DEBUG==1
	fclose(pdfile) ;
#endif

	tisset= 0 ; 
	return(0) ; 
}
