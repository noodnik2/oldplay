#include	<curses.h>

main()
{
	initscr() ;
	if (start_color() == OK) {
		init_pair(1, COLOR_RED, COLOR_GREEN) ;
		attron(COLOR_PAIR(1)) ;
		addstr("Red on Green") ;
		getch() ;
		sleep(1) ;
	}
	else {
		fprintf(stderr, "sorry, charlie\n") ;
	}
	endwin() ;
}

