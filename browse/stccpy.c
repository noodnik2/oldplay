/*

	stccpy.c			Patterned after the Lattice function
					Copyright (C) 1990, by ==C=R=C==


*/

#include	"compiler.h"

void stccpy(d, s, ml)
char *d, *s ;
int ml ;
{
	
	while( ((ml--)> 0) && ((*s)!='\0') ) {
		(*d++)= (*s++) ;
	}
	(*d)= '\0' ;
}
