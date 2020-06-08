/*

	pw_tio.c
					Copyright (C) 1992, Contahal Ltd.

	System-dependent terminal I/O subroutines.

*/

#include	<stdio.h>
#include	"compiler.h"

#if	OPSYS==OPUNIX

#include	<termio.h>
#include	<signal.h>
#include	<setjmp.h>

int pw_tio_init()
{
	if (term_init(0, -1)) return(1) ;
	term_init(0, 1) ;
	return(0) ;
}

void pw_tio_fini()
{
	term_init(0, 0) ;
}

static void to_sigfn() ;
static jmp_buf to_jmp ;

#define		SETALARM(sec, fn) \
		signal(SIGALRM, fn) ;\
		alarm(sec)

#define		RESETALARM() \
		signal(SIGALRM, SIG_DFL) ;\
		alarm(0)

int pw_tio_getch(to_sec)
int to_sec ;
{
	int rrc ;
	unsigned char c ;

	if (to_sec> 0) {
		if (setjmp(to_jmp)) {
			RESETALARM() ;
			return(-1) ;
		}
		SETALARM(to_sec, to_sigfn) ;
	}
		
	rrc= read(0, &c, 1) ;

	if (to_sec> 0) {
		RESETALARM() ;
	}

	return((rrc> 0)? (int) c: -2) ;
}

static void to_sigfn(signo)
int signo ;
{
	signal(signo, to_sigfn) ;
	longjmp(to_jmp, 1) ;
}

/*

	Terminal initialization function:

	(int)rc= term_init(fh, arg) ;

		arg :=
		(1)	<0  save current status
		(2)	>0  apply "changes" to terminal
		(3)	0   restore saved status

	caller should not call using (2) or (3) if (1) returned
	non-zero (input device not a terminal).

	"changes" made to terminal:

	if terminal device
		NON-CANONICAL input processing set:
			MIN set to 1 (reads satisfied by 1 char)
			TIME set to 0 (indefinate block for char)

	if non-terminal device
		nothing done

	NOTES:
	(1)	Uses "termio(7)" terminal interface (AT&T Unix V),
		with "file handle" of input dataset (terminal devices).

	(2)	returns non-zero return code if the "file handle"
		does not belong to a terminal device, or if the
		desired setup could not be achieved.

	(3)	Sets up terminal device to do single-character reads.

*/
static int term_init(fh, arg)
int fh, arg ;
{
	static struct termio termstat ;
	struct termio newstat ;

	if (arg< 0) {				/* init: read & save status */
		return(ioctl(fh, TCGETA, (char *)&termstat)< 0) ;
	}

	if (arg> 0) {					/* apply change */
		memcpy((char *)&newstat,
			(char *)&termstat,		/* copy status */
			sizeof(struct termio));	
		newstat.c_lflag&= ~(ECHO | ICANON) ;
		newstat.c_cc[VMIN]= 1 ;		/* block until 1 char */
		newstat.c_cc[VTIME]= 0 ;	/* forever */

							/* modify state */
		if (ioctl(fh, TCSETA, (char *)&newstat)< 0) {
			return(1) ;
		}
		return(0) ;				/* done */
	}

						/* replace state */
	return(ioctl(fh, TCSETA, (char *)&termstat)< 0) ;
}

#if 0

#include	<poll.h>

#define		POLLEVENTS	(POLLIN | POLLRDNORM)

int term_chkinp(fd)
int fd ;
{
	struct pollfd fds ;
	int i ;

	fds.fd= fd ;
	fds.events= POLLEVENTS ;
	i= poll(&fds, 1, 0) ;		/* poll one pollfd struct, no-wait */
	if (i> 0) {
		return((fds.revents & POLLEVENTS) != 0) ;
	}
	return(0) ;
}
#endif

#endif

