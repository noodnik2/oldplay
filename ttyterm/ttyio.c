/*

	ttyio.c:	ttyterm-- terminal driver interface
	=======

	t_open()	opens terminal driver

	t_start()	starts terminal driver: turns it "on".  issues
			tinit() call to catch interrupt while terminal
			is "on".  Use the other t_*() functions provided
			(below) while in terminal on state.

	t_speed(s)	gets or sets speed of remote terminal node;
			only standard baud rates are supported.

	t_stop()	stops terminal driver: turns it "off".  while
			terminal is "off", interrupt is caught by this
			routine, not tinit(); stopflag is set to 1 when
			an interrupt is detected.

	t_close()	closes terminal driver

	t_name()	returns name(s) of local or remote tty device

	t_wait()	waits for I/O on any of terminal devices


	t_*() functions:
	-----

	t_inc()		returns -1 if no input character ready, or
			returns inputted character.
	
	t_outc(ch)	sends ch to remote terminal.
	t_outs(s, l)	sends "s" for length "l" to remote terminal.
	
	t_kbdc()	returns -1 if no keyboard character ready,
			or returns struck key character.

	t_echoc(ch)	echoes character "ch" to the terminal console.


	========
	requires services of:
	
	uterm.c:	utermo(), utermc(),
			utermout(), utermin()
			utermset(), utermcso()

	?:		int stopflag

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<signal.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"uterm.h"
#include	"utset.h"
#include	"ttyio.h"

static struct uterm cltty, crtty ;
static struct utset clset, crset ;
static int t_cbaud= -1 ;

void t_outc() ;
void setflag() ;

extern int stopflag ;

/*
	t_wait:		wait for I/O event
*/
void t_wait()
{
	struct uterm *ttyl[2] ;
	
	ttyl[0]= &cltty ;
	ttyl[1]= &crtty ;	
	utermwait(ttyl, 2) ;
}

/*
	t_open:		open a terminal

	read the characteristics of the local console,
	save them in the control table.
	
	open a terminal file for the local terminal node.
	open a terminal file for the remote terminal mode.
	restore the characteristics of the local console.

	you must call t_start() before using the terminal.

*/
int t_open(fn0, fn1, opts)
char *fn0, *fn1 ;
int opts ;
{
	utermo_setopt((opts & TOO_DEBUG)? UTOO_DEBUG: UTOO_NONE) ;
	if (utermo(
		UT_CONSOLE, 
		(opts & TOO_OPENONCE)? NULL: UT_CONSOLE,
		&cltty,
		&clset
	)) {
		clperror("cannot open local console") ;
		return(1) ;
	}

	if (utermo(fn0, fn1, &crtty, &crset)) {
		utermc(&cltty) ;
		clperror("cannot open remote terminal") ;
		return(3) ;
	}
	t_cbaud= crtty.inp_baud ;

	signal(SIGINT, setflag) ;
	return(0) ;
}

int t_speed(baud)
int baud ;
{
	if (baud != 0) t_cbaud= baud ;
	return(t_cbaud) ;
}

int t_name(dir, name0_p, name1_p)
int dir ;
char **name0_p, **name1_p ;
{
	if (dir & T_REMOTE) {
		*name0_p= crtty.name0 ;
		*name1_p= crtty.name1 ;
	}
	else if (dir & T_LOCAL) {
		*name0_p= cltty.name0 ;
		*name1_p= cltty.name1 ;
	}
	else {
		return(1) ;
	}
	return(0) ;
}

/*
	t_stop:		stops the terminal

		restores the terminal files characteristics
		traps the interrupt vector to local routine
*/
void t_stop(dir)
int dir ;
{
	if (dir & T_LOCAL) utermset(&cltty, &clset) ;
	if (dir & T_REMOTE) utermset(&crtty, &crset) ;
	signal(SIGINT, setflag) ;
}

/*
	t_start:	starts the terminal

		modifies the terminal file's characteristics to be
		compatible with the character services required.

		modifies the local console to support transparency
		and immediate keyboard key availability.

		adds options specified in "yesflags" and deletes
		options specified in "noflags".
*/ 
void t_start(dir, yesflags, noflags)
int dir, yesflags, noflags ;
{
	struct utcopt utco ;

	memset((char *)&utco, '\0', sizeof(utco)) ;
	if (yesflags & TM_TANDEM) utco.ixon= utco.ixoff= 1 ;
	if (noflags & TM_TANDEM) utco.ixon= utco.ixoff= -1 ;
	if (yesflags & TM_OPOST) utco.opost= 1 ;
	if (noflags & TM_OPOST) utco.opost= -1 ;
	utco.icrnl= utco.echo= utco.isig= utco.icanon= -1 ;
	utco.cread= 1 ;

	if (dir & T_LOCAL) utermcso(&cltty, &utco, -1) ;
	if (dir & T_REMOTE) utermcso(&crtty, &utco, t_cbaud) ;

	signal(SIGINT, setflag) ;
}

/*
	t_close		close the terminal device files

		close both local and remote terminal files.
*/
void t_close()
{
	utermc(&cltty) ;
	utermc(&crtty) ;
}

/*
	t_outc:		output a character to the remote node

*/
void t_outc(c)
int c ;
{
	utermout(&crtty, (char)c) ;
}

/*
	t_inc:		input a character from the remote node

*/
int t_inc()
{
	return(utermin(&crtty)) ;
}

/*
	t_kbdc:		read a character from the terminal keyboard

*/
int t_kbdc()
{
	return(utermin(&cltty)) ;
}

/*
	t_echoc		echo a character to the terminal console

*/
void t_echoc(c)
int c ;
{
	utermout(&cltty, (char)c) ;
}

/*
	t_outs		output a string to the remote node

*/
void t_outs(s, l)
char *s ;
int l ;
{
	utermouts(&crtty, s, l) ;
}

static void setflag(i)
int i ;
{
	stopflag= 1 ;
	signal(SIGINT, setflag) ;
}
