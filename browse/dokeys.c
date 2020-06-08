/*

	dokeys				Handle user keyboard interaction
					Copyright (C) 1990, by ==C=R=C==

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

void dosettle() ;

int dokeys()
{
	char cmd[CMDSIZE+1] ;
	int key, lcp;

	keycmdzap(cmd, &lcp) ;		/* zap the command line */

	memcpy(&NDRAW, &CDRAW, sizeof(NDRAW)) ;
	bra.redraw= 0 ;

	do {

		move(CY, CX); refresh() ;
		if ((key= getch()) == EOF) return(-1) ;

		switch(CY) {
			case CMD_YPOS:	keycmd(&key, &lcp, cmd);	break;
			default:	keybrowse(&key, &lcp);		break;
		}

		if (bra.redraw || memcmp(&CDRAW, &NDRAW, sizeof(CDRAW))) {
			if (dodraw(&NDRAW)) dodraw(&CDRAW) ;
			else memcpy(&CDRAW, &NDRAW, sizeof(CDRAW)) ;
		}	

	} while(key != KEYEXIT) ;	

	move(S_LENGTH-1, 0) ;
	clrtoeol();
	refresh();

	return(0) ;
}
