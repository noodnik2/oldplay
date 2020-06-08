#include	<stdio.h>
#include	<signal.h>
#include	<errno.h>

#define		MAX_ARGBUF	40
#define		MAX_ARGS	5

void mrlp_nerr() ;
void mrlp_msg() ;
void mrlp_abort() ;
void mrlp_perr() ;

static int c_pid= 0 ;
static void kill_chld() ;
static void sigcatch() ;

void main(argc, argv)
int argc ;
char *argv[] ;
{
	char argbuf[MAX_ARGBUF] ;
	char *args[MAX_ARGS] ;
	int i, nargs, rc ;
	int ofh ;

	atexit(kill_chld) ;
	
	signal(SIGPIPE, sigcatch) ;
	signal(SIGTERM, sigcatch) ;
	signal(SIGHUP, sigcatch) ;
	signal(SIGCHLD, sigcatch) ;

	for (i= 1; i< argc; i++) {

		if ((rc= mrlpdcmd_parse(
			argv[i],
			&nargs, args, MAX_ARGS,
			argbuf, MAX_ARGBUF
		)) != 0) {
			printf("r(%d); parse(%s)\n", rc, argv[i]) ;
			continue ;
		}

		if ((rc= mrlpdcmd_fork(
			args[0],
			args, nargs,
			(int *)NULL, &ofh, &c_pid
		)) != 0) {
			printf("r(%d); fork(%s)\n", rc, args[0]) ;
			continue ;
		}
			
		printf("process forked: cmd=(%s): pid=%d\n", argv[i], c_pid) ;
		printf("now, enter stuff, then \"exit\"\n") ;

		while(c_pid != 0) {
			printf("Enter string: ") ; fflush(stdout) ;
			gets(argbuf) ;
			write(ofh, argbuf, strlen(argbuf)) ;
			if (strcmp(argbuf, "exit") == 0) break ;
		}

		close(ofh) ;
	}
}

static void sigcatch(signo)
int signo ;
{
	signal(signo, SIG_IGN) ;

	switch(signo) {

		case	SIGCHLD:
			mrlp_nerr("SIGCHLD", c_pid) ;
			c_pid= 0 ;
			break ;

		case	SIGHUP:			/* kill current job */
			mrlp_msg("SIGHUP") ;
			kill_chld() ;
			break ;

		case	SIGTERM:		/* kill daemon */
			mrlp_msg("SIGTERM") ;
			kill_chld() ;
			mrlp_abort("terminated by user signal") ;
			break ;

		case	SIGPIPE:
			mrlp_msg("SIGPIPE") ;
			kill_chld() ;
			break ;

		default:
			mrlp_nerr("ignored signal", signo) ;
			break ;
	}

	signal(signo, sigcatch) ;
}	

static void kill_chld()
{
	if (c_pid != 0) {
		printf("killing child: pid=%d\n", c_pid) ;
		if (kill(c_pid, SIGKILL)< 0) {
			mrlp_perr("kill failed") ;	/* ESRCH */
			c_pid= 0 ;
		}
	}
}

void mrlp_nerr(msg, n)
char *msg ;
int n ;
{
	fprintf(stderr, "tf: %s, code=%d\n", msg, n) ;
	fflush(stderr) ;
}

void mrlp_abort(msg)
char *msg ;
{
	fprintf(stderr, "tf: aborted: %s\n", msg) ;
	fflush(stderr) ;
	exit(1) ;
}

void mrlp_msg(msg)
char *msg ;
{
	fprintf(stderr, "tf: %s\n", msg) ;
	fflush(stderr) ;
}

void mrlp_perr(msg)
char *msg ;
{
	fprintf(stderr, "tf: ") ;
	fflush(stderr) ;
	perror(msg) ;
}
