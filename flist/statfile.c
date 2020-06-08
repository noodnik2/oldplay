/*

	statfile			Fill in file characteristics
					Copyright (C) 1990, by ==C=R=C==

*/

#include	"compiler.h"
#include	"flist.h"

#include	<sys/types.h>
#include	<sys/stat.h>

int statfile(fn, fsp)
char *fn;
struct fl_stat *fsp ;
{
	struct stat statbuf ;

	if (stat(fn, &statbuf)) return(1);
	fsp->size= (long) statbuf.st_size ;
	fsp->date= (long) statbuf.st_mtime ;
	fsp->isdir= ((statbuf.st_mode & S_IFDIR) != 0);
	return(0);
}
