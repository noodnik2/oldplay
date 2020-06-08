/*

	setscn			curses/screen interface routines
				Copyright (C) 1990-1992, by Marty Ross

	void setupscn(topline) ;
	void redraw(section) ;

*/

#include	<stdio.h>
#include	<curses.h>

#include	<sys/types.h>
#include	<sys/stat.h>

#if 0
#include	<sys/time.h>
#else
#include	<time.h>
#endif

#include	"compiler.h"
#include	"flist.h"

extern struct fl_scn fls ;
extern struct fl_arg fla ;
extern struct fl_opt flo ;
extern char pver[] ;

struct fl_line *fl_lofs() ;
void clrline() ;

/*

	setupscn(topline)

		setup file window for displaying list
		starting at topline.

		uses variables:

			fla.numfiles		total number of files in list
	
		sets variables:

			fls.numfiles		to number of files in window
			fls.maxwid		to size of longest file
			fls.topline		to passed parameter topline

		sets the terminal into raw, noecho, nolf modes,
		and redraws the screen

*/
void setupscn(topline)
int topline ;
{
	int i, j, l, m, n ;
	struct fl_line *flp ;
	char filemsg[41], sizemsg[41] ;
	long sizewas ;

	memset(fls.fl, '\0', sizeof(fls.fl)) ;
	
	m= 0 ;
	flp= fl_lofs(topline) ;
	for (n= 0; (flp != (struct fl_line *)NULL) && (n< F_LINES); n++) {
		if (flp->line != NULL) {
			if (flp->line[0]) {
				sizewas= flp->stat.size ;
				if (fl_diag(flp)) {
					TOTSIZE-= sizewas ;
					flp->line[0]= '\0' ;
				}
				l= strlen(flp->line) ;
				if (l> m) m= l ;
			}
		}
		flp= flp->next ;
	}

	flp= fl_lofs(topline) ;
	for (i= 0; i< n; i++) {
		l= strlen(flp->line) ;
		for (j= 0; j< l; j++) fls.fl[i][j]= flp->line[j] ;
		flp= flp->next ;
	}
	NUMFILE= n ;
	MAXWID= (m+1) ;
	TOPLINE= topline ;

	if (TOTFILE==1) strcpy(filemsg, ": 1 file") ;
	else		sprintf(filemsg, ": %d/%d files", TOPLINE+1, TOTFILE) ;

	if (TOTSIZE==-1L) sizemsg[0]= '\0' ;
	else		sprintf(sizemsg, ": %ld bytes", TOTSIZE) ;

	sprintf(fls.tl[0], "flist %s%s%s", pver, filemsg, sizemsg) ;

	if (TITLE[0] != '\0') {
		strcat(fls.tl[0], ": ") ;
		strcat(fls.tl[0], TITLE) ;
	}

	if (fls.tl[1][0]!='^') {
		if (TOPLINE> 0) {
			sprintf(fls.tl[1], "--more-- (^U=PgUp)") ;
		}
	}

	fls.bl[0][0]= '\0' ;
	if ((TOPLINE+NUMFILE)< TOTFILE) {
		sprintf(fls.bl[0], "--more-- (^D=PgDn)") ;
	}

	sprintf(fls.bl[1],
	"  [ ^A=Cmd ^C=Quit ^J=Dn ^K=Up Esc=Clr | \"/h\" for help ]  "
	) ;

}

/*
	redraw(section)

		section:
			(-1)	update the screen by clearing, re-drawing.
			(0)	re-draw the top status line(s)
			(1)	re-draw the filenames
			(2)	re-draw the bottom status line(s)
*/
void redraw(sect)
int sect ;
{
	int i, j, l, m ;
	struct fl_line *flp ;
	char *s, *cmd ;
	int ocy, ocx ;
	int xpos ;

	ocy= CY ;
	ocx= CX ;

	if (sect==(-1)) clear() ;

	if (sect< 1) {
		for (i= 0; i< T_LINES; i++) {
			regmove(0, i, 0) ;
			clrtoeol();
			addstr(fls.tl[i]) ;
		}
	}

	if ((sect == 1) || (sect == -1)) {
		flp= fl_lofs(TOPLINE) ;
		for (i= 0; i< NUMFILE; i++) {

			s= fls.fl[i] ;
			l= strlen(s) ;

			xpos= (flo.o_right)? (MAXWID-l): 0 ;
			regmove(1, i, xpos) ;

			if (flo.o_stout) {
				standout() ;
				regmove(1, i, xpos) ;
			}
		
			for (m= 0; m< l; m++) addch(s[m]) ;
			if (flo.o_stout) standend() ;
			regmove(1, i, MAXWID) ;

			if (flp->stat.isdir) addch(PATHSEP_CH) ;
			else if ((flp->stat.size)> 0L) addch('|') ;
			else addch(':') ;

			clrline(i, flp) ;

			if (flp->cmd != NULL) {
				addstr((flp->cmd)->cmdline) ;
			}

			flp= flp->next ;
		}
	}

	if (sect==2 || sect==(-1)) {
		for (i= 0; i< B_LINES; i++) {
			regmove(2, i, 0) ;
			clrtoeol() ;
			addstr(fls.bl[i]) ;
		}
	}

	CY= ocy ;
	CX= ocx ;

	regmove(100, CY, CX) ;
}


/*
	clrline(i, flp)

*/

void clrline(i, flp)
int i;
struct fl_line *flp ;
{
	char numbuf[81] ;

	regmove(100, i, 0);
	clrtoeol() ;
	if (flo.o_stout) standout() ;
	if (flo.o_pdate) {
		if (flp->stat.date> 0L) {
			regmove(1, i, R_MGN-26);
			addstr(ctime(&flp->stat.date)) ;
		}
	}
	else if (flo.o_dsize) {
		regmove(1, i, R_MGN-10) ;
		if (flp->stat.isdir) addstr("<dir>") ;
		else {
			sprintf(numbuf, "%ld", flp->stat.size) ;
			addstr(numbuf) ;
		}
	}
	if (flo.o_stout) standend() ;
	regmove(100, i, 0);
}
