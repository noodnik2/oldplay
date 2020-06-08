/*	Revision:	3
*/

/*

	flist			Artifact of using VM system.
				Copyright (C) 1990-1992, by Marty Ross

*/

char pver[]= "0.16" ;

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"flist.h"

char *getenv() ;

struct fl_scn fls ;
struct fl_arg fla ;
struct fl_opt flo ;

void main(), signon() ;

extern void syntax() ;

static void setdefaults(), init(), initlist(), fini() ;
static void addopt(), addoptarg() ;

struct fl_line *makelist(), *getlist() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	setdefaults() ;
	init(argc, argv) ;
	doflist() ;
	fini() ;
	exit(0) ;
}

void signon()
{
	fprintf(stderr, "FLIST v%s  File List Management Utility\n", pver) ;
	fprintf(stderr, "Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}

static void setdefaults()
{
	char *p ;

	memset(&FLO, '\0', sizeof(struct fl_opt)) ;
	memset(&FLA, '\0', sizeof(struct fl_arg)) ;
	memset(&FLS, '\0', sizeof(struct fl_scn)) ;

	flo.o_more= flo.o_right=
		flo.o_cmddisp= flo.o_redraw= 1 ;

	strcpy(DIRFN, ".") ;
	strcpy(HELPCMD, "more <") ;

	if ((p= getenv("HOME")) != NULL) {
		strcat(MACFN, p) ;
		strcat(MACFN, PATHSEP_CHA) ;
	}
	strcat(MACFN, FLMAC_EXT) ;
	OPTSTR[0]= '\0' ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int c, n, rc ;

	clinit(argv) ;
	while((c= clgetopt(argc, argv, "abd:hlnm:qrstxzH:I:T:")) != -1) {

		if (c != 'd') addopt(c) ;

		switch(c) {

			case 'a':	flo.o_all= 1 ;			break ;
			case 'b':	flo.o_stout= 1 ;		break ;
			case 'h':	flo.o_help= 1 ;			break ;
			case 'l':	flo.o_right= 0 ;		break ;
			case 'n':	flo.o_more= 0 ;			break ;
			case 'q':	flo.o_cmddisp= 0 ;		break ;
			case 'r':	flo.o_recur= 1 ;		break ;
			case 's':	flo.o_dsize= 1 ;		break ;
			case 't':	flo.o_pdate= 1 ;		break ;
			case 'x':	flo.o_redraw= 0 ;		break ;
			case 'z':	flo.o_debug= 1 ;		break ;
			
			case 'd':	strcpy(DIRFN, cloptarg) ;
				/* NOTE: NO 'addoptarg()' here (see above) */
					break ;

			case 'm':	strcpy(MACFN, cloptarg) ;
					addoptarg(cloptarg) ;
					break ;

			case 'H':	strcpy(HELPCMD, cloptarg) ;
					addoptarg(cloptarg) ;
					break ;

			case 'I':	strcpy(ICMD, cloptarg) ;
					addoptarg(cloptarg) ;
					break ;

			case 'T':	strcpy(TITLE, cloptarg) ;
					addoptarg(cloptarg) ;
					break ;

			default:	exit(1) ;
					break ;
		}
	}

	if (flo.o_help) {
		signon() ;
		syntax() ;
		exit(0) ;
	}

	if ((TOTFILE= (argc-cloptind))> 0) {		/* no arguments */
		if (strcmp(DIRFN, ".") != 0) {		/* change dir?  */
			clerror("only one directory allowed with 'd' option") ;
			exit(2) ;			/* can't do now */
		}
		FL_LIST= getlist(TOTFILE, argv+cloptind, &n, &TOTSIZE, &rc) ; 
		TOTFILE= n ;
	}
	else {
		FL_LIST= makelist(DIRFN, &n, &TOTSIZE, &rc) ;
		TOTFILE= n ;
	}

	if (rc != 0) {		/* list read error - message already printed */
		exit(1) ;
	}

	if (FL_LIST == NULL) {	      /* list is empty error - print message */
		clerror("no file(s)") ;
		exit(3) ;
	}

	initlist() ;
	initscr() ;
	scnmode(1) ;
}

static void fini()
{
	endwin();
}

static void initlist()
{
	struct fl_line *flp ;

	if (ICMD[0] != '\0') {
		for (flp= FL_LIST; flp != NULL; flp= flp->next) {
			fl_scmd(flp, ICMD) ;
		}
	}
}

static void addopt(opt)
int opt ;
{
	register char *op ;
	
	op= OPTSTR + strlen(OPTSTR) ;
	(*op++)= OPTPFX_CH ;
	(*op++)= opt ;
	(*op++)= ' ' ;
	(*op++)= '\0' ;
}

static void addoptarg(arg)
char *arg ;
{
	strcat(OPTSTR, "\"") ;
	strcat(OPTSTR, arg) ;
	strcat(OPTSTR, "\" ") ;
}
