/*

	nlbl				Print Labels in n-columns
					Copyright (C) 1992, Contahal Ltd.
					
	syntax:

		nlbl [options] < pTf > ncmlf

	Transforms pseudo-Thelma-file format into
	n-column mailing label format.


	The program "nlbl" reads a file in pseudo-Thelma format and
	produces n-column labels, using the specified parameters
	for interlabel spacing.

	A "pseudo-Thelma" file contains lines in the format:

		l1!l2!l3!l4\n

	That is, on each line of an ASCII file, there are y-lines
	(4 in this example) of label data, delimited by an
	exclamation mark, "!".

*/

#include	<stdio.h>

#include	"compiler.h"
#include	"clgetopt.h"

#define		O_PNCOLS	3		/* # of labels across page */
#define		O_PNROWS	7		/* # of labels down page */
#define		O_PVINDENT	3		/* vertical page indent */
#define		O_PHINDENT	0		/* horizontal page indent */

#define		O_LNROWS	9		/* # of output rows */
#define		O_LNCOLS	27		/* # of output cols */
#define		O_LHINDENT	0		/* output indentation */
#define		O_LRECL		1024		/* max output lrecl */

#define		I_LDELIM	'!'		/* input field delimiter */
#define		I_LRECL		1024		/* max input lrecl */

#define		F_LRECL		2048		/* file input lrecl */

static void setdefaults() ;
static void init() ;
static void getonum() ;
static void getcval() ;
static void setup() ;
static void doit() ;
static void syntax() ;
static void infmt() ;
static void outvi() ;
static void outfmt() ;
static void outpage() ;
static void report() ;
static void signon() ;

char *strchr() ;
char *malloc() ;

struct l_opts {
	int pnrows ;
	int pvindent ;
	int phindent ;
	int lnrows ;
	int lncols ;
	int lhindent ;
	int pncols ;
	int help ;
	int quiet ;
	int ldelim ;
} ;

struct l_parms {
	char l_fmtstr[15] ;
	char l_histr[15] ;
	char prog_name[65] ;
	char *file_irec ;
	char *lbl_orec ;
	int lbl_len, lblrow_len ;
} ;
	
static struct l_opts lo ;
static struct l_parms lp ;

/*
	main()

	reads standard input,
	produce standard output
*/
void main(argc, argv)
int argc ;
char *argv[] ;
{
	setdefaults() ;
	init(argc, argv) ;
	setup() ;
	doit() ;
	exit(0) ;
}

static void setdefaults()
{
	memset((char *)&lo, '\0', sizeof(lo)) ;
	lo.pnrows=	O_PNROWS ;
	lo.lnrows=	O_LNROWS ;
	lo.lncols=	O_LNCOLS ;
	lo.pvindent=	O_PVINDENT ;
	lo.phindent=	O_PHINDENT ;
	lo.lhindent=	O_LHINDENT ;
	lo.pncols=	O_PNCOLS ;
	lo.ldelim=	I_LDELIM ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;

	clinit(argv) ;
	clcmdname(lp.prog_name, argv[0], sizeof(lp.prog_name)) ;
	while((i= clgetopt(argc, argv, "r:c:l:w:v:i:s:d:hq"))>= 0) {
		switch(i) {
/* "rows" */		case 'r': getonum(&lo.pnrows) ; break ;
/* "cols" */		case 'c': getonum(&lo.pncols) ; break ;
/* "length" */		case 'l': getonum(&lo.lnrows) ; break ;
/* "width" */		case 'w': getonum(&lo.lncols) ; break ;
/* "vert. indent" */	case 'v': getonum(&lo.pvindent) ; break ;
/* "horiz. indent" */	case 'i': getonum(&lo.phindent) ; break ;
/* "label shift" */	case 's': getonum(&lo.lhindent) ; break ;
/* "delimiter" */	case 'd': getcval(&lo.ldelim) ; break ;
/* "help" */		case 'h': lo.help= 1 ; break ;
/* "quiet" */		case 'q': lo.quiet= 1 ; break ;

			default:
				clerror("use 'h' option for help") ;
				exit(1) ;
				break ;
		}
	}
}

static void getcval(ip)
int *ip ;
{
	char emsg[256] ;
	int i ;

	if (strlen(cloptarg) != 1) {
		sprintf(emsg, "bad character: %s", cloptarg) ;
		clerror(emsg) ;
		exit(1) ;
	}
	(*ip)= (int) (cloptarg[0]) ;
}

static void getonum(ip)
int *ip ;
{
	char emsg[256] ;
	int i ;

	if (sscanf(cloptarg, "%d", &i) != 1) {
		sprintf(emsg, "bad value: %s", cloptarg) ;
		clerror(emsg) ;
		exit(1) ;
	}
	(*ip)= i ;
}

static void setup()
{
	if (lo.help) {
		signon() ;
		syntax() ;
		exit(0) ;
	}

	lp.file_irec= (char *)malloc(F_LRECL) ;
	lp.lbl_orec= (char *)malloc(lo.pncols * lo.lnrows * I_LRECL) ;

	if ((lp.file_irec == (char *)NULL) || (lp.lbl_orec == (char *)NULL)) {
		clerror("can't allocate buffers") ;
		exit(12) ;
	}

	lp.lblrow_len= I_LRECL ;
	lp.lbl_len= (lo.lnrows * I_LRECL) ;

	sprintf(
		lp.l_fmtstr,
		"%%%ds%%-%ds",
		lo.lhindent,
		lo.lncols-lo.lhindent
	) ;
	sprintf(
		lp.l_histr,
		"%%%ds",
		lo.phindent
	) ;

	if (!lo.quiet) {
		report() ;
	}
}

static void doit()
{
	int i, n, lr ;
	char *orec_p ;

	n= 0 ;
	lr= 0 ;

	do {

		orec_p= lp.lbl_orec ;
		for (i= 0; i< lo.pncols; i++) {
			if (gets(lp.file_irec) == (char *)NULL) break ;
			infmt(lp.file_irec, orec_p) ;
			orec_p+= lp.lbl_len ;
		}
		if (i == 0) continue ;
		if (lo.pnrows != 0) {
			if (lr>= lo.pnrows) {
				outpage() ;
				lr= 0 ;
			}
		}
		if (lr == 0) outvi() ;
		outfmt(lp.lbl_orec, i) ;
		lr++ ;
		n++ ;

	} while( i == lo.pncols ) ;

	if (!lo.quiet) {
		fprintf(stderr, "%d labels printed; %d rows\n",
			(n*lo.pncols)+i, n) ;
	}
}

static void signon()
{
	fprintf(stderr, "%s - print labels across page\n", lp.prog_name) ;
	fprintf(stderr, "Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(stderr, "\n") ;
}

static void report()
{
	signon() ;
	fprintf(stderr, "  page:\n") ;
	fprintf(stderr, "      vertical indent: %d\n", lo.pvindent) ;
	fprintf(stderr, "    horizontal indent: %d\n", lo.phindent) ;
	fprintf(stderr, "   # of label columns: %d\n", lo.pncols) ;
	fprintf(stderr, "      # of label rows: %d\n", lo.pnrows) ;
	fprintf(stderr, "\n") ;
	fprintf(stderr, "  label:\n") ;
	fprintf(stderr, "    horizontal indent: %d\n", lo.lhindent) ;
	fprintf(stderr, "        label columns: %d\n", lo.lncols) ;
	fprintf(stderr, "          label lines: %d\n", lo.lnrows) ;
	fprintf(stderr, "\n") ;
}

static void syntax()
{
	int i ;
	static char *hlist[]= {
"syntax:",
"",
"\tnlbl [options] < infile > outfile",
"",
"options:",
"",
"\th\tprint this help message",
"\tr #\tnumber of label rows per page",
"\tl #\tlength of label (lines)",
"\tw #\twidth of label (columns)",
"\tv #\tvertical page indentation (lines)",
"\ti #\thorizontal page indentation (columns)",
"\ts #\tshift labels right # (columns)",
""
	} ;
#define	NHLIST	(sizeof(hlist)/sizeof(hlist[0]))

	for (i= 0; i< NHLIST; i++) {
		fprintf(stderr, "%s\n", hlist[i]) ;
	}

}

/*
	(void) infmt(irec, orec) ;

	Parses data in 'inrec' according to delimiter
	character 'lo.ldelim'.  Parses 'lo.lnrows' parts
	into array elements of 'orec'.
*/
static void infmt(irec, orec)
char *irec, *orec ;
{
	int i, l ;
	char *s, *p, *orec_p ;

	s= irec ;
	orec_p= orec ;
	for (i= 0; i< lo.lnrows; i++) {
		if ((p= strchr(s, lo.ldelim)) != NULL) {
			l= (int) (p-s) ;
			memcpy(orec_p, s, l) ;
			orec_p[l]= '\0' ;
			s= p+1 ;
		}
		else {
			strcpy(orec_p, s) ;
			s+= strlen(s) ;
		}
		orec_p+= lp.lblrow_len ;
	}
}

/*
	(void) outfmt(orec, npcols) ;

	Produces standard output for one row of labels.
*/
static void outfmt(orec, npcols)
char *orec ;
int npcols ;
{
	int i, j ;
	char *p, *orec_p, *olbl_p, outstr[O_LRECL] ;

	orec_p= orec ;
	for (i= 0; i< lo.lnrows; i++) {
		sprintf(outstr, lp.l_histr, "") ;
		p= outstr + strlen(outstr) ;
		olbl_p= orec_p ;
		for (j= 0; j< npcols; j++) {
			sprintf(p, lp.l_fmtstr, "", olbl_p) ;
			p[lo.lncols]= '\0' ;
			p+= strlen(p) ;
			olbl_p+= lp.lbl_len ;
		}
		for (j= strlen(outstr)-1; j>= 0; j--) {
			if (outstr[j] != ' ') break ;
		}
		outstr[j+1]= '\0' ;
		printf("%s\n", outstr) ;
		orec_p+= lp.lblrow_len ;
	}

	while(i< lo.lnrows) {
		printf("\n") ;
		i++ ;
	}

}

static void outvi()
{
	int i ;

	for (i= 0; i< lo.pvindent; i++) {
		printf("\n") ;
	}
}

static void outpage()
{
	printf("\014") ;
	fflush(stdout) ;
}

static int min(a, b)
int a, b ;
{
	return((a< b)? a: b) ;
}
