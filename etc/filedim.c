/*

	filedim.c			Show file dimensions
		 			Copyright (C) 1993 Contahal Ltd.

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<memory.h>
#include	<stdlib.h>
#include	<unistd.h>

#include	"compiler.h"
#include	"clgetopt.h"

struct parms {
	int nargs ;
	char **arglist ;
} ;

struct opts {
	int help ;
	int debug ;
} ;

struct opts o ;
struct parms p ;

static void setdefaults() ;
static void init() ;
static void doit() ;
static void syntax() ;
static void signon() ;

static void report() ;
static void emsg() ;
static void printfiledim() ;

// int exit();
void clinit();
int clgetopt(int, char**, char*);
void clerror(char*);

int main(argc, argv)
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

	signon() ;
	while((i= clgetopt(argc, argv, "hz"))> 0) {
		switch(i) {
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
#if	OPSYS==OPMSDOS
		clerror("use '/h' for help") ;
#else
		clerror("use '-h' for help") ;
#endif
		exit(1) ;
	}
}

static void doit()
{
	int i ;

	for (i= 0; i< p.nargs; i++) {
		printfiledim(p.arglist[i]) ;
	}
}

static void syntax()
{
	int i ;
	static char *s_msg[]= {
#if	OPSYS==OPMSDOS
		"syntax:	filedim [/h] [/z] fn [fn ...]",
#else
		"syntax:	filedim [-h] [-z] fn [fn ...]",
#endif
		"",
		"\th\t\tPrints this help list",
		"\tz\t\tEnables debugging",
	} ;
#define	N_MSGS	(sizeof(s_msg)/sizeof(s_msg[0]))

	for (i= 0; i< N_MSGS; i++) {
		fprintf(stderr, "%s\n", s_msg[i]) ;
	}
}

static void signon()
{
	fprintf(stderr, "filedim  display file dimensions\n") ;
	fprintf(stderr, "Copyright (C) 1993, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}

#define		BLOCKSIZE	512

static void printfiledim(fn)
char *fn ;
{
	int fh, i, len ;
	unsigned char c ;
	char fbuf[BLOCKSIZE] ;
	long fpos, binarypos, lines, minlen, maxlen, ccol ;

	if ((fh= open(fn, O_RDONLY))< 0) {
		emsg("can't open", fn) ;
		return ;
	}

	binarypos= minlen= -1L ;
	fpos= lines= maxlen= ccol= 0L ;
	for (fpos= 0L; (len= read(fh, fbuf, BLOCKSIZE))> 0; fpos+= (long)len) {
		for (i= 0; i< len; i++) {
			c= (unsigned char) fbuf[i] ;
			if (c == '\n') {
				if ((ccol< minlen) || (minlen< 0L)) {
					minlen= ccol ;
				}
				if (ccol> maxlen) maxlen= ccol ;
				ccol= 0L ;
				lines++ ;
				continue ;
			}
			if (c == '\t') {
				ccol+= (8L - (ccol & 7L)) ;
				continue ;
			}
#if	OPSYS==OPMSDOS
			if (c == '\r') continue ;
#endif
			if ((c< 32) && (binarypos< 0L)) {
				binarypos= fpos + (long) i ;
			}
			ccol++ ;
		}
	}

	report(fn, binarypos, lines, minlen, maxlen) ;
	close(fh) ;
}


static void report(fn, binarypos, lines, minlen, maxlen)
char *fn ;
long binarypos, lines, minlen, maxlen ;
{
	printf("%s: %ld lines", fn, lines) ;
	if (lines> 0L) printf(", len=%ld-%ld", minlen, maxlen) ;
	if (binarypos>= 0L) printf(": BINARY FILE @%ld", binarypos) ;
	printf("\n") ;
}

static void emsg(msg, what)
char *msg, *what ;
{
	char ebuf[512] ;

	sprintf(ebuf, "%s: %s", msg, what) ;
	clerror(ebuf) ;
}
