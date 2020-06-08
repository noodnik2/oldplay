/*

	slfmt				format a line of data on the screen
					Copyright (C) 1990, by ==C=R=C==

		slfmt(i, buf, len, indent, hexmode, binmode, ebcmode) ;

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"ebcdic.h"
#include	"browse.h"

extern struct br_scn brs ;
void chfmt() ;

/*
	format a line of data on the screen.  returns the number
	of rows on the screen used to display the line.
*/
int slfmt(i, s, x, in, hexmode, binmode, ebcmode)
int i, x, in, hexmode, binmode, ebcmode ;
unsigned char *s; 
{
	register int j ;
	unsigned char *p ;

	x-= in ;	
	s+= in ;
	p= (unsigned char *)SCN_FL(i) ;

	/*
		p -> screen output line, length is S_WIDTH bytes
		s -> file input line, contains file data
		x := length of 's' (may be > S_WIDTH)
	*/	
					/* hex display modes: handle */		
	if (hexmode != 0) {
		if (x> (S_WIDTH/2)) x= S_WIDTH/2 ;
		for (j= 0; j< x; j++, p+= 2) { 
			sprintf((char *)p, "%02X", (int)s[j]) ;
		}
		if (hexmode==(-1)) return(1) ;		/* hex only */
		if ((i+1)>= F_LINES) return(1) ;
		p= (unsigned char *)SCN_FL(i+1) ;
		for (j= 0; j< x; j++, p+= 2) { 
			chfmt(p, (int)s[j], 2, ebcmode);
		}
		p[0]= '\0' ;
		return(2) ;				/* hex/char */
	}		
						/* other display modes */
	if (x> S_WIDTH) x= S_WIDTH ;

#if 0
	if (!binmode) {					/* ascii */
		if (x> 0) memcpy(p, s, x) ;
		p[x]= '\0' ;
		return(1) ;
	}
#endif

	for (j= 0; j< x; j++, p++) {			/* binary */
		chfmt(p, (int)s[j], 1, ebcmode);
	}
	p[0]= '\0' ;
	return(1) ;
}

static int cstep[]= {   0,  32,  64,  96,  128,  160,  192,  224, 256 } ;
static int cpref[]= { '#', '^', '@', '*',  '~',  '&',  '%',  '!', '$' } ;
static int cprng[]= { 32, 126 } ; 

#define		NCPREF	(sizeof(cpref)/sizeof(cpref[0]))

static int rcpv(c, oc, l, ebc)
int c, l ;
char *oc ;
{
	register int i ;

	if (ebc) {
		if (ebcisok(c)) {
			if (ebcistxt(c)) {
				c= ebc2asc(c) ;
			}
		}
	}

	if ((c>= cprng[0]) && (c<= cprng[1])) {	    /* return if in range */
		if (l> 1) *oc++= ' ' ;
		*oc= (char) c;
		return(0) ;
	}
	for (i= 1; i< NCPREF; i++) {
		if (c< cstep[i]) {
			oc[0]= cpref[i] ;
			if (l> 1) oc[1]= ((c-cstep[i-1])+'A'-1) ;
			return(0) ;
		}
	}
	return(-1) ;
}

/*

	chfmt(s, c, l, ebcmode);

	formats character 'c' into string 's' for a length of 'l'
	as printable ascii string.  uses ^ to indicate control
	characters, ~ to indicate characters in the range 127-191,
	and * to indicate characters 192 and above.
	
*/
static void chfmt(s, c, l, ebc)
unsigned char *s ;
int c, l, ebc;
{
	rcpv(c, s, l, ebc) ;	
}
