#define		DEBUG		1

#include	<stdio.h> 
#include	<setjmp.h>
#include	"compiler.h"
#include	"pd.h"
#include	"pderr.h"
#include	"pdgetech.h" 

jmp_buf encerr ; 

extern int ttymode ;

#if DEBUG==1
extern FILE *pdfile ;
#endif

int pdtty(scn)
char *scn;
{
	int rc, s_outbuf ; 

	s_outbuf= pdoutbuf ; 
	pdoutbuf= 1 ; 
	rc= pd_gphu(scn) ; 
	pdflushbuf() ; 
	pdoutbuf= s_outbuf ; 
	return(rc) ; 
}

static int pd_gphu(scn) 
char *scn ;
{ 
	int op, i, y, x, y1, x1, rad, inter, len, sty ;
	char s[200] ; 

	pdgison= 0 ; 

		/*
		   a longjmp to this point ends graphics mode.  If 
		   argument is 1, it is an error abort, otherwise 
		   all is okay.
		*/

	if (y= setjmp(encerr)) { 
		if (pdgison) { 
			ttyclos(1, scn) ; 
			pdgison= 0 ; 
		}
		return(y==1 ? GO_ERRST : PD_OKAY) ; 
	}

	if (ttymode==0) {
		pdwrstty("pdtty: driver disabled; TTY ignored\n") ;
		longjmp(encerr, 1) ;
	}

	pdgeteci() ;

	if (badfi(scn)) {
		pdwrstty("pdtty: incompatible TTY file; ignored\n") ;
		longjmp(encerr, 1) ;
	}


		/*
		   gison is now true.
		   we exit the loop below when pdgetech() takes a longjmp 
		   to encerr.  If it passes back a 2, everything is okay
		   (request for ECM)
		*/

	for(;;) { 
		op= pdgetech() ; 
		if (op>=0x20 && op<=0x2f) { 
			y= pdgetesh() ; 
			x= pdgetesh() ; 
			ttypt(y, x, op-0x20) ; 
		}
		else if (op==0x0f) { 		/* close	*/
			ttyclos(1, scn) ; 
			pdgison= 0 ; 
			for (i= 0; i< 10; i++) { 
				pdgetech() ; 	/* should be <ECM>	*/
			}
			longjmp(encerr, 1) ; 
		}
		else if (op==0x04) { 		/* ttynoop	*/
			ttynop() ;
		}
		else	{
			fprintf(pdfile, "pdtty: bad opcode: op=%d\n", op) ;
			longjmp(encerr, 1) ; 
		}
	}
	/*NOTREACHED*/
}

#if	DEBUG==1
#define		DEBUG_HDR	1
#else
#define		DEBUG_HDR	0
#endif
			
static int badfi(scn) 
char *scn ;
{
#if 0
	int i, y, x ; 

#if DEBUG_HDR
	chau hdr[5] ;

	hdr[0]= pdgetech() ;
	hdr[1]= pdgetech() ;
	hdr[2]= pdgetech() ;
	hdr[3]= pdgetech() ;
	hdr[4]= pdgetech() ;

	if (hdr[0] != GDLCH1) return(dbgh(hdr)) ;
	if (hdr[1] != GDLCH2) return(dbgh(hdr)) ;
	if (hdr[2] != GDLRELEASE) return(dbgh(hdr)) ;
	i= hdr[3] ;
	if (i!=LOHI && i!=HILO) return(dbgh(hdr)) ;
	pdborder= i ; 
	if (hdr[4] != 0 /* gdlmetr */) return(dbgh(hdr)) ;

#else
	if (pdgetech() != GDLCH1) return(1) ; 
	if (pdgetech() != GDLCH2) return(1) ; 
	if (pdgetech() != GDLRELEASE) return(1) ; 
	i= pdgetech() ; 
	if (i!=LOHI && i!=HILO) return(1) ; 
	pdborder= i ; 
	if (pdgetech() != 0 /* gdlmetr */) return(1) ; 

#endif

	y= pdgetesh() ; 
	x= pdgetesh() ; 
	ttymetr(y,x, 1, scn) ; 

#endif
	pdgison= 1 ; 
	return(0) ; 

}

#if	DEBUG_HDR==1

static int dbgh(hb)
chau *hb ;
{
	int i ;

	for (i= 0; i< 5; i++) {
		fprintf(pdfile, "hdr[%d]= %d\n", i, (int)hb[i]) ;
	}
	return(1) ;
}
#endif
