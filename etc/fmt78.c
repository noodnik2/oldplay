#include	<stdio.h>

#define		LINELENGTH	78

#define		write			_write

void main()
{
	int i, c, infile ;
	char inbuf[LINELENGTH+1] ;

	infile= 1 ;
	while(infile) {
		for (i= 0; i< LINELENGTH; i++) {
			c= getchar_expandtabs() ;
			if (c == EOF) { infile= 0 ; break ; }
			if (c == '\n') break ;
			inbuf[i]= c ;
		}
		write(1, inbuf, i) ;
		write(1, "\n", 1) ;
	}
}

static int getchar_expandtabs()
{
	static int colpos= 0 ;
	static int tabcount= 0 ;
	int c ;

	if (tabcount> 0) {
		tabcount-- ;
		colpos++ ;
		return(' ') ;
	}

	c= getchar() ;
	if (c == '\t') {
		tabcount= (8 - (colpos & 7)) ;
		return(getchar_expandtabs()) ;
	}

	if (c == '\n') colpos= 0 ;
	else	       colpos++ ;

	return(c) ;
}
