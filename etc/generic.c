/*

	generic.c			Generic Command
		 			Copyright (C) 1993 Contahal Ltd.


	Use this template to build your own command,
	keeping with the consistent command syntax.

*/

#include	<stdio.h>
#include	"compiler.h"
#include	"clgetopt.h"

struct parms {
	int nargs ;
	char **arglist ;
} ;

struct opts {
	int help ;
	int quiet ;
	int debug ;
} ;

struct opts o ;
struct parms p ;

static void setdefaults() ;
static void init() ;
static void doit() ;
static void syntax() ;
static void signon() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	setdefaults() ;
	init(argc, argv) ;
	doit() ;
	exit(0) ;
}

static void setdefaults()
{
	memset((char *)&p, '\0', sizeof(p)) ;
	memset((char *)&o, '\0', sizeof(o)) ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;

	clinit(argv) ;

	while((i= clgetopt(argc, argv, "hqz"))> 0) {
		switch(i) {
			case 'h':	o.help= 1 ;		break ;
			case 'q':	o.quiet= 1 ;		break ;
			case 'z':	o.debug= 1 ;		break ;
			default:	exit(1) ;		break ;
		}
	}
	p.nargs= argc - cloptind ;
	p.arglist= argv + cloptind ;

	if (o.help) {
		signon() ;
		syntax() ;
		exit(0) ;
	}

	if (p.nargs == 0) {
		clerror("use 'h' option for help") ;
		exit(1) ;
	}

	if (!o.quiet) {
		signon() ;
	}
}

static void doit()
{
	int i ;

	for (i= 0; i< p.nargs; i++) {
		printf("arg%d: '%s'\n", i+1, p.arglist[i]) ;
	}
}

static void syntax()
{
	int i ;
	static char *s_msg[]= {
#if	OPSYS==OPMSDOS
		"syntax:	cmd [/h] [/q] [/z] fn [fn ...]",
#else
		"syntax:	cmd [-h] [-q] [-z] fn [fn ...]",
#endif
		"",
		"options:",
		"\th\tPrints this help list",
		"\tq\tQuiet.  Disables signon banner",
		"\tz\tEnables debugging",
	} ;
#define	N_MSGS	(sizeof(s_msg)/sizeof(s_msg[0]))

	for (i= 0; i< N_MSGS; i++) {
		fprintf(stderr, "%s\n", s_msg[i]) ;
	}
}

static void signon()
{
	fprintf(stderr, "cmd  Generic Command\n") ;
	fprintf(stderr, "Copyright (C) 1993, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}
