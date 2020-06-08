/*

	index.c				find a string in a string
					Copyright (C) 1990m by ==C=R=C==

*/

#include	<stdio.h>
#include	<ctype.h>

#include	"compiler.h"

int index(haystack, needle)
char *haystack, *needle ;
{
	int i, l, h, w ;

	l= strlen(needle) ;
	h= strlen(haystack) ;
	w= (h-l+1) ;
	for (i= 0; i< w; i++) {
		if (memcmp(haystack+i, needle, l)==0) return(i) ;
	}
	return(-1);
}

void mklowcase(s)
char *s;
{
	while(*s) {
		if (isupper(*s)) (*s)= tolower(*s) ;
		s++ ;
	}
} 
