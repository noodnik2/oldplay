/*

	dobrowse			Perform browse function
					Copyright (C) 1990, by ==C=R=C==

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

void browseproc() ;

int dobrowse()
{
	char emsg[120] ;
	int rc ;

	if (bro.o_debug) {
		sprintf(emsg, "dobrowse on '%s'\n", FILENAME);
		doerror(0, emsg) ;
	}
	
	if (rc= rrdi(FILENAME)) {
		sprintf(emsg, "can't open: %s", FILENAME);
		doerror(rc, emsg) ;
		return(rc) ;
	}

	browseproc() ;

	rrdf() ;
	return(0) ;
}
		
static void browseproc()
{
	initscr() ;
#if 1
	scnmode(1);
#endif
	redraw();
	dokeys();
#if 0
	scnmode(0);
#endif
	endwin() ;
}
