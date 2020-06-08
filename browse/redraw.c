/*
	redraw.c			Re-draw the current screen
					Copyright (C) 1990, by ==C=R=C==

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;

void redraw()
{
	dodraw(&CDRAW) ;
}

