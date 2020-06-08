#include	<curses.h>

void main()
{
	int c, nc, ncp, sce, maxy, maxx ;
	WINDOW *win ;

	win= initscr() ;
	sce= start_color() ;

	maxy= win->_maxy ;
	maxx= win->_maxx ;
	nc= COLORS ;
	ncp= COLOR_PAIRS ;
	/* c= getch() ; */
	refresh() ;
	endwin() ;

	printf("sce= %d\n", sce) ;
	printf("colors= %d\n", nc) ;
	printf("cpairs= %d\n", ncp) ;
	printf("maxy, maxx= %d,%d\n", maxy, maxx) ;
	printf("win    is %u\n", (unsigned)win) ;
	printf("stdscr is %u\n", (unsigned)stdscr) ;
	printf("curscr is %u\n", (unsigned)curscr) ;

}
