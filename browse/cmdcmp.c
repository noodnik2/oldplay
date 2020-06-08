/*

	cmdcmp.c				test an argument for a command
						Copyright (C) 1990, by ==C=R=C==

*/

#include	"compiler.h"
#include	"browse.h"

int cmdcmp(arg, word, ml)
char *arg, *word ;
int ml ;
{
	int i ;

	for (i= 0; arg[i]!='\0'; i++) {
		if (word[i]!=arg[i]) break ;
	}
	return( (arg[i]=='\0' && i>= ml)? 1: 0 ) ;
}
