/*	Revision:	3
*/
/*

	browse.c			File Browse Utility
					Copyright (C) 1990-1993, by Marty Ross

*/

#include	<stdio.h>
#include	<curses.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"browse.h"
#include	"ebcdic.h"

char pver[]= "0.14" ;

struct br_arg bra ;
struct br_opt bro ;
struct br_scn brs ;

extern void brsettab() ;

void main() ;

static void setdefaults(), init(), fini(), getrsize(), signon() ;

void main(argc, argv)
int argc;
char *argv[] ;
{
	int i, rc ;

	rc= 0 ;
	setdefaults() ;
	for (init(argc, argv, &i); i< argc; i++) {
		if (dosetup(argv[i]) == 0) {
			rc+= (dobrowse() != 0) ;
		}
		else rc++ ;
	}
	fini(rc) ;
}

void fini(rc)
int rc;
{
#if 0
	endwin() ;
#endif
	if (bro.o_debug) fprintf(stderr, "R(%d);\n", rc); 
	exit(rc);
}

/*
	begin static routines
*/


static void setdefaults()
{
	memset(&bra, '\0', sizeof(bra)); 
	memset(&bro, '\0', sizeof(bro)); 
	RECSIZE= RECSIZEMAX ;
	NUMLINE= F_LINES ;
	RLINE= F_LINES ;
	strcpy(HELPCMD, "more <") ;
}

static void init(argc, argv, ip)
int argc, *ip;
char *argv[] ;
{
	int c ;

	clinit(argv) ;
	while((c= clgetopt(argc, argv, "behmr:stxXH:z")) != -1) {
		switch(c) {
			case 'b':	bro.o_bin= 1 ;			break ;
			case 'e':	bro.o_ebcdic= 1 ;		break ;
			case 'h':	bro.o_help= 1;			break ;
			case 'm':	bro.o_mixed= 1;			break ;
			case 'r':	getrsize(cloptarg) ;		break ;
			case 's':	bro.o_scale= 1;			break ;
			case 't':	bro.o_tab= 1;			break ;
			case 'x':	bro.o_hex= -1;			break ;
			case 'X':	bro.o_hex= 1;			break ;
			case 'z':	bro.o_debug= 1;			break ;
			case 'H':	strcpy(HELPCMD, cloptarg) ;	break ;
			default:	exit(1);			break ;	
		}
	}
	(*ip)= cloptind ;

	if (bro.o_help) {
		signon() ;
		syntax() ;
		exit(0) ;
	}

	EBCDIC_OK= (ebcinit(NULL) == 0) ;

	if (bro.o_ebcdic && !EBCDIC_OK) {
		clerror("note: ebcdic disabled: option ignored") ;
		bro.o_ebcdic= 0 ;
	}

#if 0
	initscr() ;
#endif

}

static int dosetup(fn)
char *fn ;
{
	struct fl_stat fs ;
	char emsg[120] ;
	int rc ;

	if (statfile(fn, &fs)) {
		rc= 12 ;
		sprintf(emsg, "can't open: %s", fn);
		doerror(rc, emsg) ;
		return(rc) ;
	}
	if (fs.isdir) {
		rc= 19 ;
		sprintf(emsg, "%s is a directory", fn);
		doerror(rc, emsg) ;
		return(rc) ;
	}
	FILENAME= fn ;
	FILESIZE= fs.size ;
	FILEDATE= fs.date ;
	HEXMODE= bro.o_hex ;
	BINMODE= bro.o_bin ;
	CASEMODE= bro.o_mixed ;
	EBCMODE= bro.o_ebcdic ;
	RLINE= bro.o_scale? 0 : F_LINES ;

	brsettab(&CDRAW) ;

	return(0) ;
}

static void getrsize(s)
char *s;
{
	char ebuf[120] ;
	int rsize ;

	if (sscanf(s, "%d", &rsize)!=1) {
		sprintf(ebuf, "invalid number: %s", s);
		clerror(ebuf) ;
		exit(1) ;
	}
	if (rsize> RECSIZEMAX) {
		clerror("recordsize too large") ;
		exit(1) ;
	}
	RECSIZE= rsize ;
}

static void signon()
{
	fprintf(stderr, "BROWSE v%s  File Browse Utility\n", pver) ;
	fprintf(stderr, "Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}
