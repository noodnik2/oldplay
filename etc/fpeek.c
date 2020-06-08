/*

	fpeek.c				Peek at section of file
		 			Copyright (C) 1992 Contahal Ltd.


		fpeek <file> <offset> <len>

*/

#include	<stdio.h>
#include	<fcntl.h>

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
static void fdpeek() ;
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

	if (p.nargs != 3) {
		clerror("use 'h' option for help") ;
		exit(1) ;
	}
}

static void doit()
{
	int fd ;
	long offset, len ;
	
	if (getlong(p.arglist[1], &offset)) {
		clerror("invalid offset") ;
		exit(1) ;
	}
	if (getlong(p.arglist[2], &len)) {
		clerror("invalid length") ;
		exit(2) ;
	}
	if ((fd= open(p.arglist[0], O_RDONLY))< 0) {
		clerror("can't open file") ;
		exit(3) ;
	}
	fdpeek(fd, offset, len) ;
	close(fd) ;
}

#define		BLKSIZE		512

static void fdpeek(fd, offset, len) 
int fd ;
long offset, len ;
{
	int i, rl ;
	unsigned char buf[BLKSIZE] ;

	if (lseek(fd, offset, SEEK_SET)< (off_t)0) {
		clerror("lseek error") ;
		return ;
	}	
	while(len> 0L) {
		rl= (len> (long) BLKSIZE)? (int) BLKSIZE: (int) len ;
		rl= read(fd, buf, rl) ;
		if (rl == 0) break ;
		if (rl< 0) {
			clerror("read error") ;
			break ;
		}
		for (i= 0; i< rl; i++) {
			putchar(buf[i]) ;
		}
		len-= (long) rl ;
	}
}
	
static void syntax()
{
	int i ;
	static char *s_msg[]= {
#if 	OPSYS==OPMSDOS
		"syntax:	fpeek [/h] [/z] fn offset len",
#else
		"syntax:	fpeek [-h] [-z] fn offset len",
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
	fprintf(stderr, "fpeek 0.01  Peek at file section\n") ;
	fprintf(stderr, "Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}

static int getlong(s, lp)
char *s ;
long *lp ;
{
	if ((s[0] == '0') && (s[1] == 'x')) {
		return(sscanf(s, "%lX", lp) != 1) ;
	}
	return(sscanf(s, "%ld", lp) != 1) ;
}
