#include	<stdio.h>
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
	char buf[120] ;

	signal(SIGINT, setflag) ;
	while(intflag == 0) {
		gets(buf) ;
	}
	printf("intflag=%d\n", intflag) ;
}

