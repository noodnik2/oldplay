/*

	which:	   find file by searching across ${PATH}

	syntax:	   which name [name ...]

		if name is found "as is", echoes name
		if name is found in search path, prints full filename.
		if name is not found, prints nothing.

	2.10.90, Marty
	5.5.92, Marty updated for MS-DOS

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"
#include	"srchfile.h"

char *getenv() ;

static void print_syntax() ;
static void findfn() ;
static void prtfn() ;

static char *progname ;

int main(argc, argv)
int argc ;
char *argv[] ;
{
	char *path ;
	int arg ;

	progname= argv[0] ;
	if (argc< 2) {
		print_syntax() ;
		exit(1) ;
	}

	path= getenv("PATH") ;
	for (arg= 1; arg< argc; arg++) {
		findfn(argv[arg], path) ;
	}
}

static void findfn(fname, path)
char *fname, *path ;
{
	char filename[513] ;

#if	OPSYS==OPMSDOS	
	if (hasext(fname)) {
		strcpy(filename, fname) ;
		if (srchfile(filename, path, FT_EXEC)) {
			prtfn(filename) ;
		}
		return ;
	}
	strcpy(filename, fname) ;
	strcat(filename, ".COM") ;
	if (srchfile(filename, path, FT_EXEC)) {
		prtfn(filename) ;
		return ;
	}
	strcpy(filename, fname) ;
	strcat(filename, ".EXE") ;
	if (srchfile(filename, path, FT_EXEC)) {
		prtfn(filename) ;
		return ;
	}
	strcpy(filename, fname) ;
	strcat(filename, ".BAT") ;
	if (srchfile(filename, path, FT_EXEC)) {
		prtfn(filename) ;
		return ;
	}
#else
	strcpy(filename, fname) ;
	if (srchfile(filename, path, FT_EXEC)) { 	/* Unix */
		prtfn(filename) ;
		return ;
	}
#endif
}

void prtfn(filename)
char *filename ;
{
	printf("%s\n", filename) ;
}

/*
	(int) hasext(fname) ;

	Returns non-zero iff "fname" does not have an extension.
*/
int hasext(fname)
char *fname ;
{
	register char *p ;
	
	for (p= fname+strlen(fname); (p> fname) && ((*p) != PATH_SEP); p--) {
		if ((*p) == '.') return(1) ;
	}
	return(0) ;
}

static void print_syntax()
{
	fprintf(stderr, "%s: which name [name ...]\n", progname) ;
}
