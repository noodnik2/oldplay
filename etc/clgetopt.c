/*

	clgetopt.c:		initialization/termination routines.

	clinit(argv)				must call first.
	clgetopt(argc, argv, optstr)		call until returns EOF
	clexit(code)				must call last.

	clerror(msg)				calls clemsg(msg)

	NOTE:	currently, clgetopt uses the system call "getopt".

*/

#define		CLGETOPT_MAIN

#include	"compiler.h"
#include	"clgetopt.h"

#include	<unistd.h>
#include	<stdlib.h>

extern int optind ;

void clinit(argv)
char *argv[] ;
{
	cloptpn= argv[0] ;
	cloptind= 1 ;
}

void clexit(code)
int code ;
{
	exit(code) ;
}

int clgetopt(argc, argv, optstr)
int argc;
char *argv[], *optstr ;
{
	int r ;

	r= getopt(argc, argv, optstr) ;
	cloptind= optind ;
	return(r) ;
}

void clerror(umsg)
char *umsg ;
{
	if (umsg == (char *)NULL) umsg= "error" ;
	fprintf(stderr, "%s: %s.\n", cloptpn, umsg) ;
}

