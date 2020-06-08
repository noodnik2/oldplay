/*

	keycmd				Handle browse keyboard commands
					Copyright (C) 1990, by ==C=R=C==

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

void keycmdzap(cmd, lcpp)
char *cmd ;
int *lcpp ;
{
	memset(cmd, ' ', CMDSIZE) ;
	move(CY= CMD_YPOS, CX= CMD_XPOS) ;
	clrtoeol() ;
	LCP= 0 ;
}

/*
	thie routins is called when there is an input character
	and the cursor is in the command input area.
*/
void keycmd(keyp, lcpp, cmd)
int *keyp, *lcpp ;
char *cmd ;
{
	int cp ; 

	switch(KEY) {

		case KEYBS:
			if (CX> CMD_XPOS) CX-- ;
			else beep() ;
			break ;

		case KEYESC:
			keycmdzap(cmd, &LCP) ;
			break ;

		case KEYLF:
		case KEYENTER:
			if (LCP> 0) {
				cmd[LCP]= '\0' ;
				if (docmd(cmd)) KEY= KEYEXIT ;
			}
			keycmdzap(cmd, &LCP) ;
			break ;

		default:
			if (keyfunc(keyp, lcpp)) break;
			cp= (CX-CMD_XPOS) ;
			if ((cp< CMDSIZE) && ((CX+1)< S_WIDTH)) {
				addch(KEY);
				cmd[cp++]= KEY ;
				if (cp> LCP) LCP= cp ;
				CX++ ;
			}
			break;
	}
}
