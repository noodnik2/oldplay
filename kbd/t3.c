/*

	Program showing how to put keyboard into mode
	where you can read single character at a time.

*/

#include	<stdio.h>
#include	<termios.h>
#include	<unistd.h>

static unsigned char buf[10] ;
static int iPos= 0 ;
static int iLen= 0 ;
static struct termios tio0, tio ;

static int kbhit(int iTimeout)
{
	struct termios tio1 ;

	if (iPos == iLen) {

		tio1= tio ;
		if (iTimeout == -1) {
			tio.c_cc[VMIN]= 1 ;
			tio.c_cc[VTIME]= 0 ;
		}
		else {
			tio.c_cc[VMIN]= 0 ;
			tio.c_cc[VTIME]= iTimeout ;
		}

		tcdrain(0) ;
		if (tcsetattr(0, TCSANOW, &tio)< 0) {
			printf("can't do it\n") ;
			return(0) ;
		}

		iLen= read(0, buf, sizeof(buf)) ;
		if (iLen< 0) iLen= 0 ;
		iPos= 0 ;

		tcdrain(0) ;
		if (tcsetattr(0, TCSANOW, &tio1)< 0) {
			printf("can't do it again\n") ;
			return(0) ;
		}
	}

	return((iPos == iLen)? 0: 1) ;
}

static int getch()
{
	kbhit(-1) ;
	if (iPos == iLen) return(0) ;
	return((int) buf[iPos++]) ;
}

int main()
{
	int c ;
	
	if (tcgetattr(0, &tio0)< 0) return(1) ;
	tio= tio0 ;

	cfmakeraw(&tio) ;
	tio.c_oflag|= OPOST ;
	tio.c_cc[VMIN]= 0 ;
	tio.c_cc[VTIME]= 10 ;

	tcdrain(0) ;
	if (tcsetattr(0, TCSANOW, &tio)< 0) return(2) ;

	printf("waiting (Ctrl-C to quit).\n") ;
	while(1) {
#if 0
#if 0
		printf("waiting (Ctrl-C to quit).\n") ;
#else
		printf(".") ; fflush(stdout) ;
#endif
		if (kbhit(25)) {
			printf("reading.\n") ;
			c= getch() ;
			if (c == 3) break ;
			printf("c=%d\n", c) ;
		}
	}
#else
		printf("reading.\n") ;
		c= getch() ;
		if (c == 3) break ;
		printf("c=%d\n", c) ;
	}
#endif
	printf("byebye\n") ;
	
	tcdrain(0) ;
	if (tcsetattr(0, TCSANOW, &tio0)< 0) return(2) ;

	return(0) ;
}

