#define		MSRC_Revision		1

/*

	perror.c

		perror <errno>

	Prints "errno" in text form.

*/

#include	<stdio.h>
#include	<errno.h>

extern int errno ;

static char *szProgName ;

static void bad()
{
	fprintf(stderr, "%s: specify errno\n", szProgName) ;
	exit(12) ;
	/*NOTREACHED*/
}

void main(int argc, char *argv[])
{
	int iErrno ;

	szProgName= argv[0] ;
	
	if (argc != 2) bad() ;

	iErrno= atoi(argv[1]) ;
	if (iErrno == 0) bad() ;
	
	errno= iErrno ;
	perror(szProgName) ;
	exit(0) ;
	/*NOTREACHED*/
}

/* end of perror.c */

