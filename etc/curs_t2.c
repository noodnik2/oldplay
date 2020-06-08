#include	<curses.h>
#include	<locale.h>

static void doit() ;

void main()
{
	int c, maxy, maxx ;
	WINDOW *win ;

	if (setlocale(LC_CTYPE, "german") == NULL) {
		printf("setlocale failed\n") ;
		getchar() ;
	}
	win= initscr() ;

	maxy= win->_maxy ;
	maxx= win->_maxx ;
	doit() ;
	endwin() ;

	printf("maxy, maxx= %d,%d\n", maxy, maxx) ;
	printf("win    is %u\n", (unsigned)win) ;
	printf("stdscr is %u\n", (unsigned)stdscr) ;
	printf("curscr is %u\n", (unsigned)curscr) ;
}


#if 1
static unsigned char str[]= { 0xED, 0xE5, 0xEC, 0xF9 } ;
#else
static unsigned char str[]= { 0x7D, 0x7E, 0x7F, 0x7F } ;
#endif
static int l_str= (sizeof(str)/sizeof(str[0])) ;

static unsigned char str2[]= { 'o', 'l', 'l', 'e', 'H' } ;
static int l_str2= (sizeof(str2)/sizeof(str2[0])) ;

static void doit()
{
	int i ;

#if 1
	move(1, 1) ; for (i= 0; i< l_str; i++) addch(str[i]) ;
#endif
	move(2, 2) ; for (i= 0; i< l_str2; i++) addch(str2[i]) ;
	refresh() ;
	getch() ;
}
