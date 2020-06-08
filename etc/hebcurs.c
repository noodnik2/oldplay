/*	Revision:	2		12.2.95
*/
/*

	hebcurs.c

	Attempt to print Hebrew text using Curses
	Uses "setlocale()" function, as suggested by TEAM:

		setlocale(LC_ALL, "") ;
	
	with external environment variable "LANG" set to "IS"

	syntax:
		hebcurs			(attempts to display hebrew)
		-or-
		hebcurs nohebrew	(displays english string instead)

	12.2.95:	Conclusions:

			this method works on "some" computers:
			   Computer			Works?
			1. Data General DG/UX		Yes
			2. NST/NCR3000/ATT4.0/2.0	No
			3. NCR21/NCR3000/ATT4.0/3.0	Yes
			
*/

/*
	#includes
*/

#include	<curses.h>
#include	<locale.h>

/*
	#defines
*/
#define		HCI_HEBSTR_ISO		0
#define		HCI_HEBSTR_IBM		1
#define		HCI_ENGSTR		2

/*
	internal data
*/
static char *apsztestnames[]= {
	"iso",
	"ibm",
	"eng"
} ;
#define	NTESTS	(sizeof(apsztestnames) / sizeof(apsztestnames[0]))

static char *apsztestinfo[NTESTS]= {
	"ANSI/ISO Hebrew Support 8859-8 (224-based)",
	"IBM/MS-DOS Hebrew Support 862 (128-based)",
	"English Test String (no Hebrew)"
} ;

static unsigned char pszhebstr_iso[]= { 0xED, 0xE5, 0xEC, 0xF9, 0 } ;
static unsigned char pszhebstr_ibm[]= { 0x8D, 0x85, 0x8C, 0x99, 0 } ;
static unsigned char pszengstr[]= { 'H', 'e', 'l', 'l', 'o', 0 } ;

static unsigned char *apsztestdata[NTESTS]= {
	pszhebstr_iso,
	pszhebstr_ibm,
	pszengstr
} ;

/*
	forward declarations
*/

static doit(unsigned char *psztest) ;
static void syntax(void) ;

/*
	entry points
*/
void main(int argc, char **argv)
{
	int index ;

	if (argc< 1) syntax() ;
	for (index= 0; index< NTESTS; index++) {
		if (strcmp(argv[1], apsztestnames[index]) == 0) {
			break ;
		}
	}
	if (index>= NTESTS) syntax() ;
		
				/* 12.02.95: LC_CTYPE sufficient	*/
	if (setlocale(LC_CTYPE, "") == NULL) {
		fprintf(stderr, "setlocale failed!\n") ;
		fprintf(stderr, "Press ENTER to continue\n") ;
		getchar() ;
	}
	initscr() ;
	doit(apsztestdata[index]) ;
	endwin() ;
}

/*
	internal subroutines
*/

static doit(unsigned char *psztest)
{
	int i ;

	move(1, 1) ;
	for (i= 0; psztest[i]; i++) addch(psztest[i]) ;
	refresh() ;
	getch() ;
}

static void syntax()
{
	int index ;

	fprintf(stderr, "syntax: cmd arg\n") ;
	fprintf(stderr, "where 'arg' is one of:\n") ;
	for (index= 0; index< NTESTS; index++) {
		fprintf(
			stderr,
			"\t%s\t - %s\n",
			apsztestnames[index],
			apsztestinfo[index]
		) ;
	}
	exit(1) ;
}

/* eof */
