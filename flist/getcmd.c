/*

	getcmd			get command line using curses
				Copyright (C) 1990-1992, by Marty Ross


*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

extern struct fl_arg fla ;

int getcmd(cmd)
char *cmd;
{
	register int i;
	int j, ir, c, cy, cx;

	cy= fla.cy; 
	cx= 2;
	ir= i= 0;

	updscn() ;
	while(i< CMDSIZE) {
		if ((c= wgetch(stdscr)) == EOF) return(-1);
		switch(c) {

			case 27:
				if (i==0) {
					redraw(0);
					updscn();
					return(0);
					break;
				}
				ir= i= 0; cx= 2;
				absmove(cy, cx);
				clrtoeol() ;
				updscn();
				break;

			case '\003':
				cmd[0]= (char)c ;
				cmd[1]= '\0';
				return(1);
				break;

			case A_CTL('F'):
				if ((ir> 0) && (i< ir)) {
					for (j= i; j< ir; j++) {
						cmd[j]= cmd[j+1];
					}
					ir--;
					delch();
					updscn();
				}
				break;
	
			case A_CTL('L'):
				if (i< ir) {
					i++;
					absmove(cy, ++cx) ;
					updscn();
					break;
				}
				beep();
				break;

			case '\b':
				if (i> 0) {	
					i-- ;
					absmove(cy, --cx);	
					updscn();
					break;
				}
				beep();
				break;

			case '\r':
				cmd[ir]= '\0' ;
				return(ir) ;
				break;

			default:
				cmd[i++]= c ;
				if (i> ir) ir= i ;
				cx++ ;
				addch(c);
				updscn();
				break;
		}
	}
	cmd[ir]= '\0';			/* superflous code */
	return(ir);
}
