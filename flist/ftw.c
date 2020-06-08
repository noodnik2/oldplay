#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<ftw.h>

int pdir() ;

main(argc, argv)
int argc ;
char *argv[] ;
{
	if (argc< 2) exit(1) ;
	ftw(argv[1], pdir, 1000) ;
	exit(0) ;
}

static int pdir(dname, stp, type)
char *dname ;
struct stat *stp ;
int type ;
{
	printf("dir= '%s'\n", dname) ;
	return(0) ;
}

