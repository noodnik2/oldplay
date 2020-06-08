#include	<stdio.h>

#include	"compiler.h"
#include	"pd.h"

void pderrmsg() ; 

void pderrst()
{
	pderrmsg("transmission error; resynchronizing") ; 
	pderrmsg("you may need to press Enter a few times") ; 
	for (;;) { 
		if (pdgetch() == pdecm) return ; 
	}
	/*NOTREACHED*/
}
