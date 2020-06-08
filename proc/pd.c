/*

	pd.c				Process Diagnostic for AT&T Unix
		 			Copyright (C) 1992 Contahal Ltd.


*/

#include	<stdio.h>
#include	<unistd.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"pd.h"

struct opts o ;
struct parms p ;

static void setdefaults() ;
static void init() ;
static void doit() ;
static void syntax() ;
static void signon() ;

pid_t getpid() ;

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
	p.procdirpfx= "/proc/" ;
	o.diagstate= 1 ;
	o.diagpsinfo= 1 ;
	o.diagmem= 1 ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;

	clinit(argv) ;

	signon() ;
	while((i= clgetopt(argc, argv, "dhz"))> 0) {
		switch(i) {
			case 'd':	o.detail= 1 ;		break ;
			case 'h':	o.help= 1 ;		break ;
			case 'z':	o.debug= 1 ;		break ;
		}
	}
	p.nargs= argc - cloptind ;
	p.arglist= argv + cloptind ;

	if (o.help) {
		syntax() ;
		exit(0) ;
	}

	if (p.nargs == 0) {
		clerror("use 'h' option for help") ;
		exit(1) ;
	}
}

static void doit()
{
	int i ;
	long pid ;
	char *pidname ;
	char emsg[256] ;

	for (i= 0; i< p.nargs; i++) {
		pidname= p.arglist[i] ;
		if (memcmp(pidname, p.procdirpfx, strlen(p.procdirpfx)) == 0) {
			pidname+= strlen(p.procdirpfx) ;
		}
		if (strcmp(pidname, ".") == 0) {	/* my own process */
			pid= (long) getpid() ;
		}
		else if (sscanf(pidname, "%ld", &pid) != 1) {
			sprintf(emsg, "bad pid: %s", pidname) ;
			clerror(emsg) ;
			continue ;
		}
		pd_diag(pid) ;
	}
}

static void syntax()
{
	int i ;
	static char *s_msg[]= {
		"syntax:	pd [option(s)] pid [pid ...]",
		"",
		"options:",
		"\t-h\t\tPrints this help list",
		"\t-d\t\tPrints detailed report",
		"\t-z\t\tEnables debugging",
	} ;
#define	N_MSGS	(sizeof(s_msg)/sizeof(s_msg[0]))

	for (i= 0; i< N_MSGS; i++) {
		fprintf(stderr, "%s\n", s_msg[i]) ;
	}
}

static void signon()
{
	fprintf(stderr, "pd 0.04 Process Diagnostic [AT&T Unix V/4.0.2.0]\n") ;
	fprintf(stderr, "Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}
