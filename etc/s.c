#include	<stdio.h>
#include	<fcntl.h>

#include	"compiler.h"

#if	OPSYS==MSDOS
static char ttydev[]= "CON" ;
#endif

#if	OPSYS==OPUNIX
static char ttydev[]= "/dev/tty" ;
#endif

int systtycmd(cmd)
char *cmd ;
{
	int rc ;
	int old_fh[3] ;

	old_fh[0]= dup(0) ; close(0) ; open(ttydev, O_RDONLY) ;
	old_fh[1]= dup(1) ; close(1) ; open(ttydev, O_WRONLY) ;
	old_fh[2]= dup(2) ; close(2) ; open(ttydev, O_WRONLY) ;

	/* must modify the TTY line settings here (if changed by curses) */
	rc= system(cmd) ;
	/* must restore the TTY line setting if changed here */

	close(0) ; dup(old_fh[0]) ; close(old_fh[0]) ;
	close(1) ; dup(old_fh[1]) ; close(old_fh[1]) ;
	close(2) ; dup(old_fh[2]) ; close(old_fh[2]) ;
	
	return(rc) ;
}
