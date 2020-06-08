/*

	hebfilt.c			Hebrew Filter
		 			Copyright (C) 1993 Contahal Ltd.


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
	int ibm2iso ;
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

	while((i= clgetopt(argc, argv, "hiqz"))> 0) {
		switch(i) {
			case 'h':	o.help= 1 ;		break ;
			case 'i':	o.ibm2iso= 1 ;		break ;
			case 'q':	o.quiet= 1 ;		break ;
			case 'z':	o.debug= 1 ;		break ;
		}
	}
	p.nargs= argc - cloptind ;
	p.arglist= argv + cloptind ;

	if (o.help) {
		signon() ;
		syntax() ;
		exit(0) ;
	}

	if (p.nargs != 0) {
		clerror("use 'h' option for help") ;
		exit(1) ;
	}

	if (!o.quiet) {
		signon() ;
	}
}

static void doit()
{
	int c ;
	unsigned char uc ;
	int from_base, to_base ;
#define	HEBCH_FSIZE	27

	if (o.ibm2iso) {
		from_base= 128 ;
		to_base= 224 ;
	}
	else {
		from_base= 224 ;
		to_base= 128 ;
	}

	while((c= getchar()) != EOF) {
		uc= (unsigned char) c ;
		if (
			((int) uc>= from_base)
		     && ((int) uc<= (from_base+HEBCH_FSIZE))
		) {
			uc-= from_base ;
			uc+= to_base ;
			c= (int) uc ;
		}
		putchar(c) ;
	}
}

static void syntax()
{
	int i ;
	static char *s_msg[]= {
#if	OPSYS==OPMSDOS
		"syntax:	hebfilt [/h] [/q] [/i] [/z] <infile >outfile",
#else
		"syntax:	hebfilt [-h] [-q] [-i] [-z] <infile >outfile",
#endif
		"",
		"options:",
		"\th\tPrints this help list",
		"\ti\tIBM to ISO conversion",
		"\tq\tQuiet.  Disables signon banner",
		"\tz\tEnables debugging",
		"",
		"   This program converts files with Hebrew characters",
		"in either the ISO-standard location (224-250), or the",
		"IBM-standard location (128-154) to the other format.",
	} ;
#define	N_MSGS	(sizeof(s_msg)/sizeof(s_msg[0]))

	for (i= 0; i< N_MSGS; i++) {
		fprintf(stderr, "%s\n", s_msg[i]) ;
	}
}

static void signon()
{
	fprintf(stderr, "hebfilt  0.01 Hebrew Filter\n") ;
	fprintf(stderr, "Copyright (C) 1993, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}
