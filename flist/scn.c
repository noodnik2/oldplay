/*

	scn			curses/screen interface routines
				Copyright (C) 1990-1992, by Marty Ross

	regmove()
	absmove()
	updscn()
	scnmode()
	domore()
	saveline(buf);
	useline(buf);

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"flist.h"

struct fl_line *fl_lofs() ;

extern struct fl_scn fls ;
extern struct fl_arg fla ;
extern struct fl_opt flo ;

void redraw(), regmove(), absmove(), updscn() ;
void scnmode(), domore() ;

/*
	regmove(region, index, position)

		moves unidimensionally within a specified
		region of the window.
*/
void regmove(region, index, position)
int region, index, position; 
{
	switch(region) {
		case 0:
			absmove(index, position);
			break;
		case 1:	
			absmove(T_LINES+index, position);
			break;
		case 2:
			absmove(T_LINES+F_LINES+index, position);
			break;

		case 100:
			absmove(T_LINES+index, MAXWID+position+2);
			break;
	}
}

/*
	absmove(ypos, xpos)
	
		move to a specified position in the window.
		record the coordinates.
*/
void absmove(ypos, xpos)
int ypos, xpos;
{
	fla.cy= ypos;
	fla.cx= xpos;
	move(ypos, xpos);
}

/*
	updscn()

		update the screen to reflect changes
*/
void updscn()
{
	refresh();
}

/*
	scnmode(onoff)

		if onoff!=0, then scnmode() sets the screen into
		"application" mode, else the screen is set into
		"system" mode with echo, noraw, and nl set.
*/
void scnmode(onoff)
int onoff;
{
#if 1
	if (onoff)	{ fixterm(); raw(); noecho(); nonl(); }
	else 		{ nl(); echo(); noraw(); resetterm(); }
#else
	if (onoff)	{ raw(); noecho(); nonl(); }
	else 		{ nl(); echo(); noraw(); }
#endif
	initmac(onoff) ;
}

/*
	domore()

		simulate a "more" state for applications
		that have messed up the screen.  we must
		re-draw it.
*/
void domore()
{
#if	OPSYS==OPUNIX
	addstr("\r--more--"); updscn();
	wgetch(stdscr);	
	addstr("\r        "); updscn();
#endif
#if	OPSYS==OPMSDOS
	extern int LINES ;

	move(LINES-1, 0) ; addstr("--more--"); updscn();
	wgetch(stdscr);	
	move(LINES-1, 0) ; addstr("        "); updscn();
#endif
}

/*
	flserr(no, msg, pri)
	int no, pri;
	char *msg;

	print or kill error in error window

	if no==0, then the return code is not shown
	else, it is displayed in col 1 (following a ^
	symbol which is always in column 0).

	"pri" is the priority at which the message is
	update on the screen.  0 means it is simply
	queued until the next time a 'redraw()' is
	done.  1 means it is redrawn, but must be
	updscn()'d before being displayed.  2 means
	it is drawn and updscn()'d, too.

*/
void flserr(no, msg, pri)
int no, pri;
char *msg;
{
	char *tp, msgbuf[S_WIDTH-8];

	tp= fls.tl[1] ;
	if (msg==NULL) {			/* kill message   */
		if (tp[0]=='^') {		/* only if error  */
			tp[0]= '\0';
			if (pri>0) redraw(0);
			if (pri>1) updscn();
		}
		return;
	}

	memcpy(msgbuf, msg, sizeof(msgbuf));
	msgbuf[sizeof(msgbuf)-1]= '\0' ;
	if (no) sprintf(tp, "^%d: %s", no, msgbuf);
	else strcpy(tp, msgbuf);
	if (pri>0) redraw(0);
	if (pri>1) updscn();
}

#if	COMPILER==SUNC
void beep()
{
}
#endif


/*
	useline(cmdbuf);

	the current command corresponding to the what file
	the cursor is located next to is loaded into cmdbuf.

	the command is prepared for editing (synchronized
	with the screen).

*/
void useline(cmdbuf)
char *cmdbuf;
{
	char *cmd ;
	struct fl_line *flp ;

	CX= 0;
	clrline(CY, flp= CURFLP) ;
	memset(cmdbuf, '\0', CMDSIZE) ;
	if ((cmd= fl_gcmd(flp)) != NULL) {
		strcpy(cmdbuf, cmd);
		addstr(cmdbuf);
		RX= strlen(cmdbuf);
		regmove(100, CY, CX);
	}
}

/*
	saveline(cmdbuf);

	saves the contents of cmdbuf as the current command
	line corrsponding to the file next to the cursor.

	assumes that the screen already indicates this.

*/
void saveline(cmdbuf)
char *cmdbuf;
{
	cmdbuf[RX]= '\0' ;
	fl_scmd(CURFLP, cmdbuf) ;
}
