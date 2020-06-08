/*
	Machine dependent code .. color map 

	rc= pdmdcmap(s)
	char *s ; 

		please return:
			0.	done
			1.	no set cmap command
			2.	argument invalid
		      999.	other error, for which you present errmsg

	Copyright (c) 1990 by C.R.C.
*/

#include	"compiler.h"

int pdmdcmap(s) 
char *s ; 
{
	return(1) ; 
}

