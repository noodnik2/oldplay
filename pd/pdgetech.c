#define		DEBUG	0

/*

	int pdgetech() 

	0 1 2 3 4 5 6 7 
	0 1

	Think of receiving bits:

	01234567	01234567
	01abcdef			cbit = 2
	01ghijkl			cbit = 4 
			abcdefgh
	01mnopqr			cbit = 6 
			ijklmnop 
	01stuvwx
			qrstuvwx

	And the pattern then restarts.
*/

#include	<stdio.h>
#include	<setjmp.h>
#include	"compiler.h"
#include	"pd.h"
#include	"pdgetech.h" 

extern jmp_buf encerr ; 

static int cbit ; 			/* Must initialize with pdgeteci() */
static chau bits ; 

#if	DEBUG==1
extern FILE *pdfile ;
#endif

static chau pd_getutb()
{
	int x ; 

			/* ignore \r and \n bytes 	*/

	for(x= pdgetch(); x=='\r' || x=='\n'; x= pdgetch()) ; 
	if ((x & 0x00c0) != 0x40) {
#if	DEBUG==1
		if (x != pdecm) {
			fprintf(pdfile, "pdgetech: badchar is 0x%02x\n", x) ;
		}
#endif
		longjmp(encerr, (x==pdecm)? 2: 1) ; 

	}
	return((chau) x) ; 
}

void pdgeteci()
{
	cbit= 2 ;
}
	
int pdgetech() 
{
	chau xb ; 
	
	switch(cbit) { 

		case 2:		xb= (pd_getutb() << 2) ; 
				bits= pd_getutb() ; 
				xb|= ((chau) (bits & 0x30) >> 4) ; 
				cbit= 4 ; 
				break ; 

		case 4:		xb= (bits & 0x0f) << 4 ; 
				bits= pd_getutb() ; 
				xb|= ((chau) (bits & 0x3c) >> 2) ; 
				cbit= 6 ; 
				break ; 

		case 6:		xb= (bits & 0x03) << 6 ; 
				bits= pd_getutb() ; 
				xb|= (bits & 0x3f) ; 
				cbit= 2 ; 
				break ; 

	}
	return((int) xb) ; 
}

int pdgetesh()
{
	union { 
		shint	x ; 
		struct  { 
			chau b0 ; 
			chau b1 ; 
		} bytes ;
	} val ; 

	if (pdborder != BYTEORDER) { 
		val.bytes.b1= (chau) pdgetech() ; 
		val.bytes.b0= (chau) pdgetech() ; 
	}
	else { 
		val.bytes.b0= (chau) pdgetech() ; 
		val.bytes.b1= (chau) pdgetech() ; 
	}
	return((int) val.x) ; 
}
