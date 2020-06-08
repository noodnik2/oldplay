/*

	keybrowse			Handle browse keyboard functions
					Copyright (C) 1990, by ==C=R=C==

	   void keybrowse(&KEY, &LCP);

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

/*
	this routine is called whenever there is a key pressed
	and the cursor is not in the input area (command line).
*/
void keybrowse(keyp, lcpp)
int *keyp, *lcpp ;
{

	switch(KEY) {

		case KEYCMDMODE:
			CX= CMD_XPOS+LCP; CY= CMD_YPOS ;
			break;

		default:
			if (keyfunc(keyp)) break ;
			beep();
			break ;
	}
}
