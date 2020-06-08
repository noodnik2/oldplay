#include	<stdio.h>
#include	<curses.h>

void main(argc, argv)
int argc ;
char *argv[];
{
	int c ;

	initscr() ;

	raw() ;
	noecho() ;	
	nonl() ;

	while((c= getch()) != 27) {
		if (c==13) addstr("<CR>") ;
		else addch(c) ;
		refresh() ;
	}

	endwin() ;

}

