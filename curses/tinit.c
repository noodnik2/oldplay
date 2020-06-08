
#include	<stdio.h>
#include	<signal.h>
#include	<termio.h>

#define		UCC_CHAR	0x01

static struct termio termstat ;

/*

	(void) tinit(brkrtn, modeflag)

		makes changes to terminal, keeps old status

		brkrtn -> user defined interrupt signal handler

		modeflag :=
			0   do only ONLCR on output
			~0  do OPOST on output

	(void) tstop()

		undoes changes done by previous tinit().

*/

void tinit(cleanup, mode)
void (*cleanup)() ;
int mode ;
{
	struct termio newstat ;

	ioctl(0, TCGETA, (char *)&termstat);	/* read status */
	signal(SIGINT, cleanup) ;

	memcpy((char *)&newstat,
		(char *)&termstat,		/* copy status */
		sizeof(struct termio));	

	newstat.c_lflag&= ~(ICANON | ECHO) ;	/* line disc.  */
	if (mode) {
		newstat.c_oflag&= ~(OPOST) ;	/* output disc */
	}
	else {
		newstat.c_oflag|= (OPOST) ;
		newstat.c_oflag&= ~(OCRNL) ;
	}
	newstat.c_cc[VINTR]=  (char)UCC_CHAR ;	/* cntrl codes */
	newstat.c_cc[VQUIT]=  (char)UCC_CHAR ;
	newstat.c_cc[VERASE]= (char)UCC_CHAR ;
	newstat.c_cc[VKILL]=  (char)UCC_CHAR ;
	newstat.c_cc[VEOF]=   (char)UCC_CHAR ;
	newstat.c_cc[VEOL]=   (char)UCC_CHAR ;
	newstat.c_cc[VSWTCH]= (char)UCC_CHAR ;

	ioctl(0, TCSETAF, (char *)&newstat) ; 	/* modify stat */

}

void tstop()
{
	ioctl(0, TCSETAF, (char *)&termstat) ; 		/* replace stat */
	signal(SIGINT, SIG_DFL) ;
}

