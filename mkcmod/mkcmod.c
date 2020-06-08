/*	Revision:		21
*/

/*

	mkcmod.c			mkcmod C Source
					Copyright (C) 1996, Marty Ross

*/

#include	<stdio.h>
#include	<ctype.h>


/*
	local #defines
*/

#define	MKCMODMAX_FILENAMELEN		256
#define	MKCMODPATH_FILEPREFIX		"/usr/local/template/"
#define	MKCMODPATH_TMPPREFIX		"/tmp/"
#define	MKCMODCMD_SCHANGE		"schange -y"
#define	MKCMODCMD_DIR			"ls -l %s*"


/*
	local typedefs
*/

/*
	internal data
*/
static char htemplate_fn[MKCMODMAX_FILENAMELEN+1]= MKCMODPATH_FILEPREFIX "h" ;
static char ctemplate_fn[MKCMODMAX_FILENAMELEN+1]= MKCMODPATH_FILEPREFIX "c" ;
static char tempfile_fn[]= MKCMODPATH_TMPPREFIX "_temp" ;
static char tempfile_fn2[]= MKCMODPATH_TMPPREFIX "_temp2" ;

static char modname_l[MKCMODMAX_FILENAMELEN+1] ;
static char modname_n[MKCMODMAX_FILENAMELEN+1] ;
static char modname_u[MKCMODMAX_FILENAMELEN+1] ;
static int isdebug= 0 ;

/*
	public data
*/

/*
	internal forward function decls
*/
static void copytrans2upper(char *dest, char *src) ;
static void copytrans2lower(char *dest, char *src) ;
static void transfile(char *from, char *to, char *ifn, char *ofn) ;
static void dodir(char *modname) ;
static int fileexists(char *fn) ;
static void makefile(char *template_fn, char *fn) ;

/*
	entry points
*/

void main(int argc, char *argv[])
{
	char hfn[MKCMODMAX_FILENAMELEN+1] ;
	char cfn[MKCMODMAX_FILENAMELEN+1] ;
	int argidx, cppflag ;

	argidx= 1 ;
	cppflag= 0 ;
	if (strcmp(argv[argidx], "-cpp") == 0) {
		argidx++ ;
		cppflag= 1 ;
	}

	if (argc< (argidx+1)) {
		fprintf(
			stderr,
			"mkcmod: requires argument: [-cpp] module_name\n"
		) ;
		exit(1) ;
	}

	strcpy(modname_n, argv[argidx]) ;
	copytrans2lower(modname_l, modname_n) ;
	copytrans2upper(modname_u, modname_n) ;

	sprintf(hfn, "%s.h", modname_l) ;
	sprintf(cfn, cppflag? "%s.cpp": "%s.c", modname_l) ;

	if (cppflag) {
		strcat(ctemplate_fn, "pp") ;
		strcat(htemplate_fn, "pp") ;
	}

	if (argc> (argidx+1)) {
		strcat(ctemplate_fn, argv[argidx+1]) ;
		strcat(htemplate_fn, argv[argidx+1]) ;
	}

	if (!fileexists(ctemplate_fn) || !fileexists(htemplate_fn)) {
		fprintf(stderr, "mkcmod: template file(s) not found\n") ;
		exit(2) ;
	}

	if (fileexists(hfn) || fileexists(cfn)) {
		fprintf(stderr, "error: file(s) already exist\n") ;
		exit(3) ;
	}

	makefile(htemplate_fn, hfn) ;
	makefile(ctemplate_fn, cfn) ;

	dodir(modname_l) ;

	exit(0) ;
}

/*
	internal functions
*/

static void makefile(char *template_fn, char *fn)
{
	transfile("_@n_", modname_n, template_fn, tempfile_fn) ;
	transfile("_@l_", modname_l, tempfile_fn, tempfile_fn2) ;
	transfile("_@u_", modname_u, tempfile_fn2, fn) ;
	unlink(tempfile_fn2) ;
	unlink(tempfile_fn) ;
}

static void transfile(char *from, char *to, char *ifn, char *ofn)
{
	char syscmd[256] ;

	sprintf(
		syscmd,
		MKCMODCMD_SCHANGE " \"%s\" \"%s\" < %s > %s",
		from, to, ifn, ofn
	) ;
	if (isdebug) {
		printf("%s\n", syscmd) ;
	}
	system(syscmd) ;
}

static void copytrans2upper(char *dest, char *src)
{
	while(*src) {
		if (islower(*src)) (*dest)= toupper(*src) ;
		else		   (*dest)= (*src) ;
		src++ ;
		dest++ ;
	}
	(*dest)= '\0' ;
}	

static void copytrans2lower(char *dest, char *src)
{
	while(*src) {
		if (isupper(*src)) (*dest)= tolower(*src) ;
		else		   (*dest)= (*src) ;
		src++ ;
		dest++ ;
	}
	(*dest)= '\0' ;
}	

static void dodir(char *modname)
{
	char syscmd[256] ;

	sprintf(syscmd, MKCMODCMD_DIR, modname) ;
	system(syscmd) ;	
}

static int fileexists(char *fn)
{
	FILE *fp ;

	fp= fopen(fn, "r") ;
	if (fp == (FILE *) NULL) return(0) ;
	fclose(fp) ;
	return(1) ;
}

/* end of mkcmod.c */
