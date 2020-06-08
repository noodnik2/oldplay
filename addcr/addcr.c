#define	Revision		8

/*

	MS-DOS Syntax:
		addcr /{a|r|i|h} {/U|L} [/f] [/l] [/q] [/z] file [file ...]

	Unix Syntax:
		addcr -{a|r|i|h} -{U|L} [-f] [-l] [-q] [-z] file [file ...]

	Commands:
		a	adds CRs: converts file(s) to MSDOS format
		r	removes CRs: converts file(s) to Unix format
		i	inspect: lists status of current file format
		h	help: prints out this help message

	Options:
		f	force file conversion
		l	lists original file formats
		q	quiet: suppresses signon banner
		z	enables debug mode
		U	converts filenames to upper-case
		L	converts filenames to lower-case


	History:

		Jul 27, 1995	Now restores original file timestamps.

*/

#include	"compiler.h"
#include	"clgetopt.h"

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#if	OPSLVL==OPLAIX
#include	<utime.h>
#else
#include	<sys/utime.h>
#endif

#include	<errno.h>
#include	<fcntl.h>
#include	<ctype.h>

#if	OPSYS==OPMSDOS
#define	unlink(fn)	remove(fn)
#define	link(from,to)	rename(from,to)
#define	OPENWMASK	(O_WRONLY | O_CREAT | O_BINARY)
#define	OPENRMASK	(O_RDONLY | O_BINARY)
#else
#define	OPENWMASK	(O_WRONLY | O_CREAT)
#define	OPENRMASK	(O_RDONLY)
#endif

/*
	forward declarations
*/

static void init() ;
static void doit() ;
static void doreport() ;
static void rptfile(), rptstat() ;
static void l_rename() ;
static void l_utime() ;
static void dispose() ;
static void title() ;
static void syntax() ;
static void mk_rfn() ;
static void fncase() ;
static void getmode() ;
static int chg_add2() ;
/*
	static int dochange(char* fn, struct ftype* ftp) ;
	static int chg_rem(char *ifn, char *rfn, struct fmode *pfm) ;
	static int chg_add(char *ifn, char *rfn, struct fmode *pfm) ;
*/
static int dochange();
static int chg_rem();
static int chg_rem2() ;
static int chg_add();
static int chg_chk() ;
static int chg_chk2() ;
static int fn_exists() ;
static int mkofile() ;


/*
	structures
*/
struct options {
	int funct ;			/* main function selection	*/
	int debug ;			/* debug statistics on?		*/
	int list ;			/* list file details?		*/
	int force ;			/* force conversion		*/
	int cconv ;			/* case conversion		*/
	int quiet ;			/* suppress title		*/
} ;

struct ftype {
	int nchg ;			/* files changed		*/
	int nbin ;			/* binary files			*/
	int nbad ;			/* bad files			*/
	int ntot ;			/* files total			*/
	int ncrlf ;			/* files with cr/lf		*/
	int nlf ;			/* files with lf		*/	       
} ;

struct fmode {
	int mode ;			/* file open mode		*/
	time_t actime ;			/* file's access time		*/
	time_t modtime ;		/* file's modification time	*/
} ;

/*
	static variables
*/
static struct options opt ;
static char *fnstats[]= { "bad", "unix", "ms-dos", "binary" } ;

/*
	main procedure
*/
void main(argc, argv)
int argc;
char *argv[];
{
	int sarg ;

	init(argc, argv, &sarg) ;
	doit(argc-sarg, argv+sarg) ;
	clexit(0) ;
}

/*
	static procedures
*/

static void init(argc, argv, sargp)
int argc ;
char *argv[] ;
int *sargp ;
{
	int oc ;

	memset(&opt, '\0', sizeof(opt)) ;
	
	clinit(argv) ;
	while((oc= clgetopt(argc, argv, "arihlfqzUL")) != EOF) {
		switch(oc) {

			case 'a':
			case 'r':
			case 'i':
			case 'h':
				if (opt.funct && (opt.funct != (int)oc)) {
					clerror("bad option combination") ;
					clexit(1) ;
				}
				opt.funct= (int) oc ;
				break ;

			case 'l':	opt.list= 1 ;		break ;
			case 'f':	opt.force= 1 ;		break ;
			case 'q':	opt.quiet= 1 ;		break ;
			case 'z':	opt.debug= 1 ;		break ;

			case 'U':
			case 'L':
				if (opt.cconv) {
					clerror("bad option combination") ;
					clexit(1) ;
				}
				opt.cconv= (int) oc ;
				break ;
				
			default:	clexit(1) ;		break ;
		}
	}

	if (!opt.funct) {
		clerror("no function specified.  Use 'h' option for help") ;
		clexit(1) ;
	}

	if (!opt.quiet) title() ;
	if (opt.funct == 'h') {
		syntax() ;
		clexit(0) ;
	}

	(*sargp)= cloptind ;
}

static void doit(n, list)
int n ;
char *list[] ;
{
	int i, stat ;
	struct ftype ft ;

	if (opt.debug) {
		printf("func= '%c'\n", (char) opt.funct) ;
		printf("n= '%d'\n", n) ;
	}

	if (n == 0) {
		clerror("no file(s) specified") ;
		clexit(1) ;
	}

	ft.ntot= ft.nchg= ft.nbin= ft.nbad= 0 ;
	ft.ncrlf= ft.nlf= 0 ;

	for (i= 0; i< n; i++) {
		stat= dochange(list[i], &ft) ;			
		if (opt.list) rptstat(list[i], stat) ;
		if (stat == 0) ft.nlf++ ;
		else if (stat == 1) ft.ncrlf++ ;
		else if (stat< -1) {
			clerror("error writing output") ;
			clexit(1) ;
		}
	}

	doreport(&ft) ;
}

static void doreport(ftp)
struct ftype *ftp ;
{
	rptfile(ftp->nbad,		"can't be processed") ;
	if (opt.funct == 'i') {
		rptfile(ftp->nbin,	"binary") ;
		rptfile(ftp->ncrlf,	"ms-dos text") ;
		rptfile(ftp->nlf,	"unix text") ; 
	}
	else {
		rptfile(ftp->nchg,	"changed") ;
		rptfile(ftp->nbin,	"binary (unchanged)") ;
		rptfile(
			(ftp->ntot)-
			(ftp->nchg)-
			(ftp->nbin)-
			(ftp->nbad),	"unchanged") ;
	}
}

static void rptstat(fn, stat)
char *fn ;
int stat ;
{
	printf("%s: %s\n", fn, fnstats[stat+1]) ;
}

static void rptfile(n, msg)
int n ;
char *msg ;
{
	if (n == 0) return ;
	printf("%d file", n) ;
	if (n != 1) printf("s") ;
	printf(" %s.\n", msg) ;
}

/*

	rc= dochange(filename, &ftype) ;

	rc:
		-2	means (output) file could not be written
		-1	means (input) file could not be processed
		0	means (input) file is text, no cr's
		1	means (input) file is text, with cr's
		2	means (input) file is binary
*/
static int dochange(fn, ftp)
char *fn ;
struct ftype *ftp ;
{
	int rc, fh ;
	struct fmode fm ;
	char rfn[512] ;
	char buf[1025] ;

	if (opt.debug) printf("fn='%s'\n", fn) ;
	(ftp->ntot)++ ;
	
	if ((fh= open(fn, OPENRMASK))< 0) {
		sprintf(buf, "can't open: '%s'", fn) ;
		clerror(buf) ;
		(ftp->nbad)++ ;
		return(-1) ;
	}

	mk_rfn(fn, rfn) ;
	if (strcmp(fn, rfn) != 0) {
		if (fn_exists(rfn)) {
			sprintf(
				buf,
				"can't update '%s'; '%s' exists",
				fn,
				rfn
			) ;
			clerror(buf) ;
			(ftp->nbad)++ ;
			return(-1) ;
		}
	}

	dskrsti(fh, buf, sizeof(buf)) ;
	switch(opt.funct) {

		case 'a':
			getmode(fh, &fm) ;
			rc= chg_add(fn, rfn, &fm) ;
			if (rc == 0) (ftp->nchg)++ ;
			break ;

		case 'r':	
			getmode(fh, &fm) ;
			rc= chg_rem(fn, rfn, &fm) ;
			if (rc == 1) (ftp->nchg)++ ;
			break ;

		case 'i':
			rc= chg_chk() ;
			break ;

		default:
			clerror("sorry, option unimplemented") ;
			clexit(1) ;
			break ;
	}

	if (rc == 2) (ftp->nbin)++ ;
	return(rc) ;
}

static int chg_rem(ifn, rfn, pfm)
char *ifn, *rfn ;
struct fmode *pfm ;
{
	char ofn[512], obuf[1024] ;
	int rc ;

	if (mkofile(ofn, obuf, sizeof(obuf), pfm->mode)) {/* out file	      */
		dskrstif() ;
		return(-2) ;				/* can't write output */
	}
	rc= chg_rem2() ;				/* perform changes    */
	dispose(rc != 1, ifn, rfn, ofn, pfm) ;		/* handle accordingly */
	return(rc) ;	
}

static int chg_add(ifn, rfn, pfm)
char *ifn, *rfn ;
struct fmode *pfm ;
{
	char ofn[512], obuf[1024] ;
	int rc ;

	if (mkofile(ofn, obuf, sizeof(obuf), pfm->mode)) {/* out file  */
		dskrstif() ;
		return(-2) ;				/* can't write output */
	}
	rc= chg_add2() ;				/* perform changes    */
	dispose(rc != 0, ifn, rfn, ofn, pfm) ;		/* handle accordingly */
	return(rc) ;	
}

static int chg_chk()
{
	int rc ;

	rc= chg_chk2() ;
	dskrstif() ;
	return(rc) ;
}

/*
	chg_chk2:		read file; report about text contents

		returns:
				0	may be text; had lf's without cr's
				1	text; has only crlf's
				2	file is binary

		note:	file is never changed.

		note:	if last character is ^Z, it's ok, as it
			is the END OF FILE character under MSDOS.

*/
static int chg_chk2()
{
	int hascrlf, haslf, ch, ch2 ;

	haslf= hascrlf= 0;
	while((ch= dskrdc())>= 0) {			/* while there's inp */
		if (ch == '\r') {			/* if is cr, examine */
			if ((ch2= dskrdc()) != '\n') {	/* next char for lf. */
				dskurdc(ch2) ;		/* if not, put back  */
			}
			else {				/* if so, write '\n' */
				ch= '\n' ;
				hascrlf= 1 ;		/* remember that has */
			}
		}
		else if (ch == '\n') {			/* if lf, remember   */
			haslf= 1 ;
			/* return(0) ; */		/* because only lf   */
		}
		else if ((ch != '\t') && ((ch< 0x20) || (ch>= 0x80))) {
			if (ch == ('Z'-'A'+1)) {	/* ok if last && ^Z  */
				if ((ch2= dskrdc()) == -1) break ;
				dskurdc(ch2) ;		/* ^Z is not last    */
			}
			return(2) ;			/* file is binary    */
		}
	}
	if (haslf) return(0) ;				/* text: only has lf */
	return(hascrlf? 1: 2) ;				/* binary if no lf's */
}

/*
	chg_rem2:		read file with crlf's; change crlf's to lf's

		returns:
				-2	can't write to new file
				0	may be text; had lf's without cr's
				1	text; has only crlf's
				2	means file is binary

		note:	file is changed only if return code is 1.

		note:	if last character is ^Z, it's ok, as it
			is the END OF FILE character under MSDOS.

*/
static int chg_rem2()
{
	int haslf, hascrlf, ch, ch2 ;

	haslf= hascrlf= 0;
	while((ch= dskrdc())>= 0) {			/* while there's inp */
		if (ch == '\r') {			/* if is cr, examine */
			if ((ch2= dskrdc()) != '\n') {	/* next char for lf. */
				dskurdc(ch2) ;		/* if not, put back  */
			}
			else {				/* if so, write '\n' */
				ch= '\n' ;
				hascrlf= 1 ;		/* remember that has */
			}
		}
		else if (ch == '\n') {			/* if lf, remember   */
			haslf= 1 ;
		}
		else if ((ch != '\t') && ((ch< 0x20) || (ch>= 0x80))) {
			if (ch == ('Z'-'A'+1)) {	/* ok if last && ^Z  */
				if ((ch2= dskrdc()) == (-1)) break ;
				dskurdc(ch2) ;		/* ^Z is not last    */
			}
			if (!opt.force) return(2) ;	/* file is binary    */
		}
		if (dskwrc(ch)) return(-2) ;		/* can't write 2file */
	}
	if (opt.force) return(1) ;			/* do it !	     */
	if (haslf) return(0) ;				/* text: has only lf */
	return(hascrlf? 1: 2) ;				/* binary if no lf's */
}

/*
	chg_add2:		read file with lf's; change lf's to crlf's

		returns:
				-2	can't write to new file
				0	text; had lf's
				1	may be text; already has crlf's
				2	binary file

		note: file is changed only if return code is 0.

*/
static int chg_add2()
{
	int haslf, hascrlf, ch, ch2 ;

	haslf= hascrlf= 0;
	while((ch= dskrdc())>= 0) {			/* while there's inp */
		if (ch == '\r') {				/* if is cr, examine */
			if ((ch2= dskrdc()) == '\n') {	/* next char for lf. */
				hascrlf= 1 ;		/* remember has crlf */
			}
			dskurdc(ch2) ;			/* put back next chr */
		}
		else if (ch == '\n') {			/* if is lf, change  */
			if (dskwrc('\r')) return(-2) ;	/* to cr/lf sequence */
			haslf= 1 ;			/* remember that has */
		}
		else if ((ch != '\t') && ((ch< 0x20) || (ch>= 0x80))) {
			if (!opt.force) return(2) ;	/* file is binary    */
		}
		if (dskwrc(ch)) return(-2) ;		/* can't write 2file */
	}
	if (opt.force) return(0) ;			/* do it!  	     */
	if (hascrlf) return(1) ;			/* text: with crlf   */
	return(haslf? 0: 2) ;				/* binary if no lf's */
}

static void dispose(kill, ifn, rfn, ofn, pfm)
int kill ;
char *ifn, *rfn, *ofn ;
struct fmode *pfm ;
{
	dskrstif(); dskrstof() ;		/* close both files,   */
	if (kill) {				/* keep old file, so   */
		unlink(ofn) ;			/* unlink output file  */
	}
	else {					/* keep new file, so   */
		unlink(ifn) ;			/* unlink input file   */
		l_rename(ofn, rfn) ;		/* move output 2 input */
		l_utime(rfn, pfm) ;		/* update times	       */
	}
}

static void mk_rfn(ifn, rfn)
char *ifn, *rfn ;
{
	strcpy(rfn, ifn) ;			/* create result filen */
	if (opt.cconv) fncase(rfn, opt.cconv) ;	/* case result filenam */
}

static int fn_exists(fn)
char *fn ;
{
	int fh ;

	if ((fh= open(fn, OPENRMASK))< 0) {
		return(0) ;
	}
	close(fh) ;
	return(1) ;
}

static void fncase(fn, ct)
char *fn ;
int ct ;
{
	switch(ct) {
		case 'U':
			while(*fn) {
				if (islower(*fn)) *fn= toupper(*fn) ;
				fn++ ;
			}
			break  ;
		case 'L':
			while(*fn) {
				if (isupper(*fn)) *fn= tolower(*fn) ;
				fn++ ;
			}
			break ;
	}
}

static void l_utime(fn, pfm)
char *fn ;
struct fmode *pfm ;
{
	char emsg[512] ;
	struct utimbuf uit ;

	uit.actime= pfm->actime ;
	uit.modtime= pfm->modtime ;

	if (utime(fn, &uit)) {
		sprintf(
			emsg,
			"warning: can't set timestamp for: '%s', errno=%d",
			fn,
			errno
		) ;
		clerror(emsg) ;
	}
}

static void l_rename(from_fn, to_fn)
char *from_fn, *to_fn ;
{
	char emsg[512] ;
	
	if (link(from_fn, to_fn) != 0) {
		sprintf(emsg, "cannot rename: '%s' to '%s'", from_fn, to_fn) ;
		clerror(emsg) ;
		clexit(1) ;
		return ;
	}
	unlink(from_fn) ;
}

static int mkofile(ofn, obuf, obuflen, mode)
char *ofn, *obuf ;
int obuflen, mode ;
{
	char emsg[512] ;
	int oldmask, ofh ;

	strcpy(ofn, "adXXXXXX") ;		/* create output file */
	mktemp(ofn) ;

	oldmask= umask(0) ;			/* clear user mask    */
	if ((ofh= open(ofn, OPENWMASK, mode))< 0) {
		umask(oldmask) ;		/* restore proc mask  */
		sprintf(emsg, "can't open for output: '%s'", ofn) ;
		clerror(emsg) ;
		return(1) ;			/* can't write output */
	}
	umask(oldmask) ;			/* restore proc mask  */

	dskrsto(ofh, obuf, obuflen) ;		/* setup output file  */
	return(0) ;				/* return no errors   */
}

static void getmode(fh, pfm)
int fh ;
struct fmode *pfm ;
{
	struct stat fs ;

	fstat(fh, &fs) ;
	pfm->mode= (int)fs.st_mode ;
	pfm->actime= fs.st_atime ;
	pfm->modtime= fs.st_mtime ;
}

static void syntax()
{
	static char *smsg[]= {
		"syntax:",
#if	OPSYS==OPMSDOS
		"\taddcr /{a|r|i|h} {/U|L} [/f] [/l] [/q] [/z] file [file ...]",
#else
		"\taddcr -{a|r|i|h} {-U|L} [-f] [-l] [-q] [-z] file [file ...]",
#endif
		"",
		"\ta\tadds CRs: converts file(s) to MSDOS format",
		"\tr\tremoves CRs: converts file(s) to Unix format",
		"\ti\tinspect: lists status of current file format",
		"\th\thelp: prints out this help message",
		"",
		"options:",
		"\tf\tforce file conversion",
		"\tl\tlists original file formats",
		"\tq\tquiet: suppresses signon banner",
		"\tz\tenables debug mode",
		"\tU\tconverts filenames to upper-case",
		"\tL\tconverts filenames to lower-case",
	} ;
#define	NSMSG	(sizeof(smsg) / sizeof(smsg[0]))

	int i ;

	for (i= 0; i< NSMSG; i++) {
		fprintf(stderr, "%s\n", smsg[i]) ;
	}
}

static void title()
{
	fprintf(
		stderr,
		"addcr 2.0.%d - MSDOS/Unix File conversion\n",
		Revision
	) ;
	fprintf(stderr, "Copyright (C) 1991-1995, by Marty Ross\n") ;
	fprintf(stderr, "\n") ;
}

/* end of addcr.c */
