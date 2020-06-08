/*
	root mail filter:

	Filter root's mail file to condense long messages
*/

#include	<stdio.h>

static void process() ;
static void output_line() ;
static void output_pfxline() ;
static void init_sect() ;
static void fini_sect() ;
static void open_fncondense() ;
static void close_fncondense() ;

struct findline {
	int fmode ;
	char *fstring ;
} ;
#define	FMODE_LMATCH	0
#define	FMODE_LNOMATCH	1

struct section {
	int smode ;
	struct findline sect_start ;
	struct findline sect_end ;
} ;
#define		SMODE_PASSTHRU		0
#define		SMODE_FNCONDENSE	1

static struct section find_sect[]= {
	{
		SMODE_FNCONDENSE,
		{ FMODE_LMATCH, "Complete System backup in progress" },
		{ FMODE_LNOMATCH, "/" },
	},
	{
		SMODE_FNCONDENSE,
		{ FMODE_LMATCH, "Verifying cartridge tape(s)" },
		{ FMODE_LNOMATCH, "/" },
	}
} ;

#define		N_SECTIONS	(sizeof(find_sect)/sizeof(find_sect[0]))

struct state {
	char *pname ;
	int smode ;
	struct findline *sep ;
	int nsect ;
} ;

static struct state s= { "rmf", SMODE_PASSTHRU } ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char buffer[4096] ;

	s.pname= argv[0] ;
	s.nsect= 0 ;
	while(gets(buffer)) {
		process(buffer) ;
	}

	fprintf(stderr, "%s: %d section(s) cleaned\n", s.pname, s.nsect) ;

	if (s.smode != SMODE_PASSTHRU) {
		fprintf(
			stderr,
			"%s: error: open section: type=%d\n",
			s.pname,
			s.smode
		) ;
		exit(1) ;
	}
	exit(0) ;
}

static void process(buf)
char *buf ;
{
	int smode ;
	struct findline *sep ;

	if (s.smode == SMODE_PASSTHRU) {
		if (find_sect_start(buf, &smode, &sep)) {
			s.smode= smode ;
			s.sep= sep ;
			s.nsect++ ;
			init_sect() ;
			output_pfxline(buf) ;
			return ;
		}
		output_line(buf) ;
		return ;
	}

	if (s.smode == SMODE_FNCONDENSE) {
		if (output_fncondense(buf)) {
			fini_sect() ;
			s.smode= SMODE_PASSTHRU ;
			output_pfxline(buf) ;
			return ;
		}
		return ;
	}
}

static int find_sect_start(buf, smp, sepp)
char *buf ;
int *smp ;
struct findline **sepp ;
{
	register int i ;

	for (i= 0; i< N_SECTIONS; i++) {
		if (match_line(buf, &find_sect[i].sect_start)) {
			(*smp)= find_sect[i].smode ;
			(*sepp)= &find_sect[i].sect_end ;
			return(1) ;
		}
	}
	return(0) ;
}

static int match_line(buf, flp)
char *buf ;
struct findline *flp ;
{
	if (flp->fmode == FMODE_LMATCH) {
		return(memcmp(buf, flp->fstring, strlen(flp->fstring)) == 0) ;
	}
	if (flp->fmode == FMODE_LNOMATCH) {
		return(memcmp(buf, flp->fstring, strlen(flp->fstring)) != 0) ;
	}
	return(0) ;
}

static void init_sect()
{
	if (s.smode == SMODE_FNCONDENSE) {
		open_fncondense() ;
	}
}

static void fini_sect()
{
	if (s.smode == SMODE_FNCONDENSE) {
		close_fncondense() ;	
	}
}

static int fnc_dirc ;
static char fnc_name[4096] ;

static void open_fncondense()
{
	fnc_dirc= 0 ;
	fnc_name[0]= '\0' ;
}

static void close_fncondense()
{
	char msgbuf[256] ;

	if (fnc_dirc == 0) return ;
	sprintf(
		msgbuf,
		"%s: %5d file(s): %s",
		s.pname,
		fnc_dirc,
		fnc_name
	) ;
	output_line(msgbuf) ;
}

static int output_fncondense(buf)
char *buf ;
{
	register char *p ;

	if (buf[0] == '\0') return(0) ;		/* ignore blank line */

#if 0
	if (buf[0] != '/') return(1) ;		/* break out if bad pattern */
#endif

	if (match_line(buf, s.sep)) return(1) ;	/* break out if ended */

	if ((fnc_dirc == 0) || (memcmp(buf, fnc_name, strlen(fnc_name)) != 0)) {
		close_fncondense() ;
		open_fncondense() ;
		strcpy(fnc_name, buf) ;
	}

	fnc_dirc++ ;
	return(0) ;
}

static void output_pfxline(buf)
char *buf ;
{
	fprintf(stdout, "%s: ", s.pname) ;
	puts(buf) ;
}

static void output_line(buf)
char *buf ;
{
	puts(buf) ;
}
