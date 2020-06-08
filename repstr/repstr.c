#define Revision        31

/*

	MS-DOS Syntax:
		repstr /{s|i|h} [/f fnsl] [/q] [/z] file [file ...] [< fnsl]

	Unix Syntax:
		repstr -{s|i|h} [-f fnsl] [-q] [-z] file [file ...] [< fnsl]

	Commands:
		s	performs substitution
		i	inspect: does not perform substitution
		h	help: prints out this help message

	Options:
		f fnsl	use file 'fnsl' containing substitution list
			(else reads substitution list from stdin)
		l	list: lists substitutions as they happen
		q	quiet: suppresses signon banner
		z	enables debug mode

*/

#include        "compiler.h"
#include	<stdio.h>
#include        <memory.h>

#include	"clgetopt.h"
#include        "sublist.h"


#if	OPSYS==OPMSDOS
#define	unlink(fn)	remove(fn)
#define	link(from,to)	rename(from,to)
#endif

#define	TEXTRECMAX_LENGTH	2048
#define SLIMAX_ITEMS            512

/*
	structures
*/
struct options {
	int funct ;			/* main function selection	*/
	int list ;			/* listing substitutions?	*/
	int debug ;			/* debug statistics on?		*/
	int quiet ;			/* suppress title		*/
	char *fnsl ;			/* substitution list input file	*/
} ;

struct ftype {
	int nsub ;			/* number of substitutions	*/
        int nlchg ;                     /* number of lines changed      */
        int nfchg ;                     /* number of files changed      */
        int nscan ;                     /* number of files scanned      */
	int ntot ;			/* number of files total	*/
	SUBLISTITEM sli[SLIMAX_ITEMS] ;	/* substitution items		*/
	int nsli ;			/* number of " "		*/
	char *fn ;			/* file name			*/
	long line ;			/* file line			*/
} ;

/*
	static variables
*/
static struct options opt ;

/*
	forward declarations
*/

static void init(int argc, char **argv, int *sargp) ;
static void doit(int n, char **list) ;
static void doreport(struct ftype *ftp) ;
static int dochange(struct ftype *ftp) ;
static int chg_sub(char *rfn, struct ftype *ftp) ;
static int chg_chk(struct ftype *ftp) ;
static int chg_chk2(struct ftype *ftp) ;
static int chg_sub2(struct ftype *ftp) ;
static void dispose(int kill, char *ifn, char *rfn, char *ofn) ;
static void mk_rfn(char *ifn, char *rfn) ;
static int fn_exists(char *fn) ;
static void l_rename(char *from_fn, char *to_fn) ;
static int mkofile(char *ofn, char *obuf, int obuflen) ;
static void syntax() ;
static void title() ;
static void setifp(FILE *ifp) ;
static void closeifp() ;
static void setofp(FILE *ofp) ;
static void closeofp() ;
static int dosearch(int wrflg, struct ftype *ftp) ;
static int dosub(TSSTR *psLine, int maxlen, struct ftype *ftp) ;

/*
	main procedure
*/
void main(int argc, char **argv)
{
	int sarg ;

	init(argc, argv, &sarg) ;
	doit(argc-sarg, argv+sarg) ;
	clexit(0) ;
}

/*
	static procedures
*/

static void init(int argc, char **argv, int *sargp)
{
	int oc ;

	memset(&opt, '\0', sizeof(opt)) ;
	
	clinit(argv) ;
	while((oc= clgetopt(argc, argv, "sihlqzf:")) != EOF) {
		switch(oc) {

			case 's':
			case 'i':
			case 'h':
				if (opt.funct && (opt.funct != (int)oc)) {
					clerror("bad option combination") ;
					clexit(1) ;
				}
				opt.funct= (int) oc ;
				break ;

			case 'l':	opt.list= 1 ;		break ;
			case 'q':	opt.quiet= 1 ;		break ;
			case 'z':	opt.debug= 1 ;		break ;
			case 'f':	opt.fnsl= cloptarg ;	break ;

			default:	clerror("invalid option") ;
					clexit(1) ;		break ;
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

static void doit(int n, char **list)
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

        memset(&ft, '\0', sizeof(struct ftype)) ;
        ft.nsli= getsublist(ft.sli, SLIMAX_ITEMS, opt.fnsl) ;
        if (ft.nsli< 0) {
                fprintf(stderr, "error reading list: %d\n", ft.nsli) ;
                exit(1) ;
        }
        if (opt.debug) printf("%d items in list\n", ft.nsli) ;

	for (i= 0; i< n; i++) {
		ft.fn= list[i] ;
		stat= dochange(&ft) ;
		if (stat< -1) {
			clerror("error writing output") ;
			clexit(1) ;
		}
	}

	doreport(&ft) ;
}

static void doreport(struct ftype *ftp)
{
	char *pszInspect ;
	pszInspect= (opt.funct == 'i')? " would be" : "" ;

        printf("%d/%d files scanned\n", ftp->nscan, ftp->ntot) ;
        printf("%d file(s)%s changed\n", ftp->nfchg, pszInspect) ;
        printf("%d line(s)%s changed\n", ftp->nlchg, pszInspect) ;
        printf("%d substitution(s)%s performed\n", ftp->nsub, pszInspect) ;
}

/*

	rc= dochange(&ftype) ;

	rc:
		-2	means (output) file could not be written
		-1	means (input) file could not be processed
		0	means (input) file did not contain search string(s)
		1	means (input) file contained search string(s)
*/
static int dochange(struct ftype *ftp)
{
	int rc ;
	char rfn[512] ;
	char msg[512] ;
	FILE *ifp ;

	if (opt.debug) printf("file='%s'\n", ftp->fn) ;
	(ftp->ntot)++ ;

	if ((ifp= fopen(ftp->fn, "r")) == (FILE *) NULL) {
		sprintf(msg, "can't open: '%s'", ftp->fn) ;
		clerror(msg) ;
		return(-1) ;
	}

	mk_rfn(ftp->fn, rfn) ;
	if (strcmp(ftp->fn, rfn) != 0) {
		if (fn_exists(rfn)) {
			sprintf(
				msg,
				"can't update '%s'; '%s' exists",
				ftp->fn,
				rfn
			) ;
			clerror(msg) ;
			return(-1) ;
		}
	}

	setifp(ifp) ;

	rc= 0 ;
	switch(opt.funct) {
                case 's':       rc= chg_sub(rfn, ftp) ;     	break ;
                case 'i':       rc= chg_chk(ftp) ;              break ;
		default:
			clerror("option unimplemented") ;
			clexit(1) ;
			/*NOTREACHED*/
			break ;
 
	}

	return(rc) ;
}

/*
	rc:
		-2	means (output) file could not be written
		-1	means (input) file could not be processed
		0	means (input) file did not contain search string(s)
		1	means (input) file contained search string(s)
*/
static int chg_sub(char *rfn, struct ftype *ftp)
{
	char ofn[512], obuf[1024] ;
	int rc ;

	if (mkofile(ofn, obuf, sizeof(obuf))) {		/* out file	*/
		closeifp() ;
		return(-2) ;				/* can't write	*/
	}
	rc= chg_sub2(ftp) ;				/* do changes	*/
	dispose(rc != 1, ftp->fn, rfn, ofn) ;		/* update file	*/
	return(rc) ;	
}

/*
	rc:
		-2	means (output) file could not be written
		-1	means (input) file could not be processed
		0	means (input) file did not contain search string(s)
		1	means (input) file contained search string(s)
*/
static int chg_chk(struct ftype *ftp)
{
	int rc ;

	rc= chg_chk2(ftp) ;
	closeifp() ;
	return(rc) ;
}

/*
	chg_chk2:		read file; report about text contents

	rc:
		-1	means (input) file could not be processed
		0	means (input) file did not contain search string(s)
		1	means (input) file contained search string(s)

		note:	file is never changed.

		note:	if last character is ^Z, it's ok, as it
			is the END OF FILE character under MSDOS.

*/
static int chg_chk2(struct ftype *ftp)
{
	return(dosearch(0, ftp)) ;
}

/*
	chg_sub2:		read file with crlf's; change crlf's to lf's

	rc:
		-2	means (output) file could not be written
		-1	means (input) file could not be processed
		0	means (input) file did not contain search string(s)
		1	means (input) file contained search string(s)

	note:	file is changed only if return code is 1.

	note:	if last character is ^Z, it's ok, as it
		is the END OF FILE character under MSDOS.

*/
static int chg_sub2(struct ftype *ftp)
{
	return(dosearch(1, ftp)) ;
}

static void dispose(int kill, char *ifn, char *rfn, char *ofn)
{
	closeifp() ; closeofp() ;		/* close both files,   */
	if (kill) {				/* keep old file, so   */
		unlink(ofn) ;			/* unlink output file  */
	}
	else {					/* keep new file, so   */
		unlink(ifn) ;			/* unlink input file   */
		l_rename(ofn, rfn) ;		/* move output 2 input */
	}
}

static void mk_rfn(char *ifn, char *rfn)
{
	strcpy(rfn, ifn) ;			/* create result filen */
}

static int fn_exists(char *fn)
{
	FILE *fp ;
	if ((fp= fopen(fn, "r")) == (FILE *) NULL) return(0) ;
	fclose(fp) ;
	return(1) ;
}

static void l_rename(char *from_fn, char *to_fn)
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

static int mkofile(char *ofn, char *obuf, int obuflen)
{
	char emsg[512] ;
	FILE *ofp ;

        strcpy(ofn, "tsXXXXXX") ;               /* create output file */
	mktemp(ofn) ;

	if ((ofp= fopen(ofn, "w")) == (FILE *) NULL) {
		sprintf(emsg, "can't open for output: '%s'", ofn) ;
		clerror(emsg) ;
		return(1) ;			/* can't write output */
	}

	setofp(ofp) ;				/* setup output file  */
	return(0) ;				/* return no errors   */
}

static void syntax()
{
	static char *smsg[]= {
		"syntax:",
#if	OPSYS==OPMSDOS
	"\trepstr /{s|i|h} [/l] [/q] [/z] [/f fnsl] file [file ...] [< fnsl]",
#else
	"\trepstr -{s|i|h} [-l] [-q] [-z] [-f fnsl] file [file ...] [< fnsl]",
#endif
		"",
		"\ts\tperform substitutions",
		"\ti\tinspect: no substitutions performed",
		"\th\thelp: prints out this help message",
		"",
		"options:",
		"\tf fnsl\tuse substitution list file 'fnsl'",
		"\tl\tlists substitutions as they happen",
		"\tq\tquiet: suppresses signon banner",
		"\tz\tenables debug mode",
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
                "repstr 0.%d - text file substitution utility\n",
		Revision
	) ;
	fprintf(stderr, "Copyright (C) 1995, by Marty Ross\n") ;
	fprintf(stderr, "\n") ;
}

static FILE *g_ifp ;
static FILE *g_ofp ;

static void setifp(FILE *ifp) { g_ifp= ifp ; }
static void closeifp() { fclose(g_ifp) ; }
static void setofp(FILE *ofp) { g_ofp= ofp ; }
static void closeofp() { fclose(g_ofp) ; }

/*
	dosearch:		

	rc:
		-2	means (output) file could not be written
		-1	means (input) file could not be processed
		0	means (input) file did not contain search string(s)
		1	means (input) file contained search string(s)
*/

static int dosearch(int wrflg, struct ftype *ftp)
{
	char ibuf[TEXTRECMAX_LENGTH+1] ;
	char obuf[TEXTRECMAX_LENGTH+1] ;
        int rc, hassub ;
        TSSTR sLine ;

        hassub= 0 ;
        sLine.psz= obuf ;
	for (
		ftp->line= 1L;
		fgets(ibuf, TEXTRECMAX_LENGTH, g_ifp);
		ftp->line++
	) {
                sLine.len= strlen(ibuf) ;
                memcpy(sLine.psz, ibuf, sLine.len + 1) ;
                rc= dosub(&sLine, TEXTRECMAX_LENGTH, ftp) ;
                if (rc< 0) return(rc) ;
                if (rc> 0) {
                        hassub= 1 ;
                        ftp->nlchg++ ;
                        ftp->nsub+= rc ;
			if (opt.list) {
				printf(
					"%s(%ld): %d change(s):\n",
					ftp->fn,
					(long) ftp->line,
					(int) rc
				) ;
				printf("%s", ibuf) ;
				printf("%s", sLine.psz) ;
			}
                }
                if (wrflg) fputs(sLine.psz, g_ofp) ;
	}
        if (hassub) ftp->nfchg++ ;
        ftp->nscan++ ;
        return(hassub) ;
}

static int dosub(TSSTR *psLine, int maxlen, struct ftype *ftp)
{
        int i, j, k, rc ;

        k= 0 ;
        for (i= 0; i< ftp->nsli; i++) {
                j= 0 ;
                while(/*CONSTCOND*/1) {
                        rc= tsub(
                                &ftp->sli[i].sFrom,
                                &ftp->sli[i].sTo,
                                psLine,
                                &j,
                                maxlen
                        ) ;
                        if (rc< 0) {
                                clerror("line overflow") ;
                                return(-1) ;
                        }
                        if (rc == 0) break ;
			if (opt.debug) {
				printf(
					"%s:(%ld:%d) '%s' => '%s'\n",
					ftp->fn,
					(long) ftp->line,
					(int) (j - ftp->sli[i].sTo.len),
					ftp->sli[i].sFrom.psz,
					ftp->sli[i].sTo.psz
				) ;
			}
                        k++ ;
                }
        }
        return(k) ;
}

/* end of repstr.c */
