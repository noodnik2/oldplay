/*


	mrlpd.c:			Marty's Remote Line Printer Daemon
					Copyright (C) 1992, Contahal Ltd.


*/

#include	<stdio.h>
#include	<signal.h>
#include	<netdb.h>
#include	<time.h>

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/wait.h>

#include	<netinet/in.h>

#include	"compiler.h"
#include	"clgetopt.h"
#include	"mrlpsub.h"
#include	"substr.h"

#define		MAX_ERRS	10

struct lpm_opts {
	int help ;
	int quiet ;
	int debug ;
	int nofork ;
	char *lpcmd ;
	char *lpargs ;
	char *lpopts ;
	char *lpserv ;
} ;

#define	OPT_HELP	(lpmo.help)
#define	OPT_QUIET	(lpmo.quiet)
#define	OPT_DEBUG	(lpmo.debug)
#define	LPT_LPCMD	(lpmo.lpcmd)
#define	OPT_LPARGS	(lpmo.lpargs)
#define	OPT_LPOPTS	(lpmo.lpopts)
#define	OPT_LPCMD	(lpmo.lpcmd)
#define	OPT_LPSERV	(lpmo.lpserv)
#define	OPT_NOFORK	(lpmo.nofork)

#define	RSOCK		(lpma.rsock)
#define	LSOCK		(lpma.lsock)
#define	LSOCKA		(lpma.lsocka)
#define	ERRS		(lpma.errs)
#define	DONE		(lpma.done)
#define	EXIT_RC		(lpma.exit_rc)
#define	JOBNO		(lpma.jobno)
#define	FILENO		(lpma.fno)
#define	LP_PID		(lpma.lp_pid)
#define	JOB_PID		(lpma.job_pid)
#define	JOB_RC		(lpma.job_rc)

struct lpm_arg {
	long jobno ;				/* job number               */
	int fno ;				/* file number within job   */
	int errs ;				/* number of errors in row  */
	int done ;				/* set to nonzero to exit   */
	int exit_rc ;				/* set to exit return code  */
	int job_rc ;				/* returncode from last job */
	pid_t job_pid ;				/* current server pid       */
	pid_t lp_pid ;				/* current lp process id    */
	int rsock ;				/* recieved command socket  */
	int lsock ;				/* listening command socket */
	SOCKA_T lsocka ;			/* listening cmd sock addr  */

	char lpm_lpserv[MRLP_SNBSIZE+1] ;	/* name of command service  */
	char lpm_lpproto[MRLP_PNBSIZE+1] ;	/* protocol of command serv */

	char lpm_lpexec[MRLP_LECBSIZE+1] ;	/* line printer command     */
	char lpm_lpopts[MRLP_LEOBSIZE+1] ;	/* line printer optinos     */
	char lpm_lpargs[MRLP_LEABSIZE+1] ;	/* line printer arguments   */

	char cur_lpexec[MRLP_LECBSIZE+1] ;	/* current lp command	    */
	char cur_lpopts[MRLP_LEOBSIZE+1] ;	/* current lp options	    */
	char cur_lpargs[MRLP_LEABSIZE+1] ;	/* current lp arguments     */
	char cur_lpfn[MRLP_LENBSIZE+1] ;	/* current lpr remote fn    */

	char lpm_start_time[25] ;		/* time daemon started      */
	char lpm_pname[MRLP_NAMEBSIZE+1] ;	/* program name		    */

} ;

static void setdefaults() ;
static void init() ;
static void setup() ;
static void takedown() ;
static void accept_request() ;
static void inc_errs() ;
static void kill_lpjob() ;
static void sigcatch() ;

static void mrlp_abortjob() ;
static void makecmd_subst() ;

static struct lpm_arg lpma ;
static struct lpm_opts lpmo ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	setdefaults() ;

	init(argc, argv) ;

	setup() ;

	if (!OPT_QUIET) mrlp_msg("daemon started") ;
	if (OPT_DEBUG) mrlp_nmsg("debug on", "level", OPT_DEBUG) ;
	for (JOBNO= 1L; !DONE; JOBNO++) {
		accept_request() ;
	}

	if (!OPT_QUIET) mrlp_msg("daemon stopped") ;
	exit(EXIT_RC) ;
}
	
static void setdefaults()
{
	time_t st ;

	memset((char *)&lpma, '\0', sizeof(lpma)) ;
	memset((char *)&lpmo, '\0', sizeof(lpmo)) ;
	strcpy(lpma.lpm_lpargs, "lp -t\"@FN\" @OPT") ;
	strcpy(lpma.lpm_lpserv, MRLPD_SCMD) ;
	strcpy(lpma.lpm_lpproto, MRLPD_PCMD) ;

	time(&st) ;
	memcpy(lpma.lpm_start_time, ctime(&st), 24) ;
	lpma.lpm_start_time[24]= '\0' ;
}

static void init(argc, argv)
int argc ;
char *argv[] ;
{
	int i ;

	clinit(argv) ;
	clcmdname(lpma.lpm_pname, argv[0]) ;
	mrlp_setname(lpma.lpm_pname) ;

	while((i= clgetopt(argc, argv, "a:c:Fhqz"))>= 0) {
		switch(i) {
			case 'a': OPT_LPARGS= cloptarg ;	break ;
			case 'c': OPT_LPCMD= cloptarg ;		break ;
			case 'F': OPT_NOFORK= 1 ;		break ;
			case 'h': OPT_HELP= 1 ;			break ;
			case 'q': OPT_QUIET= 1 ;		break ;
			case 'z': OPT_DEBUG++ ;			break ;
			default:  exit(1) ;			break ;	
		}
	}	

	if (OPT_HELP) {
		clerror("no help available at this time") ;
		exit(0) ;
	}
}

static void setup()
{
	char *p, *getenv() ;

	/*
		apply environment values,
		then user options
	*/
	if ((p= OPT_LPCMD) == (char *)NULL) p= getenv(MRLPD_EVLPCMD) ;
	if (p != (char *)NULL) strncpy(lpma.lpm_lpexec, p, MRLP_LECBSIZE) ;
	if ((p= OPT_LPARGS) == (char *)NULL) p= getenv(MRLPD_EVLPARGS) ;
	if (p != (char *)NULL) strncpy(lpma.lpm_lpargs, p, MRLP_LEABSIZE) ;
	if ((p= OPT_LPOPTS) == (char *)NULL) p= getenv(MRLPD_EVLPOPTS) ;
	if (p != (char *)NULL) strncpy(lpma.lpm_lpopts, p, MRLP_LEOBSIZE) ;
	if ((p= OPT_LPSERV) == (char *)NULL) p= getenv(MRLPD_EVLPSERV) ;
	if (p != (char *)NULL) strncpy(lpma.lpm_lpserv, p, MRLP_SNBSIZE) ;

	/*
		get socket addresses of all sockets we will
		listen on...
	*/
	SOCKA_SADDR(LSOCKA)= INADDR_ANY ;
	if (mrlp_getnp(&LSOCKA, lpma.lpm_lpserv, lpma.lpm_lpproto)) {
		mrlp_abort("can't get remote printer command service port") ;
	}

	/*
		issue listen on command socket
	*/
	if (OPT_DEBUG) mrlp_msg("issue listen on command socket") ;
	if (issuelisten(&LSOCK, &LSOCKA)) {
		mrlp_abort("can't listen for connections") ;
		/*NOTREACHED*/
	}

	/*
		monitor certain signals
	*/
	signal(SIGPIPE, sigcatch) ;
	signal(SIGTERM, sigcatch) ;
	signal(SIGHUP, sigcatch) ;

#if 0

/* "atexit()" function not available on sun-Sparc */

	/*
		ensure that our "de-install" program will be called.

		I don't think this is right: because both the daemon
		AND the lp printer processes can execute the takedown
		procedure according to this...
	*/
	atexit(takedown) ;
#endif
}

static void takedown()
{
	kill_lpjob() ;
}

static void accept_request()
{
	SOCKA_T rsocka ;
	int forked ;
	pid_t fork(), getpid() ;

	if (OPT_DEBUG) mrlp_msg("wait for connection to command socket") ;
	if (waitaccept(LSOCK, &RSOCK, &rsocka, 0)) return ;
	if (!OPT_QUIET) mrlp_smsg("connect", mrlp_inetaddr_str(&rsocka)) ;

	if (forked= !OPT_NOFORK) {
		signal(SIGCHLD, SIG_IGN) ;
		if ((JOB_PID= fork())< (pid_t)0) {
			mrlp_perr("can't fork job") ;
			forked= 0 ;
		}
	}

	if (forked) {
		if (JOB_PID != (pid_t) 0) {		/* parent */
			close(RSOCK) ;
			if (!OPT_QUIET) {
				mrlp_nmsg("job forked", "pid", (int)JOB_PID) ;
			}
			return ;
		}
		close(LSOCK) ;
		mrlp_setid((long)getpid()) ;
		if (OPT_DEBUG) mrlp_msg("in job process") ;
	}

	ERRS= 0 ;
	memcpy(lpma.cur_lpexec, lpma.lpm_lpexec, MRLP_LECBSIZE) ;
	memcpy(lpma.cur_lpopts, lpma.lpm_lpopts, MRLP_LEABSIZE) ;
	memcpy(lpma.cur_lpargs, lpma.lpm_lpargs, MRLP_LEABSIZE) ;
	for (FILENO= 1; process_cmd() == 0; FILENO++) ;

	if (OPT_DEBUG) mrlp_msg("closing command socket") ;
	close(RSOCK) ;

	if (forked) {
		if (OPT_DEBUG) mrlp_msg("child exiting") ;
		exit(0) ;
	}
}

/*
	(int)stop_flag= process_cmd() ;

	Returns zero if expecting another command to follow.

*/
static int process_cmd()
{
	char cmdbuf[MRLP_CMDBSIZE+1] ;
	char *arg_p ;

	if (OPT_DEBUG) mrlp_msg("waiting for command") ;

	/*	read, perform a command		*/
	if (mrlp_getline(RSOCK, cmdbuf, MRLP_CMDBSIZE)) {
		if (OPT_DEBUG) mrlp_msg("cmd: unexpected EOF!") ;
		mrlp_abortjob("can't read cmd") ;
		return(1) ;		/* can't read command: abort job */
	}
	if (OPT_DEBUG) mrlp_smsg("command received", cmdbuf) ;

	/*	exit command			*/
	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_EXIT, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: exit") ;
		mrlpd_replyokrc(0) ;
		return(1) ;
	}

	/*	version check command		*/
	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_VERCHK, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: verchk") ;
		mrlpd_replymsg(
			(strcmp(arg_p, MRLPD_VER) == 0)?
			MRLPCMDR_OK: MRLPCMDR_ABORT,
			MRLPD_VER
		) ;
		return(0) ;
	}

	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_TESTHOST, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: testhost") ;
		sprintf(
			cmdbuf,
			"%s: jobid=%ld: up since: %s",
			lpma.lpm_pname,
			JOBNO, 
			lpma.lpm_start_time
		) ;
		mrlpd_replymsg(MRLPCMDR_OK, cmdbuf) ;
		return(0) ;
	}

	/*	check job command		*/
	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_JOBCHK, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: jobchk") ;
		mrlpd_replyokrc(JOB_RC) ;
		return(0) ;
	}

	/*	set print command		*/
	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_LPCMD, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: lpcmd") ;
		lpma.cur_lpexec[0]= '\0' ;
		strncpy(lpma.cur_lpargs, arg_p, MRLP_LECBSIZE) ;
		mrlpd_replyokrc(0) ;
		return(0) ;
	}

	/*	set print options		*/
	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_LPOPT, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: lpopts") ;
		strncpy(lpma.cur_lpopts, arg_p, MRLP_LEOBSIZE) ;
		mrlpd_replyokrc(0) ;
		return(0) ;
	}

	/*	print file command		*/
	if (mrlp_iscmdmsg(cmdbuf, MRLPCMD_PRINT, &arg_p)) {
		if (OPT_DEBUG) mrlp_msg("cmd: print") ;
		strncpy(lpma.cur_lpfn, arg_p, MRLP_LENBSIZE) ;
		if (doprintjob()) {
			mrlp_abortjob("lpjob failed") ;
			return(1) ;
		}
		return(0) ;
	}

	/*	invalid command			*/
	mrlp_abortjob("invalid command") ;
	return(1) ;
}

/*

	(int)rc= doprintjob() ;

	Performs current lp job, returns
	it's return code in global 'JOB_RC'.

	rc:
		0	Job performed ok
		!= 0	Error in job setup or execution

	logic:
		(1)	establish connected data transfer sockets
		(2)	start lp process, routing datasets using pipes
		(3)	transfer file to the lp processes
		(4)	close input data transfer socket, job stdin pipe
		(5)	wait for lp process, retrieve exitcode into 'JOB_RC'
		(6)	close output data transfer socket

*/
static int doprintjob()
{
	int csockdin, csockdout ;
	int lpcmd_stdinfh, lpcmd_stdoutfh ;

	/*
		(1)	Get connected data transfer sockets
	*/
	if (getdsockpair(&csockdin, &csockdout)) {
		return(1) ;
	}

	/*
		(2)	Start lp process, routing datasets using pipes
	*/
	lpcmd_stdinfh= -1 ;			/* create pipe to stdin      */
	lpcmd_stdoutfh= csockdout ;		/* lpcmd stdout to csockdout */
	signal(SIGCHLD, SIG_DFL) ;		/* establish normal waiting  */
	if (start_proc(&lpcmd_stdinfh, &lpcmd_stdoutfh, &LP_PID)) {
		mrlp_err("can't start lp process") ;
		close(csockdin) ;
		close(csockdout) ;
		return(2) ;
	}

	/*
		(3)	Transfer file to the lp process
	*/
	if (xfer_data(csockdin, lpcmd_stdinfh)) {
		close(csockdin) ;
		close(csockdout) ;
		return(3) ;
	}

	/*
		(4)	Close input data transfer sockets, job stdin pipe
	*/
	if (OPT_DEBUG) mrlp_msg("close lp job stdin pipe") ;
	close(lpcmd_stdinfh) ;
	if (OPT_DEBUG) mrlp_msg("close input data socket") ;
	close(csockdin) ;

	/*
		(5)	Wait for lp process, retrieve return code
	*/
	if (OPT_DEBUG) mrlp_msg("wait for lp job to complete") ;
	JOB_RC= wait_proc(LP_PID) ;
	if (OPT_DEBUG) mrlp_msg("lp job completed") ;

	/*
		(6)	Close the output data transfer socket
	*/
	if (OPT_DEBUG) mrlp_msg("close output data socket") ;
	close(csockdout) ;

	if (OPT_DEBUG) mrlp_msg("returning from print job") ;
	return(0) ;
}

/*

	(int)rc= getdsockpair(&csockdin, &csockdout) ;

	Gets pair of connected data sockets using dialogue
	with client connected on command socket.

	rc:
		0	pair of data sockets return in arguments
		!= 0	error

	logic:

		(1)	create data listening sockets using system-chosen ports
		(2)	inform client on command socket of port numbers
		(3)	accept connections on data listening sockets,
			returns "timeout" error status if timeout
		(4)	closes data listening sockets
		(5)	return data transfer sockets
*/
static int getdsockpair(csockdin_p, csockdout_p)
int *csockdin_p, *csockdout_p ;
{
	char jobidbuf[65] ;
	int csockdin, csockdout ;
	int lsockdin, lsockdout ;
	SOCKA_T lsockdina, lsockdouta ;
	SOCKA_T csockdina, csockdouta ;

	/*
		(1)	Create data listening sockets,
			let system choose port number
	*/
	memcpy((char *)&lsockdina, &LSOCKA, sizeof(SOCKA_T)) ;
	memcpy((char *)&lsockdouta, &LSOCKA, sizeof(SOCKA_T)) ;
	SOCKA_SPORT(lsockdina)= htons((u_short)0) ;
	SOCKA_SPORT(lsockdouta)= htons((u_short)0) ;
	if (OPT_DEBUG) mrlp_msg("issue data-in listen") ;
	if (issuelisten(&lsockdin, &lsockdina)) {
		mrlp_err("can't listen for data-in") ;
		return(-1) ;
	}
	if (OPT_DEBUG) mrlp_msg("issue data-out listen") ;
	if (issuelisten(&lsockdout, &lsockdouta)) {
		mrlp_err("can't listen for data-out") ;
		return(-1) ;
	}

	/*
		(2)	Inform client on command socket of port numbers
	*/
	sprintf(
		jobidbuf,
		"%d:%d:%ld:%d",
		(int) ntohs(SOCKA_SPORT(lsockdina)),
		(int) ntohs(SOCKA_SPORT(lsockdouta)),
		JOBNO,
		FILENO
	) ;
	mrlpd_replymsg(MRLPCMDR_OK, jobidbuf) ;

	/*
		(3)	Accept connections on data listening sockets,
			creating data transfer sockets.
	*/
	if (OPT_DEBUG) mrlp_msg("wait for data-in connection") ;
	if (waitaccept(lsockdin, &csockdin, &csockdina, 30)) {
		close(lsockdin) ;
		close(lsockdout) ;
		return(-2) ;
	}
	if (OPT_DEBUG) mrlp_msg("wait for data-out connection") ;
	if (waitaccept(lsockdout, &csockdout, &csockdouta, 30)) {
		close(lsockdin) ;
		close(lsockdout) ;
		return(-2) ;
	}

	/*
		(4)	Close data listening sockets
	*/
	if (OPT_DEBUG) mrlp_msg("close listening data sockets") ;
	close(lsockdin) ;
	close(lsockdout) ;

	/*
		(5)	Return data transfer sockets
	*/
	(*csockdin_p)= csockdin ;
	(*csockdout_p)= csockdout ;
	return(0) ;
}

static int xfer_data(in_fh, out_fh)
int in_fh, out_fh ;
{
	char iobuf[MRLP_IOBSIZE] ;
	int l, len ;

	if (OPT_DEBUG) mrlp_msg("transferring data to lp job") ;
	while(/*CONSTCOND*/1) {
		if (OPT_DEBUG> 1) mrlp_msg("reading") ;
		if ((len= read(in_fh, iobuf, sizeof(iobuf)))<= 0) break ;
		if (OPT_DEBUG> 1) mrlp_msg("writing") ;
		if ((l= write(out_fh, iobuf, len))< len) {
			mrlp_nerr("wrote short block", l) ;
		}
	}
	return(0) ;
}

static int issuelisten(lsock_p, lsocka_p)
int *lsock_p ;
SOCKA_T *lsocka_p ;
{
	int on_flag, addrlen ;

	if (((*lsock_p)= socket(AF_INET, SOCK_STREAM, 0))< 0) {
		mrlp_perr("socket") ;
		return(1) ;
	}

	on_flag= 1 ;

	if (setsockopt(
		(*lsock_p),
		SOL_SOCKET,
		SO_REUSEADDR,
		&on_flag,
		sizeof(on_flag)
	)< 0) {
		mrlp_perr("setsockopt") ;
		return(2) ;
	}

	if (bind(*lsock_p, lsocka_p, sizeof(SOCKA_T))< 0) {
		mrlp_smsg("bind failed", mrlp_inetaddr_str(lsocka_p)) ;
		mrlp_perr("bind") ;
		return(3) ;
	}

	if (listen(*lsock_p, 1)< 0) {
		mrlp_perr("listen") ;
		return(4) ;
	}

	addrlen= sizeof(SOCKA_T) ;
	if (getsockname(*lsock_p, lsocka_p, &addrlen)< 0) {
		mrlp_perr("getsockname") ;
		return(5) ;
	}

	return(0) ;
}

static int waitaccept(lsock, sock_p, socka_p, timeout)
int lsock ;				/* socket on which to listen */
int *sock_p ;				/* return socket descriptor  */
SOCKA_T *socka_p ;			/* return address of socket  */
int timeout ;				/* timeout # seconds or 0    */
{
	struct sockaddr csocka ;
	int csock, csocka_l ;

	csocka_l= sizeof(struct sockaddr) ;
	if ((csock= accept(lsock, &csocka, &csocka_l))< 0) {
		mrlp_perr("accept") ;
		inc_errs() ;
		return(1) ;
	}
	if (csocka_l != sizeof(SOCKA_T)) {
		mrlp_nerr("wrong accept socket address size", csocka_l) ;
		inc_errs() ;
		return(2) ;
	}
	memcpy((char *)socka_p, (char *)&csocka, sizeof(SOCKA_T)) ;
	(*sock_p)= csock ;
	return(0) ;
}

/*
	(int)rc= start_proc(&infh, &outfh, &pid) ;

	Forks lp process, routing stdin/stdout from 'infh' and 'outfh'.
	Returns 0 iff process forked and active, else error.
*/
static int start_proc(stdinfh_p, stdoutfh_p, pid_p)
int *stdinfh_p, *stdoutfh_p ;
pid_t *pid_p ;
{
	char cmd_argbuf[MRLP_LEABSIZE+1] ;
	char lit_argbuf[MRLP_LEABSIZE+1] ;
	char *args[MRLP_NEARGS+1] ;
	int rc ;

	makecmd_subst(cmd_argbuf) ;
	if (!OPT_QUIET) mrlp_smsg("lp cmd", cmd_argbuf) ;

	if ((rc= mrlpdcmd_parse(
		cmd_argbuf,
		args, MRLP_NEARGS+1,
		lit_argbuf, MRLP_LEABSIZE+1
	)) != 0) {
		mrlp_nerr("parseing lp command", rc) ;
		return(-1) ;
	}

	if (args[0] == (char *)NULL) {
		mrlp_err("lp command missing") ;
		return(-1) ;
	}

	if ((rc= mrlpdcmd_fork(
		(lpma.cur_lpexec[0]!='\0')? lpma.cur_lpexec: args[0],
		args,
		stdoutfh_p, stdinfh_p, pid_p
	)) != 0) {
		mrlp_nerr("forking lp command", rc) ;
		return(-2) ;
	}

	return(0) ;
}

/*

	(int)rc= wait_proc(pid) ;

	Waits for process 'pid' and retreives its exit code.

	rc:	
		-1		process status not available
		>= 0		exit() code from process
		<= 1000		-(1000+signo) causing process termination
		<= 2000		-(2000+signo) causing process to dump core

*/
static int wait_proc(pid)
pid_t pid ;
{
	int wrc, rc ;
	pid_t wpid ;

	rc= -1 ;				/* don't know return code */

	if (pid == (pid_t) 0) return(rc) ;

	if ((wpid= wait(&wrc)) != pid) {
		if (wpid< 0) mrlp_perr("wait for lp process") ;
		else         mrlp_nerr("wait for lp process", (int)wpid) ;
		return(rc) ;
	}

	if (WIFEXITED(wrc)) {
		rc= WEXITSTATUS(wrc) ;
	}
	else if (WIFSIGNALED(wrc)) {
		rc= -(1000 + WTERMSIG(wrc)) ;
#if 0
/* macro not available on SPARC */
		if (WCOREDUMP(wrc)) rc-= 1000 ;
#endif
	}
	return(rc) ;
}

static void sigcatch(signo)
int signo ;
{
	signal(signo, SIG_IGN) ;

	switch(signo) {

		case	SIGHUP:			/* kill current job */
			if (LP_PID != 0) {
				mrlp_abort("job aborted by user signal") ;
				kill_lpjob() ;
			}
			break ;

		case	SIGTERM:		/* kill daemon */
			kill_lpjob() ;
			mrlp_abort("terminated by user signal") ;
			break ;

		case	SIGPIPE:
			if (LP_PID != 0) {
				mrlp_abort("job aborted: SIGPIPE") ;
				kill_lpjob() ;
			}
			break ;

		default:
			mrlp_nmsg("ignored", "signal", signo) ;
			break ;
	}

	signal(signo, sigcatch) ;
}	

static void kill_lpjob()
{
	if (LP_PID != 0) {
		if (kill(LP_PID, SIGKILL) == 0) {
			mrlp_nmsg("killed lpjob", "pid", (int)LP_PID) ;
		}
		LP_PID= 0 ;
	}
}

static void inc_errs()
{
	if (++ERRS> MAX_ERRS) {
		mrlp_abort("giving up due to excessive errors") ;
		/*NOTREACHED*/
	}
}

static int mrlpd_replymsg(rtype, msg)
char *rtype, *msg ;
{
	char cmdbuf[MRLP_CMDBSIZE+1] ;

	if (OPT_DEBUG) mrlp_smsg("send reply", rtype) ;

	sprintf(cmdbuf, "%s%s\n", rtype, msg) ;
	return(mrlp_wsock(RSOCK, cmdbuf, strlen(cmdbuf), cmdbuf)) ;
}

static int mrlpd_replyokrc(rc)
int rc ;
{
	char cmdbuf[MRLP_CMDBSIZE+1] ;

	sprintf(cmdbuf, "%s%d\n", MRLPCMDR_OK, rc) ;
	return(mrlp_wsock(RSOCK, cmdbuf, strlen(cmdbuf), cmdbuf)) ;
}

static void mrlp_abortjob(msg)
char *msg ;
{
	mrlp_serr("job aborted", msg) ;
	mrlpd_replymsg(MRLPCMDR_ABORT, msg) ;
}

/*
	command-argument substitution, using "substr()" function.

	NOTE:	NOT protected!

		substr() command does not know how long the
		output string is, therefore it may overflow!

*/

static void cmdsubproc(outb, outblp, fmt, value)
char *outb, *value, *fmt ;
int *outblp ;
{
	memcpy(outb, value, (*outblp)= strlen(value)) ;
}
	
static struct sub_item cmdsub[]= {
	{ "@FN",  (char *)NULL, (void *)lpma.cur_lpfn, cmdsubproc },
	{ "@OPT", (char *)NULL, (void *)lpma.cur_lpopts, cmdsubproc }
} ;
#define	N_SUBITEMS	(sizeof(cmdsub)/sizeof(cmdsub[0]))

static void makecmd_subst(cmdbuf)
char *cmdbuf ;
{
	int i ;

	memcpy(cmdbuf, lpma.cur_lpargs, MRLP_LEABSIZE) ;
	for (i= 0; i< N_SUBITEMS; i++) {
		substr(cmdbuf, &cmdsub[i]) ;		/* see NOTE: */
	}
}
