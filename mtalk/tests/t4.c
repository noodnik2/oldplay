#include	<stdio.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<sys/time.h>
#include	<sys/types.h>
#include	<unistd.h>

void main()
{
	int l, j, wn, rc, fdin, fdout, nfd ;
	int did ;
	char buf[128] ;
	fd_set rfd ;
	fd_set wfd ;

	fdin= open("jp.in", O_RDONLY | O_NDELAY) ;
	if (fdin< 0) {
		fprintf(stderr, "errno=%d\n", errno) ;
		perror("open in") ;
		exit(1) ;
	}
#if 0
	fdout= open("jp.out", O_WRONLY | O_NDELAY) ;
#else
	fdout= open("jp.out", O_RDWR | O_NDELAY) ;
#endif
	if (fdout< 0) {
		fprintf(stderr, "errno=%d\n", errno) ;
		perror("open out") ;
		exit(1) ;
	}

	printf("pipes opened\n") ;

	nfd= fdin ;
	if (nfd< fdout) nfd= fdout ;
	wn= 0 ;

	while(1) {
		FD_ZERO(&rfd) ;
		FD_SET(fdin, &rfd) ;
		FD_ZERO(&wfd) ;
		FD_SET(fdout, &wfd) ;
		rc= select(
			nfd+1,
			&rfd,
			&wfd,
			(fd_set *) 0,
			(struct timeval *) 0
		) ;
		if (rc< 0) {
			perror("select") ;
			break ;
		}
		did= 0 ;
		if (FD_ISSET(fdin, &rfd)) {
			did++ ;
			l= read(fdin, buf, sizeof(buf)) ;
			if (l< 0) {
				perror("read") ;
				break ;
			}
			printf("read s(%d)='%s'\n", l, buf) ;
		}
		if (FD_ISSET(fdout, &wfd)) {
			did++ ;
			j= sprintf(buf, "this is write # %d\n", wn++) ;
			l= write(fdout, buf, j) ;
			if (l != j) {
				printf("wrote %d/%d\n", l, j) ;
			}
			if (l< 0) {
				perror("write") ;
				break ;
			}
			printf("wrote s(%d)='%s'\n", l, buf) ;
		}
		if (did == 0) {
			printf("did nothing!!\n") ;
		}
	}
	
	close(fdin) ;
	close(fdout) ;
}
