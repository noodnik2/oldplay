
#include	<stdio.h>
#include	<fcntl.h>

#include	"compiler.h"
#include	"termsig.h"

int stopflag= 0 ;

int termwait(fn)
int (*fn)() ;
{
	fn() ;	
}

int termputch(c)
int c ;
{
	putchar(c) ;
}

int termgetch()
{
	int c ;

	c= getchar() ;
	return(c) ;
}

int termsig(sig)
{
	switch(sig) {
		case TSIG_BREAK:
			break ;
	}
}

int iskbch()
{
	return(0) ;
}

int getkbch()
{
	int c ;

	c= getchar() ;
	return(c) ;
}

