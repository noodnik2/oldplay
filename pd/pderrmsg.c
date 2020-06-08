/*
	(void) pderrmsg(s)
	char *s ; 

	Presents error message s to screen in RED

*/

#include	"pd.h"
#include	"pdcolor.h"

void pderrmsg(s) 
char *s ; 
{
	int i, oldclr, oldob ; 

	oldclr= pdcurcolor ; 
	oldob= pdoutbuf;

	pdoutbuf= 1 ; 	/* enable buffering	*/
	pdcolor(RED) ; 
	if (*s=='\n') { 
		wrstty("\n") ; 
		i= 1 ; 
	}
	else	i= 0 ; 

	wrstty("pd: ") ; 
	wrstty(&s[i]) ; 
	wrstty("\n") ;
	pdcolor(oldclr) ; 

	pdflushbuf();
	pdoutbuf= oldob ; 
}
