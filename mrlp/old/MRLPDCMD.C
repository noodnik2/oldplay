/*

	mrlpdcmd.c:			Marty's Remote Line Printer Daemon
					Copyright (C) 1992, Contahal Ltd.


	Unix fork/exec interface.

	(int)rc= mrlpdcmd_parse() ;		//	Parse command
	(int)rc= mrlpdcmd_fork() ;		//	Fork/exec command
	
*/


#include	<stdio.h>
#include	<signal.h>
#include	<unistd.h>

#include	"compiler.h"
#include	"mrlpsub.h"

static void fork_failed() ;
static char *find_ws() ;
static char *find_nws() ;

/*
	(int)rc= mrlpdcmd_parse(cmdl, e_args, m_args, abuf, abuf_size) ;

	Parses command "cmdl" into "e_args[]" array structure, suitable for
	passing to "execvp(2)" (max. number of elements m_args).  Uses user
	buffer "abuf" for storing the literal arguments (max. size abuf_size).

	returns:

		rc:	0	parsed ok
			1	too many arguments found (>m_args)
			2	too much literal data (>abuf_size)	


*/
int mrlpdcmd_parse(cmdline, args, max_args, argbuf, argbuf_size)
char *cmdline, *args[], *argbuf ;
int max_args, argbuf_size ;
{
	char *in_p, *in_p_next, *out_p, *out_p_max ;
	int i, len ;

	in_p= find_nws(cmdline) ;
	out_p= argbuf ;
	out_p_max= (argbuf + argbuf_size) ;
	
	max_args-- ;
	for (i= 0; i< max_args; i++) {
		if ((*in_p) == '\0') break ;
		in_p_next= find_ws(in_p) ;
		len= (int) (in_p_next - in_p) ;
		if ((out_p + len + 1)> out_p_max) {
			break ;
		}
		args[i]= out_p ;
		memcpy(out_p, in_p, len) ;
		out_p+= len ;
		(*out_p++)= '\0' ;
		in_p= find_nws(in_p_next) ;
	}
	args[i]= (char *)NULL ;

	if ((*in_p) != '\0') {			/* stopped early -- why? */
		if (i == max_args) return(1) ;	/* too many arguments    */
		return(2) ;			/* buffer overflow       */
	}
	return(0) ;
}
		
/*
	macro CATCH_SIGCHLD:
		set to '1' if parent is catching SIGCHLD signal,
		or '0' if parent is not catching SIGCHLD signal
*/
#define		CATCH_SIGCHLD	0

/*

	(int)rc= mrlpdcmd_fork(cmd, args, stdoutfh_p, stdinfh_p, pid_p) ;

	returns:

		rc:	0	child was forked
			!= 0	no child was forked

	Sets up for child lp process, mapping stdin and stdout to local
	datasets if desired, and forks and execs the child lp process
	using the "execvp(2)" system call.

	arguments:

		cmd	     "execvp(2)" 'file' argument: command
		args	     "execvp(2)" 'argv' argument: argument array

		(*pid_p)     filled in on return with pid of forked lp cmd

		stdinfh_p    if null pointer, lp cmd stdin will be our stdin,
			     if points to file handle containing (-1),
			     then file handle will be filled in with file
			     descriptor of pipe created for writing lp cmd
			     stdout, else points to open file handle from
			     which lp cmd stdin will be read.

		stdoutfh_p   if null pointer, lp cmd stdout will be ignored,
			     if points to file handle containing (-1),
			     then file handle will be filled in with file
			     descriptor of pipe created for reading lp cmd
			     stdout, else points to open file handle where
			     lp cmd stdout will be written.

*/
int mrlpdcmd_fork(cmd, args, stdoutfh_p, stdinfh_p, pid_p)
char *cmd, *args[] ;
int *stdoutfh_p, *stdinfh_p ;
pid_t *pid_p ;
{
	pid_t pid ;
	int lpo, lpo_fhp[2] ;
	int lpi, lpi_fhp[2] ;

	/*
		Set flags and standard file I/O descriptors:

		       lpi	not (0) iff to create pipe for lp cmd stdin
		       lpo	not (0) iff to create pipe for lp cmd stdout
		
		lpi_fhp[0]	not (-1) iff to use as lp cmd stdin file descr
		lpo_fhp[1]	not (-1) iff to use as lp cmd stdout file descr
		
	*/
	lpi_fhp[0]= lpo_fhp[1]= (-1) ;
	if (lpi= (stdinfh_p != (int *)NULL)) {
		if ((*stdinfh_p) != (-1)) {
			lpi_fhp[0]= (*stdinfh_p) ;
			lpi= 0 ;
		}
	}
	if (lpo= (stdoutfh_p != (int *)NULL)) {
		if ((*stdoutfh_p) != (-1)) {
			lpo_fhp[1]= (*stdoutfh_p) ;
			lpo= 0 ;
		}
	}

	/*
		create pipes for stdin, stdout, according to lpi, lpo flags
		return nonzero return code to caller if can't create pipe(s)
	*/
	if (lpi) {
		if (pipe(lpi_fhp) == -1) {
			mrlp_perr("can't make lp input pipe") ;
			return(2) ;
		}
	}

	if (lpo) {
		if (pipe(lpo_fhp) == -1) {
			mrlp_perr("can't make lp output pipe") ;
			return(1) ;
		}
	}

	/*
		fork new process
		return nonzero return code to caller if can't fork
	*/
	if ((pid= fork()) == (pid_t) -1) {
		mrlp_perr("can't fork lp process") ;
		return(3) ;
	}

	/*
		setup for both parent and child processes:
	*/
	if (pid != (pid_t) 0) {

		/*
			We're in the parent process:

			(1)	return PID of the forked process,
			(2)	return file descriptors of ends belonging to
				parent of any created pipes,
			(3)	close the ends belonging to the child of any
				created pipes,
			(4)	return to parent with return code 0.

		*/
		(*pid_p)= pid ;
		if (lpi) {
			close(lpi_fhp[0]) ;
			(*stdinfh_p)= lpi_fhp[1] ;
		}
		if (lpo) {
			close(lpo_fhp[1]) ;
			(*stdoutfh_p)= lpo_fhp[0] ;
		}
		return(0) ;
	}

	/*
		We're in the child process:

		(1)	close ends belonging to parent of any created pipes,
		(2)	re-direct stdin from remote input source, if any
		(3)	re-direct stdout to remote output destination, if any
		(4)	fork

	*/
	if (lpi) close(lpi_fhp[1]) ;
	if (lpo) close(lpo_fhp[0]) ;

#if	CATCH_SIGCHLD
	/*
		sleep a little:	(if SIGCHLD is being caught by parent)
		allow parent to retrieve forked process pid

		this is a KLUDGE and I'm not proud of it
		(there's got to be a better way! - maybe
		to arrange for parent send us a signal?)
	*/
	sleep(1) ;
#endif

	/*
		re-direct stdin/stdout datasets:

		(1)	stdin comes from "lpi_fhp[0]" (if defined)
		(2)	stdout written to "lpo_fhp[1]" (if defined)

	*/
	if (lpi_fhp[0] != (-1)) {
		close(0) ;
		if (dup(lpi_fhp[0]) != 0) {
			mrlp_abort("can't dup(2) to stdin") ;
			/*NOTREACHED*/
		}
		close(lpi_fhp[0]) ;
	}

	if (lpo_fhp[1] != (-1)) {
		close(1) ;
		if (dup(lpo_fhp[1]) != 1) {
			mrlp_abort("can't dup(2) to stdout") ;
			/*NOTREACHED*/
		}
		close(lpo_fhp[1]) ;
	}

	/*
		now exec user command in this environment
	*/
#if 0
	setpgrp() ;
#endif
	execvp(cmd, args) ;
		
	fork_failed(cmd) ;
	/*NOTREACHED*/
}

static void fork_failed(cmd)
char *cmd ;
{
	char msgbuf[MRLP_MSGBSIZE+1] ; 
		
	sprintf(msgbuf, "can't execvp: cmd='%s'", cmd) ;
	mrlp_abort(msgbuf) ;
	/*NOTREACHED*/
}

/*
	internal (static) subroutines
*/

static char *find_nws(s)
register char *s ;
{
	while(((*s) == ' ') || ((*s) == '\t')) s++ ;
	return(s) ;
}

static char *find_ws(s)
register char *s ;
{
	while((*s) && ((*s) != ' ') && ((*s) != '\t')) s++ ;
	return(s) ;
}
