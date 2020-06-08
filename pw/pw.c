/*

	pw.c				Unix Process Watch Utility
		 			Copyright (C) 1992 Contahal Ltd.


*/

#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"pw.h"

pid_t getpid() ;

struct opts o ;
struct parms p ;

static void setdefaults() ;
static void init() ;
static void syntax() ;
static void signon() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	setdefaults() ;
	init(argc, argv) ;
	pw_doit() ;
	exit(0) ;
}

static void setdefaults()
{
	memset((char *)&p, '\0', sizeof(p)) ;
	memset((char *)&o, '\0', sizeof(o)) ;
	p.waitsec= 2 ;
	p.iterations= 10 ;
	p.mypid= (long) getpid() ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;

	clinit(argv) ;

	signon() ;
	while((i= clgetopt(argc, argv, "hzw:i:x:I"))> 0) {
		switch(i) {
			case 'w': p.waitsec= getnum(cloptarg) ;		break ;
			case 'i': p.iterations= getnum(cloptarg) ;	break ;
			case 'I': o.inter= 1 ;				break ;
			case 'h': o.help= 1 ;				break ;
			case 'x': o.reject_user= cloptarg ;		break ;
			case 'z': o.debug= 1 ;				break ;
		}
	}
	p.nargs= argc - cloptind ;
	p.arglist= argv + cloptind ;

	if (o.help) {
		syntax() ;
		exit(0) ;
	}

	if (p.nargs != 0) {
		clerror("use 'h' option for help") ;
		exit(1) ;
	}

}

static void syntax()
{
	int i ;
	static char *s_msg[]= {
		"syntax:	pw [option(s)]",
		"",
		"options:",
		"\t-h\t\tPrints this help list",
		"\t-I\t\tInteractive mode",
		"\t-i #\t\tSet total iterations to #",
		"\t-w #\t\tWait # seconds between iterations",
		"\t-z\t\tEnables debugging",
	} ;
#define	N_MSGS	(sizeof(s_msg)/sizeof(s_msg[0]))

	for (i= 0; i< N_MSGS; i++) {
		fprintf(stderr, "%s\n", s_msg[i]) ;
	}
}

static void signon()
{
	fprintf(stderr, "pw 0.02  Unix Watch Process Diagnostic\n") ;
	fprintf(stderr, "Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}

static int getnum(s)
char *s ;
{
	int i ;
	char emsg[256] ;

	if (sscanf(s, "%d", &i) != 1) {
		sprintf(emsg, "invalid number: %d", s) ;
		clerror(emsg) ;
		exit(1) ;
	}
	return(i) ;
}

