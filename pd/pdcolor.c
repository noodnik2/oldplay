#include	"pd.h"
#include	"pdcolor.h"

/*
	(void) pdcolor(i)
	int i ; 
		switches terminal to specified color.
		i takes on the values:
		0	yellow
		1	green
		2	blue
		3	red
		4	white
		5	colors off (default state)
*/

int pdcurcolor= NOCOLOR ; 
void pdmdcolor() ; 

void pdcolor(i) 
int i ;
{
	if ( i != pdcurcolor) {
		pdmdcolor(i) ; 
		pdcurcolor= i ; 
	}
}


