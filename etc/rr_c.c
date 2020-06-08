#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>

static int intflag= 0 ;

static void setflag(i)
int i ;
{
	intflag= 1 ;
	signal(SIGINT, setflag) ;
}

void main()
{
	char c ;

	initscr() ; nonl() ; cbreak() ; noecho() ;
	keypad(stdscr, TRUE) ;
	signal(SIGINT, setflag) ;
	while(intflag == 0) {
		c= getch() ;
		addch(c) ;
	}
	endwin() ;
	printf("intflag=%d\n", intflag) ;
}

