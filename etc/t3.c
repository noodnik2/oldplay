
#include	<stdio.h>
#include	<fcntl.h>
#include	<errno.h>

void main()
{
	int fh ;

	if ((fh= open("/dev/tty00s", O_RDONLY))< 0) {
		perror("/dev/tty00s: can't open") ;
		fprintf(stderr, "(errno=%d)\n", errno) ;
		exit(1) ;
	}
	close(fh) ;
	fprintf(stderr, "opened no problem\n") ;
	exit(0) ;
}
