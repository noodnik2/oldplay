/*

	syntax			produce help message / syntax diagram
				Copyright (C) 1990-1992, by Marty Ross

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"flist.h"
#include	"srchfile.h"

extern struct fl_arg fla ;

#if	OPSYS==OPUNIX
static char _hfile[]= "flist.man1" ;
#else
static char _hfile[]= "flist.man" ;
#endif

static void lcl_syntax() ;

extern char *getenv() ;

void syntax()
{
	char hcmd[256], hfile[132] ;		/* why 256, 132 ?? */

	strcpy(hfile, _hfile) ;
	if (srchfile(hfile, getenv("PATH"), 0)) { /* can find help file? */
		sprintf(hcmd, "%s %s", HELPCMD, hfile) ;
		if (system(hcmd) == 0) return ;	/* yes: can display it? */
	}

	lcl_syntax() ;				/* no: do local syntax */
}

static void lcl_syntax()
{
	register int i;
	static char *msg[]= {
	"syntax:",
#if	OPSYS==OPMSDOS
	"\tflist [/option(s)] { [/d dir] | [file list] }",
#else
	"\tflist [-option(s)] { [-d dir] | [file list] }",
#endif
	"",
	"options:",
	"\t\t-a\t\tshow hidden files",
	"\t\t-b\t\tuse standout mode",
	"\t\t-d dir\t\tspecify target directory",
	"\t\t-l\t\tleft justify names",
	"\t\t-m fn\t\tset macro file name",
	"\t\t-q\t\tdon't echo commands",
	"\t\t-s\t\tshow file sizes",
	"\t\t-t\t\tshow file last update times",
	"\t\t-x\t\tsuppress --more-- for commands",
#if 0
	"\t\t-z\t\tenable debugging messages",
#endif
	"\t\t-I \"c\"\t\tset initial command: \"c\"",
	"\t\t-T \"t\"\t\tset user title: \"t\"",
	"",
	"   Similar to FLIST under IBM's VM/SP system, flist is a tool for managing",
	"lists of files.  Invoked with no arguments flist reads the files in the",
	"current directory.  With the 'd' option, an alternate directory can be read.",
	"A group of files can also be specified by using wildcards.",
} ;
#define	NMSG	(sizeof(msg)/sizeof(msg[0]))

	for (i= 0; i< NMSG; i++) {
		fprintf(stderr, "%s\n", msg[i]);
	}
}
