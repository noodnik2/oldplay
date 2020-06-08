/*

	Program showing how to put keyboard into mode
	where you can read single character at a time.

*/

#include	<stdio.h>
#include	<termios.h>
#include	<unistd.h>

static unsigned char buf[10] ;
int iPos= 0 ;
int iLen= 0 ;

static int kbhit()
{
	if (iPos == iLen) {
		iLen= read(0, buf, sizeof(buf)) ;
		if (iLen< 0) iLen= 0 ;
		else if (iLen> 1) {
			printf("read = %d\n", iLen) ;
		}
		iPos= 0 ;
	}
	return(iLen) ;
}

static int getch()
{
	kbhit() ;
	if (iPos == iLen) return(0) ;
	return((int) buf[iPos++]) ;
}

int main()
{
	struct termios tio0, tio ;
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
		printf("waiting (Ctrl-C to quit).\n") ;
#else
		printf(".") ;
#endif
		if (kbhit()) {
			printf("reading.\n") ;
			c= getch() ;
			if (c == 3) break ;
			printf("c=%d\n", c) ;
		}
	}
	printf("byebye\n") ;
	
	tcdrain(0) ;
	if (tcsetattr(0, TCSANOW, &tio0)< 0) return(2) ;

	return(0) ;
}

