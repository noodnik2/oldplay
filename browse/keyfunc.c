/*

	keyfunc				Handle browse keyboard functions
					Copyright (C) 1990, by ==C=R=C==

	   int keyfunc(&KEY, &LCP) ;

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

void showcln() ;

/*
	this routine handles the "regular" screen cursor movement
	functions.  if it does anything, it returns a 1, else a 0.
*/
int keyfunc(keyp)
int *keyp ;
{
	int fline ;

	fline= (CY-T_LINES) ;

	switch(KEY) {

		case KEYEXIT:
			break;

		case KEYREDRAW:	 
			clear() ;
			redraw() ;
			break;
			
		case KEYWHERE:
			if ((CY>CMD_YPOS) && ((fline>=0) && (fline<F_LINES))) {
				if (fline!=RLINE) {
					if (fline>RLINE) fline-- ;
					showcln(fline, CX);
					break;
				}
			}
			beep() ;
			break;

		case KEYHERE:
			if ((CY>CMD_YPOS) && ((fline>=0) && (fline<F_LINES))) {
				if (fline!=RLINE) {
					if (fline>RLINE) fline-- ;
					NEWTOPLINE+= fline ;
					CY= CMD_YPOS; CX= CMD_XPOS ;
					break;
				}
			}
			beep() ;
			break;

		case KEYPGDN:
			NEWTOPLINE+= (long) NUMLINE ;
			break;

		case KEYPGUP:
			NEWTOPLINE-= (long) F_LINES ;
			if (NEWTOPLINE< 0L) NEWTOPLINE= 0L ;
			break;

		case KEYENTER:
			if (CY< S_LENGTH) CY++ ;
			else CY= 0 ;
			CX= 0 ;
			break ;

		case KEYUP:
			if (CY> 0) CY--;
			else CY= S_LENGTH-1 ;
			break ;
		
		case KEYDN:
			if (CY< S_LENGTH) CY++ ;
			else CY= 0 ;
			break ;

		case KEYBS:
			if (CX> 0) CX-- ;
			else CX= S_WIDTH- 1 ;
			break;

		case KEYFS:
			if ((CX+1)< S_WIDTH) CX++ ;
			else CX= 0;
			break;

		default:
			return(0);
			break;
	}
	return(1) ;
}

/*
	end of entry points
	start of static subroutines
*/

static void showcln(i, j)
int i, j ;
{
	char buf[S_WIDTH+1] ;

	if (HEXMODE) {
		j/= 2 ;
		if (HEXMODE==1) {
			i/= 2 ;
		}
	}
	sprintf(buf, "* %d %d", TOPLINE+i+1, INDENT+j+1) ;
	move(CMD_YPOS, CMD_XPOS);
	clrtoeol() ;
	addstr(buf) ;
}
