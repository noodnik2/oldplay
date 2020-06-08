/*

	terminal driver - file I/O

*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"term.h"
#include	"ttyio.h"

extern int stopflag ;
extern struct termopts term_o ;

#define		CTL(x)	(x-'A'+1)
#define		RDMODE	(O_RDONLY)
#define		WRMODE	(O_WRONLY|O_CREAT|O_TRUNC)
#define		WRFLAGS	(S_IREAD|S_IWRITE)

static char abortmsg[]= "\n-- file xfer aborted --\n" ;

void mkrcmd(), mklcmd() ;

int std_sendfile(lfn, rfn)
char *lfn, *rfn ;
{
	int rc ;

	fprintf(stderr, "\n") ;
	switch(term_o.xfer) {
		case XFER_TEXT:	rc= sendfile_text(lfn, rfn) ;	break ;
		case XFER_XMT:	rc= sendfile_xmtext(lfn, rfn) ;	break ;
		case XFER_XMB:	rc= sendfile_xmbin(lfn, rfn) ;	break ;
	}
	return(rc) ;
}

int std_rcvfile(rfn, lfn)
char *rfn, *lfn ;
{
	int rc ;

	fprintf(stderr, "\n") ;
	switch(term_o.xfer) {
		case XFER_TEXT:	rc= rcvfile_text(lfn, rfn) ;	break ;
		case XFER_XMT:	rc= rcvfile_xmtext(lfn, rfn) ;	break ;
		case XFER_XMB:	rc= rcvfile_xmbin(lfn, rfn) ;	break ;
	}
	return(rc) ;
}

static int sendfile_text(lfn, rfn)
char *lfn, *rfn ;
{
	char iobuf[512] ;
	long nbytes ;
	int len, ifh ;

	if (stopflag) return(1) ;

	if ((ifh= open(lfn, RDMODE))<= 0) {
		fprintf(stderr, "--Can't open: %s\n", lfn) ;
		return(1) ;
	}

	fprintf(stderr, "--sending text file: %s--(^C to abort)--\n", lfn) ;

	t_start(T_CH, TM_TANDEM, TM_OPOST) ;

	sprintf(iobuf, "\rstty -echo\r") ;
	t_outs(iobuf, strlen(iobuf)) ;

	sprintf(iobuf, "\rcat > %s\r", rfn) ;
	t_outs(iobuf, strlen(iobuf)) ;

	nbytes= 0L ;
	do {

		if (stopflag) break ;
		len= read(ifh, iobuf, sizeof(iobuf)) ;
		t_outs(iobuf, len) ;
		nbytes+= (long) len ;

	} while(len == sizeof(iobuf)) ;
	close(ifh) ;

	if (stopflag) t_outs(abortmsg, strlen(abortmsg)) ;

	t_outc(CTL('D')) ;

	sprintf(iobuf, "\rstty echo\r\r") ;
	t_outs(iobuf, strlen(iobuf)) ;

	t_stop(T_CH) ;

	if (stopflag) {
		fprintf(stderr, "--send aborted!: %s\n", lfn) ;
		stopflag= 0 ;
		return(1) ;
	}

	fprintf(stderr, "--send complete: %s--(%ld bytes)--\n", lfn, nbytes) ;
	return(0) ;
}	

static int rcvfile_text(lfn, rfn)
char *lfn, *rfn ;
{
	char iobuf[512] ;
	int ofh, ch, infile ;
	char startc, stopc, c ;
	long nbytes ;

	if (stopflag) return(1) ;

	if ((ofh= open(lfn, RDMODE))>= 0) {
		fprintf(stderr, "--Already exists: %s\n", lfn) ;
		close(ofh) ;
		return(1) ;
	}

	if ((ofh= open(lfn, WRMODE, WRFLAGS))<= 0) {
		fprintf(stderr, "--Can't open: %s\n", lfn) ;
		return(1) ;
	}

	fprintf(stderr, "--receiving text file: %s--(^C to abort)--\n", lfn) ;

	t_start(T_CH, TM_TANDEM, TM_OPOST) ;

	startc= CTL('A') ;
	stopc= CTL('B') ;

	sprintf(iobuf,
		"\r(echo \"%c\\c\"; cat %s; echo \"%c\")\r",
		(int)startc, rfn, (int)stopc
	) ;

	t_outs(iobuf, strlen(iobuf)) ;

	nbytes= 0L ;
	for (infile= 0; stopflag==0;) {
		if ((ch= t_inc()) != (-1)) {
			c= (char) ch ;
			if (infile) {
				if (c==stopc) {
					infile= 0 ;
					break ;
				}
				if (c==CTL('M')) continue ;
				write(ofh, &c, 1) ;
				nbytes++ ;
			}
			else {
				if (c==startc) infile= 1 ;
			}
		}
	}

	t_stop(T_CH) ;

	if (stopflag) {
		write(ofh, abortmsg, strlen(abortmsg)) ;
	}

	close(ofh) ;

	if (stopflag) {
		fprintf(stderr, "--receive aborted!: %s\n", lfn) ;
		stopflag= 0 ;
		return(1) ;
	}

	fprintf(stderr, "--receive complete: %s--(%ld bytes)--\n",lfn,nbytes) ;
	return(0) ;
}

static int sendfile_xmtext(lfn, rfn)
char *lfn, *rfn ;
{
	int rc ;
	char rcmd[512], lcmd[512] ;

	mklcmd(lcmd, "-s") ;
	mkrcmd(rcmd, "-r") ;
	rc= xferfile_xm(lfn, rfn, lcmd, rcmd) ;
	return(rc) ;
}

static int sendfile_xmbin(lfn, rfn)
char *lfn, *rfn ;
{
	int rc ;
	char rcmd[512], lcmd[512] ;

	mklcmd(lcmd, "-S") ;
	mkrcmd(rcmd, "-R") ;
	rc= xferfile_xm(lfn, rfn, lcmd, rcmd) ;
	return(rc) ;
}

static int rcvfile_xmtext(lfn, rfn)
char *lfn, *rfn ;
{
	int rc ;
	char rcmd[512], lcmd[512] ;

	mklcmd(lcmd, "-r") ;
	mkrcmd(rcmd, "-s") ;
	rc= xferfile_xm(lfn, rfn, lcmd, rcmd) ;
	return(rc) ;
}

static int rcvfile_xmbin(lfn, rfn)
char *lfn, *rfn ;
{
	int rc ;
	char rcmd[512], lcmd[512] ;

	mklcmd(lcmd, "-R") ;
	mkrcmd(rcmd, "-S") ;
	rc= xferfile_xm(lfn, rfn, lcmd, rcmd) ;
	return(rc) ;
}

/*
	mklcmd(buf, option)

	makes the local command, with option specified
*/
static void mklcmd(buf, opt)
char *buf, *opt ;
{
	char *tname0, *tname1 ;

	t_name(T_REMOTE, &tname0, &tname1) ;
	if (tname1 == NULL) tname1= tname0 ;
	sprintf(buf, "xm %s < %s > %s", opt, tname0, tname1) ;
	if (term_o.debug) fprintf(stderr, "--debug: lcmd='%s'\n", buf) ;
}

/*
	mkrcmd(buf, option)

	makes the remote command, with option specified
*/
static void mkrcmd(buf, option)
char *buf, *option ;
{
	sprintf(buf, "xm %s", option) ;
	if (term_o.debug) fprintf(stderr, "--debug: rcmd='%s'\n", buf) ;
}

/*
	xferfile_xm:		perform an XMODEM "conversation"

		starts the XMODEM process on the remote side,
		then starts the XMODEM process on the local.

*/
static int xferfile_xm(lfn, rfn, mycmd, hiscmd)
char *lfn, *rfn, *mycmd, *hiscmd ;
{
	int rc ;
	char rcmd[512], lcmd[512] ;
	char spaces[2] ;

	spaces[0]= ' '; spaces[1]= '\0';
	strcpy(rcmd, hiscmd); strcat(rcmd, spaces); strcat(rcmd, rfn) ;
	strcpy(lcmd, mycmd); strcat(lcmd, spaces); strcat(lcmd, lfn) ;

	fprintf(stderr, "--transferring file: %s--(^C to abort)--\n", lfn) ;

	t_start(T_LOCAL, TM_OPOST, 0) ;
	t_start(T_REMOTE, 0, TM_OPOST|TM_TANDEM) ;

						/* start up remote side */
	t_outc('\n') ;
	t_outs(rcmd, strlen(rcmd)) ;
	t_outc('\n') ;
						/* start up local side  */
	sleep(2) ;
	while(t_inc() != (-1)) ;

	rc= system(lcmd) ;
	t_stop(T_CH) ;

	return(rc) ;
}
