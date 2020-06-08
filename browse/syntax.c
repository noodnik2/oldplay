/*

	syntax			produce help message / syntax diagram
				Copyright (C) 1990-1992, by Marty Ross

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"browse.h"
#include	"srchfile.h"

#if	OPSYS==OPUNIX
static char _hfile[]= "browse.man1" ;
#else
static char _hfile[]= "browse.man" ;
#endif

void lcl_syntax() ;

extern char *getenv() ;

extern struct br_arg bra ;

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
static char *msg[]= {
	"syntax:",
#if	OPSYS==OPMSDOS
	"\tbrowse [-option(s)] fn [fn ...]",
#else
	"\tbrowse [/option(s)] fn [fn ...]",
#endif
	"",
	"options:",
	"\t\tb\t\tbinary file; don't parse records on \\n",
	"\t\te\t\tebcdic file; convert EBCDIC text to ASCII",
	"\t\th\t\tobtain help for browse",
	"\t\tm\t\tcase-insensitive text matching",
	"\t\tr #\t\tset record size to #",
	"\t\ts\t\tset scale on line 0",
	"\t\tt\t\texpand tabs internally",
	"\t\tx\t\thex display mode",
	"\t\tX\t\thex/char display mode",
	"\t\tz\t\tenter debug mode",
	"",
	"   Similar to BROWSE on IBM's VM/SP system, browse is a tool for viewing",
	"files in full-screen mode with optional hexadecimal display formatting.",
} ;
#define	NMSG	(sizeof(msg)/sizeof(msg[0]))

	register int i;

	for (i= 0; i< NMSG; i++) {
		fprintf(stderr, "%s\n", msg[i]);
	}
}
