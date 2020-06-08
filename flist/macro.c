/*

	macro.c				Keyboard macro facility
					Copyright (C) 1990, by ==C=R=C==



*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

struct fl_arg fla ;

void initmac(onoff)
int onoff ;
{
	if (fla.usemac) {
		rddstrf() ;
	}
	fla.usemac= 0;
	fla.macptr= 0;
	if (onoff) {
		if (fla.macfile[0] != '\0') {
			if (rddstri(fla.macfile, FL_MACMAGIC)) return;
			fla.usemac= 1;
		}
	}
}

int inmackey()
{
	int c ;

	if (fla.macptr> 0) {
		if ((c= fla.macbuf[fla.macptr++]) != '\0') {
			return(c) ;
		}
		fla.macptr= 0 ;
	}	
		
	if ((c= wgetch(stdscr)) == EOF) return(c) ;
	if (fla.usemac) {
		if (c< ' ') {
			if (rddstr(0, (c-1), fla.macbuf, CMDSIZE)) {
				initmac(0);
			}
			else {
				c= fla.macbuf[fla.macptr++] ;
			}
		}
	}
	return(c);
}
