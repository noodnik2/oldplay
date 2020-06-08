/*

	terminal driver

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"clgetopt.h"

#include	"term.h"
#include	"ttyio.h"

char *prog_ver= "0.8" ;
struct termopts term_o ;

void tsetxfer() ;

static void fini() ;
static void syntax() ;
static void term_title() ;
static void in_term() ;
static int do_shell() ;
static int rt_syntax() ;

extern char *getenv() ;

/* extern */ int stopflag ;

void main(argc, argv)
int argc;
char *argv[];
{
	int i, rc;
	char *line ;

	i= init(argc, argv) ;

	if (term_o.help) {
		if (term_o.debug) clerror("help requested") ;
		syntax() ;
		exit(1) ;
	}

	if (i< argc) {
		if (i != (argc-1)) {
			if (term_o.debug) clerror("wrong # arguments") ;
			syntax() ;
			exit(1) ;
		}
		line= argv[i] ;
	}
	else {
		if ((line= getenv("TERMTTY")) == (char *)NULL) {
			if (term_o.debug) clerror("TERMTTY not set") ;
			syntax() ;
			exit(1) ;
		}
	}
	term_o.ttyline= line ;

	rc= doit() ;

	fini(rc) ;
}

void tsetxfer(mode)
int mode ;
{
	static char *xf_mode[]= {	/* see XFER_* modes in term.h */
		"standard text file",
		"xmodem text file",
		"xmodem binary file"
	} ;

	term_o.xfer= mode ;
	term_o.xfmode= xf_mode[mode] ;	/* must be kept up2date */

	fprintf(stderr, "--xfer mode: %s\n", term_o.xfmode) ;
}

static int init(argc, argv)
int argc;
char *argv[];
{
	int i;

	clinit(argv);
	term_o.esc= 1 ;			/* ESCAPE now the default */
	term_o.help= 0 ;
	term_o.debug= 0 ;
	term_o.baud= -1 ;		/* use current baud rate */
	term_o.xfer= XFER_TEXT ;

	while((i= clgetopt(argc, argv, "hzbs:xX")) != (-1)) {
		switch(i) {
			case 'h':term_o.help= 1 ;			break;
			case 'z':term_o.debug= 1 ;			break;
			case 'b':term_o.esc= 0 ;			break;
			case 's':term_o.baud= getbaud(cloptarg);	break ;
			case 'x':term_o.xfer= XFER_XMT;			break ;
			case 'X':term_o.xfer= XFER_XMB;			break ;
			default: exit(1) ;				break;
		}
	}

	return(cloptind) ;
}

static int getbaud(str)
char *str ;
{
	int baud ;

	if (sscanf(str, "%d", &baud)==1) {
		switch(baud) {
			case 150: case 300: case 600:
			case 1200: case 2400: case 4800:
			case 9600: case 19200:
					return(baud) ;
		}
	}
	fprintf(stderr, "invalid baud rate: %s\n", str);
	exit(1) ;
	/* notreached */
}

static int doit()
{

	if (t_open(term_o.ttyline)) return(1) ;
	t_speed(term_o.baud) ;

	term_title(1) ;
	tsetxfer(term_o.xfer) ;

	in_term() ;

	STD_TERM_ON() ;
	stopflag= 0;
	while(do_diag()==0) ;
	STD_TERM_OFF() ;

	term_title(0) ;
	t_close() ;

	return(0) ;
}

static int do_diag()
{
	int c, ch ;
	static int yeshesc= 0 ;

	/*
		if not allowing ESCAPE keys, exit if stopflag
	*/
	if (stopflag && !term_o.esc) return(1) ;

	/*
		get character from local keyboard
	*/
	c= t_kbdc() ;

	if (c> 0) {
		ch= (c & 127) ;
		/*
			interpret it if we're looking for local escape keys
		*/
		if (term_o.esc) {		/* if in "ESCAPE" mode */
			if (yeshesc) {
				yeshesc= 0 ;
				if (do_esc(ch)) return(1) ;
			}
			else if (ch==ESC_CHAR) {
				yeshesc= 1 ;
			}
			else t_outc(ch) ;
		}
		else {
			t_outc(ch) ;
		}
	}

	/*
		get a character from remote site, if available
	*/
	c= t_inc() ;

	if (c> 0) {
		ch= (c & 127) ;
		/*
			if NOT interpreting local escape codes,
			allow remote site to abort us with ^C.
		*/
		if (!term_o.esc) {		/* if NOT in "ESCAPE" mode */
			if (ch==3) {		/* other side can abort    */
				t_outc(ch) ;
				return(1) ;
			}
		}

		/*
			echo the incoming character to the local screen
		*/
		t_echoc(ch) ;
	}
	return(0) ;
}

/*
	do_esc(ch)

	interpret the character 'ch' as the final character
	in 2-byte escape sequence.  perform action associated
	with 'ch'.  returns nonzero if action causes terminal
	process to exit.
*/
static int do_esc(ch)
int ch ;
{
	int do_app() ;
	int do_shell() ;
	int tcmd() ;

	switch(ch) {
		case 'x':	return(1) ;			break ;
		case '?':	do_app(rt_syntax) ;		break ;
		case '!':	do_app(do_shell) ;		break ;
		case '%':	if (do_app(tcmd)) return(1) ;	break ;
		case ESC_CHAR:	t_outc(ESC_CHAR) ;		break ;
		default:	t_echoc('\007') ;		break;
	}
	return(0) ;
}

/*
	rc= do_app(app_func) ;

	performs specified application program, then returns
	non-zero if terminal is to be terminated.

	before application is executed, terminal is turned
	off so that the application has access to a clean
	environment (console behaves normally).  terminal
	is restored to on state before returning.
*/
static int do_app(app)
int (*app)() ;
{
	int rc ;

	STD_TERM_OFF() ;
	if ((rc= app())==0) in_term() ;
	STD_TERM_ON() ;
	return(rc) ;
}

/*
	in_term()			print message that in terminal mode

*/
static void in_term()
{
	fprintf(stderr, "-- in term(%s) --\n", term_o.ttyline) ;
}

static void fini(rc)
int rc;
{
	if (term_o.debug) {
		fprintf(stderr, "R(%d);\n", rc) ;
		exit(1) ;
	}
	exit(0) ;
}

static void term_title(arg)
int arg ;
{
	if (arg) {
		fprintf(stderr, "\n--Entering terminal mode:\n") ;
		if (term_o.esc) fprintf(stderr, "--ESC mode; ~? for help\n") ;
		else fprintf(stderr, "--std mode; <BREAK> to exit\n") ;
		return ;
	}
	fprintf(stderr, "\n--Leaving terminal mode; %s\n", t_name(T_REMOTE)) ;
}

static void syntax()
{
	int i ;
	static char *instr[]= {
		"term [-option(s)] <tty-device>",
		"",
		"options:",
		"	-h	prints this",
		"	-b	BREAK signal exits (disables ~)",
		"	-s #	sets speed of terminal: #=baud",
		"	-x	Use XMODEM TEXT file transfer",
		"	-X	Use XMODEM BINARY file transfer",
		"	-z	Enable DEBUG mode",
		"",
		"use term() to become a terminal to another system connected",
		"via a tty device.  the tty device cannot be currently in use",
		"by the local system.  Once you are in terminal mode, use the",
		"escape character ~ to issue commands to term() (e.g. use ~?",
		"to see a list of all the term() escape commands).  The env-",
		"ironment variable \"TERMTTY\" can be set to <tty-device> if",
		"it undesireable to specify <tty-device> on the commandline."
	} ;
#define	NINSTR	(sizeof(instr)/sizeof(instr[0]))

	fprintf(stderr, "term %s: UNIX tty-terminal driver\n\n", prog_ver) ;
	for (i= 0; i< NINSTR; i++) {
		fprintf(stderr, "%s\n", instr[i]) ;
	}
	fprintf(stderr, "\n") ;
}

static int rt_syntax()
{
	int i ;
	static char *rt_instr[]= {
		"term() help:",
		"",
		"~?		prints this",
		"~x		exit terminal",
		"~%		enter term() command",
		"~!		enter local shell",
		"",
	} ;
#define	NRTINSTR	(sizeof(rt_instr)/sizeof(rt_instr[0]))

	fprintf(stderr, "\n") ;
	for (i= 0; i< NRTINSTR; i++) {
		fprintf(stderr, "-- %s\n", rt_instr[i]) ;
	}
	return(0) ;
}

static int do_shell()
{
	static char *scmd ;

	if ((scmd= getenv("SHELL")) == (char *)NULL) {
		scmd= "/bin/sh" ;
	}

	fprintf(stderr, "!%s\n--(local shell)--\n", scmd) ;
	system(scmd) ;
	return(0) ;
}
