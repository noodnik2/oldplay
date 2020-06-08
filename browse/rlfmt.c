/*

	rlfmt				format a rule line on the screen
					Copyright (C) 1990, by ==C=R=C==

		rlfmt(i, indent, hexmode) ;

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

void mk10str() ;

/*
	format a ruler on the screen.  returns the number
	of rows on the screen used to display the ruler.
*/
int rlfmt(i, indent, hexmode)
int i, indent, hexmode ;
{
	int d, j, k, l, r ;
	char *p, buf[(S_WIDTH+20)+1] ;

	if ((i< 0) || (i>= F_LINES)) return(0) ;

	l= indent / 10 ;
	r= indent % 10 ;

	if (hexmode) { d= 20; r*= 2; }
	else { d= 10; }
		
	k= (S_WIDTH/d) + 1 ;

	p= buf ;
	for (j= 0; j< k; j++, p+= d) {
		mk10str(p, ((j+l+1)*10), d);
	}

	p= SCN_FL(i) ;
	memcpy(p, buf+r, S_WIDTH);
	p[S_WIDTH]= '\0'; 
	
	return(1) ;
}

static void mk10str(s, n, d)
char *s;
int n, d;
{
	register int i;
	int l ;
	char buf[41] ;

	sprintf(buf, "%d", n);
	i= 0 ;
	l= d - strlen(buf) ;
	while(i< l) s[i++]= '.' ;
	strcpy(s+i, buf) ;
	s[(d/2)-1]= '+' ;
}
