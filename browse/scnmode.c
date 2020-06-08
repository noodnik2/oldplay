/*

	scnmode.c		curses/screen interface routines
				Copyright (C) 1990, by ==C=R=C==

	scnmode()

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"

/*

	scnmode(onoff)

		if onoff!=0, then scnmode() sets the screen into
		"application" mode, else the screen is set into
		"system" mode with echo, noraw, and nl set.
*/
void scnmode(onoff)
int onoff;
{
	if (onoff)	{ fixterm(); raw(); noecho(); nonl(); }
	else 		{ refresh(); nl(); echo(); noraw(); resetterm(); }
}

