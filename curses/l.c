#include	<curses.h>

main()
{
	initscr() ;
	addstr("hello, world") ;
	getch() ;
	endwin() ;
}

