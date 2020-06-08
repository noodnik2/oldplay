/*

	dodraw				Draw the screen from current file pos
					Copyright (C) 1990, by ==C=R=C==
	

		rc= dodraw(struct br_draw *brp) ;

		rc:
			0 - screen drawn properly
			1 - screen cannot be drawn
			
*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"browse.h"

extern char pver[] ;
extern struct br_arg bra ;
extern struct br_opt bro ;
extern struct br_scn brs ;

void ititle(), dsl() ;

int dodraw(brp)
struct br_draw *brp ;
{
	int i, j, k ;
	
						/* draw our internal screen */
	memset(&brs, ' ', sizeof(brs)) ;
	if ((k= ilines(brp))<= 0) return(1) ;		/* no lines to draw */
	ititle(brp) ;
							/* xfer scn->curses */
	j= 0;
	for (i= 0; i< T_LINES; i++) dsl(SCN_TL(i), j++) ;
	for (i= 0; i< F_LINES; i++) dsl(SCN_FL(i), j++) ;
#if	B_LINES != 0
	for (i= 0; i< B_LINES; i++) dsl(SCN_BL(i), j++) ;
#endif

#if 0
	if (typeahead(0)) bra.redraw= 1 ;
	else {
		refresh() ;
		bra.redraw= 0 ;
	}
#endif

#if 1
	refresh() ;
#endif

	return(0) ;
}

/*

	start of static subroutines

*/

static void dsl(line, j)
register char *line;
int j ;
{
	line[S_WIDTH]= '\0' ;
	strim(line) ;
	move(j, 0);
	clrtoeol() ;

	while(*line) {	
		switch(*line) {
			case '\n':  case '\r':	break;
			default: addch(*line);	break;
		}
		line++ ;
	}
}

static void ititle(brp)
struct br_draw *brp ;
{
	char tmsg[S_WIDTH+1], cmsg[S_WIDTH+1] ;
	int indent ;
	long topline ;

	indent= brp->indent ;
	topline= brp->topline ;

	tmsg[0]= '\0'; cmsg[0]= '\0' ;
	if (indent> 0) {
		sprintf(cmsg, " : col=%d", indent+1) ;
	}
	sprintf(SCN_TL(0), "browse %s: %s : line=%ld%s : %ld bytes%s",
		pver, FILENAME, (topline+1L), cmsg, FILESIZE, tmsg );
	strcpy(SCN_TL(1), "===> ") ;
}

/*
	paint the entire screen, according to the
	current drawing mode.

	added support 2-7-90: floating rule line: rline

	returns number of lines drawn on screen, or -1
	if error.

*/
static int ilines(brp)
struct br_draw *brp ;
{
	register int j ;
	int i, k, x, y;
	unsigned char rbuf[RECSIZEMAX+1] ;
	long topline ;
	int indent, hexmode, tabmode, rline ;
	int ebcmode, binmode, recsize, numline ;

	i= k= y= 0;

	topline= brp->topline ;
	indent=  brp->indent ;
	rline=   brp->rline ;
	hexmode= brp->hexmode ;
	binmode= brp->binmode ;
	ebcmode= brp->ebcmode ;
	tabmode= brp->tabmode ;
	recsize= brp->recsize ;
	numline= brp->numline ;

	while(i< numline) {
		if ((y!=0) || (i<rline)) {	
			memset((char *)rbuf, '\0', recsize) ;
			if (rrd(topline+k, rbuf, recsize, &x)) break;
			if (tabmode) xpand(rbuf, &x, sizeof(rbuf)) ;
			k++ ;
#if 0
			for (j= 0; (j< indent) && (j< x); j++) {
				if (rbuf[j] == '\0') break ;
			}
#else
			j= indent ;
#endif
			i+= slfmt(i, rbuf, x, j, hexmode, binmode, ebcmode);
			continue ;
		}
		i+= rlfmt(i, indent, hexmode) ;
		y++ ;
	}
	return(k) ;
}

/*
	rc= xpand(buf, lenp, maxlen) ;

	expands a line that contains tabs.  replaces tab characters
	with a number of blanks, from 1 to 7.

	returns length of expanded string in *lenp.
	returns nonzero value if output buffer overflow error.
*/
static int xpand(buf, lenp, maxlen)
unsigned char *buf ;
int *lenp, maxlen ;
{
	int j, k, l, t ;
	unsigned char c, xpbuf[RECSIZEMAX+1] ;

	j= k= t= 0;
	l= (*lenp) ;
	while((k< l) && (j< RECSIZEMAX)) {
		if (t> 0) { c= ' '; t--; }
		else c= buf[k++] ;
		if (c=='\t') t= (8-(j&7)) ;
		else xpbuf[j++]= c ;
	}	
	if (j>= maxlen) j= maxlen- 1;
	xpbuf[j++]= '\0' ;
	memcpy(buf, xpbuf, j) ;
	(*lenp)= j-1 ;
	return(j==maxlen) ;
}
