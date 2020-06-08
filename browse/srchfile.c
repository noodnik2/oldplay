/*

	srchfile.c:			Find file using PATH env variable
					Copyright (C) 1992, Contahal Ltd.

*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"
#include	"srchfile.h"

static void get_element() ;

char *strchr() ;

/*
	(int)flag= srchfile(fname, path, ftype) ;

	Searches the current directory, then all directories in
	"path" (in environment variable PATH format) for "fname"
	with type "ftype".  If it is found, returns full path/
	filename in "fname", and returns 1.  Else, 0 is returned
	to indicate find failure.

	NOTE:	if "fname" contains a path separator character,
		then the "path" variable is NOT used: the name,
		as given, is checked, and 1 is returned if the
		file exists as named, or 0 if not.

	buffer pointed to by "fname" is assumed to be large enough to
	hold result (dependent on max length of path in the operating
	system environment).

*/
int srchfile(fname, path, ftype)
char *fname, *path ;
int ftype ;
{
	char element[129] ;
	char filename[513] ;
	int i ;

	if (path == (char *)NULL) path= "" ;

	if (state(fname, ftype) != 0) return(1) ;
	if (strchr(fname, PATH_SEP) != (char *)NULL) return(0) ;

	while(path[0] != '\0') {
		get_element(element, &path) ;
		strcpy(filename, (element[0]!='\0')? element: ".") ;
		i= strlen(filename) ;
		filename[i]= PATH_SEP ;
		strcpy(&filename[i+1], fname) ;
		if (state(filename, ftype) != 0) {
			strcpy(fname, filename) ;
			return(1) ;
		}
	}

	return(0) ;
}

static void get_element(element, path_p)
char *element, **path_p ;
{
	char *path ;
 
	path= *path_p ;
	while(((*path) != PATH_DELIM) && ((*path) != '\0')) {
		(*element++)= (*path++) ;
	}
	(*element)= '\0' ;
	if ((*path) == PATH_DELIM) path++ ;
	*path_p= path ;
}

/*
	(int)flag= state(fname, ftype) ;

	Queries presence of file "fname", whose type is specified by "ftype".

	NOTE:	"ftype" is currently not supported, but represents the
		type of file we wish to find.  For example, exexutable,
		directory, writeable, etc...
*/
static int state(file, ftype)
char *file ;
int ftype ;
{
	struct stat stb ;
	return(stat(file, &stb)==0) ;
}
