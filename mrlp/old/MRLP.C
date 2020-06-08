/*

	mrlp.c:				Marty's Remote Line Printer 
					Copyright (C) 1992, Contahal Ltd.


*/

#include	<stdio.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<errno.h>

#include	<sys/file.h>

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"mrlpsub.h"

struct lpm_opts {
	int help ;
	int quiet ;
	int debug ;
	int xhelp ;
	int testhost ;
	char *host ;
	char *serv ;
	char *lpcmd ;
	char *lpopt ;
} ;

#define	OPT_QUIET	(lpmo.quiet)
#define	OPT_DEBUG	(lpmo.debug)
#define	OPT_HOST	(lpmo.host)
#define	OPT_SERV	(lpmo.serv)
#define	OPT_HELP	(lpmo.help)
#define	OPT_LPCMD	(lpmo.lpcmd)
#define	OPT_LPOPT	(lpmo.lpopt)
#define	OPT_XHELP	(lpmo.xhelp)
#define	OPT_TESTHOST	(lpmo.testhost)

#define	RSOCK		(lpma.rsock)
#define	RSOCK_A		(lpma.rsock_a)
#define	EXIT_RC		(lpma.exit_rc)
#define	JOBREQA		(lpma.jobreqa)
#define	NJOBS		(lpma.njobs)

struct lpm_arg {
	int rsock ;				/* remote socket descriptor */
	SOCKA_T rsock_a ;			/* remote socket address    */
	char **jobreqa ;			/* job file name array      */
	int njobs ;				/* number of job files      */
	int exit_rc ;				/* set to exit return code  */
	char lpm_lphost[MRLP_HNBSIZE+1] ;	/* name of remote host      */
	char lpm_lpserv[MRLP_SNBSIZE+1] ;	/* name of command service  */
	char lpm_lpproto[MRLP_PNBSIZE+1] ;	/* protocol of command serv */
	char xhelpfn[MRLP_FNBSIZE+1] ;		/* extended help file       */
} ;

static void setdefaults() ;
static void init() ;
static void setup() ;
static void takedown() ;
static void make_request() ;
static void catchsig() ;
static void verify_cmdmsg() ;
static void send_cmdmsg() ;
static void wait_cmdreply() ;
static void signon() ;
static void syntax() ;
static void xhelp() ;

static struct lpm_arg lpma ;
static struct lpm_opts lpmo ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;

	/*
		set default parameter values
	*/
	setdefaults() ;

	/*
		parse user command line/options, etc
	*/
	init(argc, argv) ;

	/*
		setup internal values, and connect to
		to remote printer server.
	*/
	setup() ;

	/*
		submit the print jobs
	*/
	for (i= 0; i< NJOBS; i++) {
		make_request(JOBREQA[i]) ;
	}

	/*
		release the server,
		close the remote connection,
		do other "de-installation"
	*/
	takedown() ;

	exit(EXIT_RC) ;
}
	
static void setup()
{
	char *p, *getenv() ;
	int bad, rc ;

	/*
		initialize arguments from environment, options
	*/
	if ((p= OPT_HOST) == (char *)NULL) p= getenv(MRLPD_EVLPHOST) ;
	if (p != (char *)NULL) strncpy(lpma.lpm_lphost, p, MRLP_HNBSIZE) ;
	if ((p= OPT_SERV) == (char *)NULL) p= getenv(MRLPD_EVLPSERV) ;
	if (p != (char *)NULL) strncpy(lpma.lpm_lpserv, p, MRLP_SNBSIZE) ;
	if ((p= getenv(MRLP_EVXHELPFN)) != (char *)NULL) {
		strncpy(lpma.xhelpfn, p, MRLP_FNBSIZE) ;
	}

	/*
		check arguments
	*/
	if ((NJOBS == 0) && (OPT_TESTHOST == 0)) {
		mrlp_abort("no files specified; use 'h' option for help") ;
	}

	if ( (lpma.lpm_lphost[0]=='\0') || (lpma.lpm_lpserv[0]=='\0') 
	) {
		mrlp_abort("remote print server address not fully specified") ;
	}

	/*
		get network addresses of sockets
	*/
	if (mrlp_getnh(&RSOCK_A, lpma.lpm_lphost)) {
		mrlp_abort("can't get remote printer host") ;
		/*NOTREACHED*/
	}

	if (mrlp_getnp(&RSOCK_A, lpma.lpm_lpserv, lpma.lpm_lpproto)) {
		mrlp_abort("can't get remote printer command service port") ;
		/*NOTREACHED*/
	}

	/*
		connect to server and setup
		print command, options
	*/
	signal(SIGPIPE, catchsig) ;
	if (waitconnect(&RSOCK, &RSOCK_A, 5)) {
		mrlp_abort("can't connect to remote host") ;
		/*NOTREACHED*/
	}

	verify_cmdmsg(MRLPCMD_VERCHK, MRLPD_VER, "version check", &p) ;
	
	if (OPT_TESTHOST) {
		verify_cmdmsg(MRLPCMD_TESTHOST, "", "test host cmd", &p) ;
		mrlp_smsg("remote daemon", p) ;
		return ;
	}

	if (OPT_LPCMD != (char *)NULL) {
		verify_cmdmsg(MRLPCMD_LPCMD, OPT_LPCMD, "lp command", &p) ;
	}

	if (OPT_LPOPT != (char *)NULL) {
		verify_cmdmsg(MRLPCMD_LPOPT, OPT_LPOPT, "lp option", &p) ;
	}

}

static void takedown()
{
	char *arg_p ;

	verify_cmdmsg(MRLPCMD_EXIT, "", "exit", &arg_p) ;
	close(RSOCK) ;
	signal(SIGPIPE, SIG_DFL) ;
}

static void setdefaults()
{
	memset((char *)&lpma, '\0', sizeof(lpma)) ;
	memset((char *)&lpmo, '\0', sizeof(lpmo)) ;
	strcpy(lpma.lpm_lpserv, MRLPD_SCMD) ;
	strcpy(lpma.lpm_lpproto, MRLPD_PCMD) ;
	strcpy(lpma.xhelpfn, MRLP_XHELPFN) ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;
	char pname[MRLP_NAMEBSIZE+1] ;

	clinit(argv) ;
	clcmdname(pname, argv[0]) ;
	mrlp_setname(pname) ;

	while((i= clgetopt(argc, argv, "c:hHm:o:qtz"))>= 0) {
		switch(i) {
			case 'c': OPT_LPCMD= cloptarg ;		break ;
			case 'h': OPT_HELP= 1 ;			break ;
			case 'H': OPT_XHELP= 1 ;		break ;
			case 'm': OPT_HOST= cloptarg ;		break ;
			case 'o': OPT_LPOPT= cloptarg ;		break ;
			case 'q': OPT_QUIET= 1 ;		break ;
			case 't': OPT_TESTHOST= 1 ;		break ;
			case 'z': OPT_DEBUG= 1 ;		break ;
			default:  exit(1) ;			break ;
		}
	}	

	if (OPT_HELP) {
		signon() ;
		syntax() ;
		exit(0) ;
	}

	if (OPT_XHELP) {
		signon() ;
		xhelp() ;
		exit(0) ;
	}

	JOBREQA= argv + cloptind ;	
	NJOBS=   argc - cloptind ;
}

static void catchsig(signo)
int signo ;
{
	mrlp_nerr("signal caught", signo) ;
}

static void make_request(fn)
char *fn ;
{
	int fh, rc ;

	if ((fh= open(fn, O_RDONLY))< 0) {
		mrlp_perr(fn) ;
		EXIT_RC++ ;
		return ;
	}
	if (rc= process_request(fn, fh)) {
		mrlp_nerr("process_request", rc) ;
	}
	close(fh) ;
}


static int process_request(fn, fh)
char *fn ;
{
	char iobuf[MRLP_IOBSIZE] ;
	char msgbuf[MRLP_MSGBSIZE+1] ;
	char *arg_p ;
	int rc, len ;
	int dport_in, dport_out ;
	int rsockdin, rsockdout ;
	SOCKA_T rsockdin_a, rsockdout_a ;

	/*
		print the file:

		(1)	command the server to print the current file.
		(2)	open connection on the data socket.
		(3)	send the file on the data socket.
		(4)	read and display any data from the socket.
		(5)	wait for server return result code.
	*/
	verify_cmdmsg(MRLPCMD_PRINT, fn, "print", &arg_p) ;	/* (1) */
	if (!OPT_QUIET) {
		if (OPT_DEBUG) {
			sprintf(msgbuf, "printing: %s (%s)", fn, arg_p) ;
			mrlp_msg(msgbuf) ;
		}
		else {
			mrlp_smsg("printing", fn) ;
		}
	}

	if (sscanf(arg_p, "%d:%d", &dport_in, &dport_out) != 2) {
		mrlp_abort("can't get data port #s") ;
		/*NOTREACHED*/
	}
	memcpy((char *)&rsockdin_a, (char *)&RSOCK_A, sizeof(SOCKA_T)) ;
	SOCKA_SPORT(rsockdin_a)= htons((u_short)dport_in) ;
	memcpy((char *)&rsockdout_a, (char *)&RSOCK_A, sizeof(SOCKA_T)) ;
	SOCKA_SPORT(rsockdout_a)= htons((u_short)dport_out) ;

	/*
		connect to the data sockets, and send the file.
	*/
	if (waitconnect(&rsockdin, &rsockdin_a, 5)) {		/* (2) */
		mrlp_abort("can't connect to data-in socket") ;
		/*NOTREACHED*/
	}

	if (waitconnect(&rsockdout, &rsockdout_a, 5)) {		/* (2) */
		mrlp_abort("can't connect to data-out socket") ;
		/*NOTREACHED*/
	}

	while(/*CONSTCOND*/1) {					/* (3) */
		if (OPT_DEBUG) mrlp_msg("reading") ;
		if ((len= read(fh, iobuf, sizeof(iobuf)))<= 0) break ;
		if (OPT_DEBUG) mrlp_msg("writing") ;
		if (mrlp_wsock(rsockdin, iobuf, len, "file data")) break ;
		if (OPT_DEBUG) mrlp_msg("checking") ;
		if ((len= mrlp_rsock(rsockdout, iobuf, sizeof(iobuf), 0))> 0) {
			write(1, iobuf, len) ;
		}
	}
	if (OPT_DEBUG) mrlp_msg("closing data-in socket") ;
	close(rsockdin) ;

	if (OPT_DEBUG) mrlp_msg("reading data-out socket") ;
	while((len= read(rsockdout, iobuf, sizeof(iobuf)))> 0) {	
		write(1, iobuf, len) ;
	}

	if (OPT_DEBUG) mrlp_msg("closing data-out socket") ;
	close(rsockdout) ;					/* (4) */

	if (OPT_DEBUG) mrlp_msg("sockets closed") ;

	verify_cmdmsg(MRLPCMD_JOBCHK, "", "print rc", &arg_p) ;	/* (5) */
	if (sscanf(arg_p, "%d", &rc) != 1) rc= 1 ;
	if (rc != 0) mrlp_smsg("NOTE: remote print exit code", arg_p) ;

	return(0) ;
}

static int waitconnect(csock_p, csocka_p, n_retries)
int *csock_p ;
SOCKA_T *csocka_p ;
int n_retries ;
{
	int i, s ;
	char msgbuf[MRLP_MSGBSIZE+1] ;

	if (OPT_DEBUG) {
		mrlp_smsg(
			"trying to connect",
			mrlp_inetaddr_str(csocka_p)
		) ;
	}
	for (i= 0; i< n_retries; i++) {
		if ((s= socket(AF_INET, SOCK_STREAM, 0))< 0) {
			mrlp_perr("socket") ;
			return(1) ;
		}
		if (connect(s, csocka_p, sizeof(SOCKA_T)) == 0) {
			(*csock_p)= s ;
			return(0) ;
		}
		if (errno != ECONNREFUSED) break ;
		close(s) ;
		sleep(1) ;
	}

	sprintf(msgbuf, "connecting to: %s", mrlp_inetaddr_str(csocka_p)) ;
	mrlp_perr(msgbuf) ;
	return(2) ;
}

static void verify_cmdmsg(cmd, arg, desc, argp_p)
char *cmd, *arg, *desc, **argp_p ;
{
	send_cmdmsg(cmd, arg, desc) ;
	wait_cmdreply(argp_p, desc) ;
}

static void send_cmdmsg(cmd, arg_p, ab_desc)
char *cmd, *arg_p, *ab_desc ;
{
	int len ;
	char cmdbuf[MRLP_CMDBSIZE+1] ;

	sprintf(cmdbuf, "%s%s\n", cmd, arg_p) ;
	len= strlen(cmdbuf) ;
	if (OPT_DEBUG) mrlp_smsg("send cmd", cmd) ;
	if (mrlp_wsock(RSOCK, cmdbuf, len, "send_cmdmsg")) {
		mrlp_abort(ab_desc) ;
		/*NOTREACHED*/
	}
}

static void wait_cmdreply(argp_p, ab_desc)
char **argp_p, *ab_desc ;
{
	/* MUST be static because pass it back (*argp_p) to caller */
	static char cmdbuf[MRLP_CMDBSIZE+1] ;

	if (OPT_DEBUG) mrlp_smsg("wait for reply", ab_desc) ;

	if (mrlp_getline(RSOCK, cmdbuf, MRLP_CMDBSIZE)) {
		mrlp_err("wait_cmdreply: unexpected EOF!") ;
		mrlp_abort(ab_desc) ;
		/*NOTREACHED*/
	}

	if (OPT_DEBUG) mrlp_smsg("server's reply", cmdbuf) ;

	if (mrlp_iscmdmsg(cmdbuf, MRLPCMDR_ABORT, argp_p)) {
		mrlp_abort(*argp_p) ;
		/*NOTREACHED*/
	}

	if (!mrlp_iscmdmsg(cmdbuf, MRLPCMDR_OK, argp_p)) {
		mrlp_serr("wait_cmdreply: invalid msg", cmdbuf) ;
		mrlp_abort(ab_desc) ;
		/*NOTREACHED*/
	}

}

static void signon()
{
	fprintf(STDTTY, "mrlp - Marty's Remote Line Printer driver\n") ;
	fprintf(STDTTY, "Ver 1.00 - Copyright (C) 1992, Contahal Ltd.\n") ;
	fprintf(STDTTY, "\n") ;
}

static void syntax()
{
static char *smsg[]= {
	"syntax:",
	"\tmrlp [-options] fn [fn ...]",
	"",
	"options:",
	"\tc \"lp cmd\"\tSpecify remote line printer command",
	"\th\t\tPrint this help text",
	"\tH\t\tSee on-line documentation, if available",
	"\tm HOST\t\tSpecify name of HOST on which printer resides",
	"\to \"lp opt\"\tSpecify remote line printer command option(s)",
	"\tq\t\tSuppress informatory output",
	"\tt\t\tTest for state of remote printer",
	"\tz\t\tEnable debug messages",
	"",
	"Sends specified files to remote printer.  Requires installed daemon",
	"on remote system, \"mrlpd\".  To check if remote daemon is active,",
	"use the 't' option.  To receive more extensive help with using the",
	"command, use the 'H' option."
} ;
#define	N_SMSG	(sizeof(smsg)/sizeof(smsg[0]))
	int i ;

	for (i= 0; i< N_SMSG; i++) {
		fprintf(STDTTY, "%s\n", smsg[i]) ;
	}
}

static void xhelp()
{
	char syscmdbuf[1024+1] ;

	sprintf(syscmdbuf, "more %s", lpma.xhelpfn) ;
	system(syscmdbuf) ;
}
