#include	<sys/socket.h>
#include	<sys/un.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<unistd.h>

void main()
{
	int i, l, k, did, nfd, rc ;
	int sin, sout ;
	struct sockaddr_un sain, saout ;
	char buf[128] ;
	fd_set rfd, wfd ;

	sin= socket(AF_UNIX, SOCK_STREAM, 0) ;
	if (sin< 0) {
		perror("in socket") ;
		exit(1) ;
	}

	sout= socket(AF_UNIX, SOCK_STREAM, 0) ;
	if (sout< 0) {
		perror("out socket") ;
		exit(2) ;
	}

	memset(&sain, '\0', sizeof(sain)) ;
	sain.sun_family= AF_UNIX ;
	strcpy(sain.sun_path, "mtalk.so") ;

	memset(&saout, '\0', sizeof(saout)) ;
	saout.sun_family= AF_UNIX ;
	strcpy(saout.sun_path, "mtalk.si") ;

	unlink(sain.sun_path) ;
	if (bind(sin, (struct sockaddr *) &sain, sizeof(sain))< 0) {
		perror("bind: input") ;
		exit(4) ;
	}

	unlink(saout.sun_path) ;
	if (bind(sout, (struct sockaddr *) &saout, sizeof(saout))< 0) {
		perror("bind: output") ;
		exit(5) ;
	}

	nfd= sin ;
	if (nfd< sout) nfd= sout ;

	printf("sin=%d sout=%d\n", sin, sout) ;

	while(1) {
		FD_ZERO(&rfd) ;
#if 0
		FD_SET(sin, &rfd) ;
#endif
		FD_ZERO(&wfd) ;
		FD_SET(sout, &wfd) ;
		rc= select(
			nfd + 1,
			&rfd,
			&wfd,
			(fd_set *) 0,
			(struct timeval *) 0
		) ;
		if (rc< 0) {
			perror("select") ;
			break ;
		}
		printf("select returns %d\n", rc) ;
		did= 0 ;
		if (FD_ISSET(sin, &rfd)) {
			did++ ;
			l= read(sin, buf, sizeof(buf)) ;
			if (l< 0) {
				perror("read") ;
				continue ;
			}
			buf[l]= '\0' ;
			printf("s(%d)='%s'\n", l, buf) ;
		}
		if (FD_ISSET(sout, &wfd)) {
			did++ ;
			sprintf(buf, "from s=%d, t3 # %d\n", sout, i) ;
			k= strlen(buf) ;
#if 0
			l= write(sout, buf, k) ;
#else
			l= send(
				sout,
				buf,
				k,
				0
			) ;
#endif
			if (l != k) {
				printf("%d != %d\n", l, k) ;
			}
			if (l< 0) {
				perror("write") ;
				break ;
			}
		}
		if (did == 0) {
			printf("did nothing!\n") ;
		}
	}

	close(sin) ;
	close(sout) ;
}
