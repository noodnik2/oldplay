/*

	pdmdsetp.c			PD machine dependent code
					Copyright (C) 1992, Contahal Ltd.


	rc= pdmdsetpen(s1, s2)
	char *s1 ; 
	char *s2 ; 

	set pen s1 to color s2.

	Returns nonzero on failure.

	It is not this routines responsibility to issue
	error messages (although it could do so).

*/

#include	"compiler.h"
#include	"pd.h"

#define		NSTPENS		9
#define		NSTCOLORS	9

int pdmdsetpen(s1, s2)
char *s1, *s2 ;
{
	int i1, i2 ;

	if (sscanf(s1, "%d", &i1) != 1) return(1) ;
	if ((i1< 1) || (i1> NSTPENS)) return(1) ;

	if (sscanf(s2, "%d", &i2) != 1) return(2) ;
	if ((i2< 0) || (i2> NSTCOLORS)) return(2) ;

	return(0) ; 
}
