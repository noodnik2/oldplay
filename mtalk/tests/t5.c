#include	<errno.h>
#include	<sys/socket.h>
#include	<sys/un.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<unistd.h>

static char *pszToWrite= "hello there\n" ;

void main()
{
	int s, s2 ;
	int paddr_len ;
	fd_set fds ;
	struct sockaddr_un saddr ;
	struct sockaddr_un paddr ;
	char buf[128] ;

	s= socket(AF_UNIX, SOCK_DGRAM, 0) ;
	if (s< 0) {
		perror("socket") ;
		exit(1) ;
	}

	memset(&saddr, '\0', sizeof(saddr)) ;
	saddr.sun_family= AF_UNIX ;
	strcpy(saddr.sun_path, "sock") ;
	unlink(saddr.sun_path) ;
	if (bind(s, (struct sockaddr *) &saddr, sizeof(saddr))< 0) {
		perror("bind") ;
		exit(4) ;
	}

	listen(s, 5) ;

	while(/*CONSTCOND*/1) {
		FD_ZERO(&fds) ;
		FD_SET(s, &fds) ;
		if (select(s, 0, &fds, 0, 0)< 0) {
			perror("select") ;
			exit(5) ;
		}
		printf("selected\n") ;
		paddr_len= sizeof(paddr) ;
		s2= accept(s, (struct sockaddr *) &paddr, &paddr_len) ;
		write(s2, pszToWrite, strlen(pszToWrite)) ;
		close(s2) ;
	}

	close(s) ;
}
