/*

	doerror.c			Present error (informatory) message
					Copyright (C) 1990, by ==C=R=C==

*/

#include	"compiler.h"
#include	"clgetopt.h"
#include	"browse.h"


void doerror(rc, msg)
int rc ;
char *msg ;
{
	char emsg[256] ;

	if (rc != 0) {
		sprintf(emsg, "rc=%d, %s", rc, msg) ;
		clerror(emsg) ;
		return ;
	}
	clerror(msg) ;
}
