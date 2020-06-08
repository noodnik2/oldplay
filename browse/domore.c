/*

	domore			do --more-- display, request
				Copyright (C) 1990-1992, by Marty Ross

	domore()

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_scn brs ;
extern struct br_arg bra ;
extern struct br_opt bro ;

/*
	domore()

		simulate a "more" state for applications that have
		messed up the screen.  we must re-draw it.
*/
void domore()
{
#if	OPSYS==OPUNIX
	addstr("\r--more--"); refresh() ;
	getch() ;
	addstr("\r        "); refresh() ;
#else
	extern int LINES ;

	move(LINES-1, 0) ;	addstr("--more--"); refresh() ;
	getch() ;
	move(LINES-1, 0) ;	addstr("        ") ;
 	move(LINES-1, 0) ;	refresh() ;
#endif
}
